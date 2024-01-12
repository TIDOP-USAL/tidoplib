#include "Renderer.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 color;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
"color = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 color;"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

namespace tl
{

Renderer::Renderer(unsigned int _viewportWidth, unsigned int _viewportHeight)
	: viewportWidth(_viewportWidth), viewportHeight(_viewportHeight), 
	previousMouse({ 0, 0 }) {
}

void Renderer::init() 
{
	Shader vertexShader = Shader::fromCode(vertexShaderSource, Shader::ShaderType::Vertex);
	Shader fragmentShader = Shader::fromCode(fragmentShaderSource, Shader::ShaderType::Fragment);
	shaderProgram = ShaderProgram::New(vertexShader, fragmentShader);

	double aspectRatio = static_cast<double>(viewportWidth) / viewportHeight;
	camera = TrackballCamera::perspectiveCamera(consts::grad_to_rad<float> * 45.0f, aspectRatio, 0.1f, 1000.f);
}

void Renderer::resize(int w, int h)
{
	viewportWidth = w;
	viewportHeight = h;
	glViewport(0, 0, w, h);

	float theta = camera->getTheta();
	float phi = camera->getPhi();
	float radius = camera->getRadius();

	double aspectRatio = static_cast<double>(w) / h;
	camera = TrackballCamera::perspectiveCamera(consts::grad_to_rad<float> * 45.0f, aspectRatio, 0.1, 1000);

	camera->setTheta(theta);
	camera->setPhi(phi);
	camera->setRadius(radius);
}
	
void Renderer::render() 
{
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	shaderProgram->useProgram();

	shaderProgram->uniformMat4("view", camera->getViewMatrix());
	shaderProgram->uniformMat4("projection", camera->getProjectionMatrix());

	for (auto& model : models) {

		shaderProgram->uniformMat4("model", model->getModelMatrix());
		model->draw();
	}
}

void Renderer::pan(int mouseX, int mouseY) 
{
	float dx = (mouseX - previousMouse.x()) / (static_cast<float>(viewportWidth) / 2);
	float dy = (mouseY - previousMouse.y()) / (static_cast<float>(viewportHeight) / 2);

	const float panSensitivity = 1.0f;
	previousMouse = Vector2i({ mouseX, mouseY });

	camera->pan(dx * panSensitivity, dy * panSensitivity);
}

void Renderer::rotate(int mouseX, int mouseY) 
{
	float dTheta = (mouseX - previousMouse.x()) / static_cast<float>(viewportWidth);
	float dPhi = (mouseY - previousMouse.y()) / static_cast<float>(viewportHeight);

	const float sensitivity = 1.0f;
	previousMouse = Vector2i({ mouseX, mouseY });

	camera->rotate(-dTheta * sensitivity, -dPhi * sensitivity);
}

void Renderer::zoom(int delta)
{
	float sensitivity = 1.0f;
	camera->zoom(sensitivity * delta);
}

}