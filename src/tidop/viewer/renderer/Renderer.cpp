#include "Renderer.h"

#include <boost/smart_ptr/shared_ptr.hpp>

#include "group/Mesh.h"

const char* vertexShaderSource = "#version 330 core\n"
"\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"layout (location = 2) in vec3 aNormal;\n"
"layout (location = 3) in vec2 aTexCoord;\n"
"\n"
"out vec4 Color;\n"
"out vec3 Normal;\n"
"out vec2 TexCoord;\n"
"\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);\n"
"	Color = aColor;\n"
"	Normal = aNormal;\n"
"	TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"in vec4 Color;\n"
"in vec3 Normal;\n"
"in vec2 TexCoord;\n"
"\n"
"uniform sampler2D tex;\n"
"uniform bool hasTexture;\n"
"\n"
"void main()\n"
"{\n"
"   vec4 color = Color;\n"
"	if(hasTexture) {\n"
"		color = texture(tex, TexCoord);\n"
"   }\n"
"	FragColor = color;\n"
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

	float aspectRatio = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
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
	camera = TrackballCamera::perspectiveCamera(consts::grad_to_rad<float> * 45.0f, aspectRatio, 0.1, 10000);

	camera->setTheta(theta);
	camera->setPhi(phi);
	camera->setRadius(radius);
}
	
void Renderer::render() 
{
	//glEnable(GL_BLEND);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	shaderProgram->useProgram();

	shaderProgram->uniformMat4("view", camera->getViewMatrix());
	shaderProgram->uniformMat4("projection", camera->getProjectionMatrix());

	for (const auto& model : models) {

		// Mesh uniforms
		if(model->getType() == ModelBase::Type::Mesh) {

			Mesh::Ptr mesh = std::static_pointer_cast<Mesh>(model);

			//Texture uniforms
			shaderProgram->uniformInt("hasTexture", false);
			for(auto& texture : mesh->getTextures()) {
			
				if (texture->getType() == Texture::Type::TextureDiffuse) {
			
					texture->bind();
					//shaderProgram->uniformInt("tex", texture->getID() - 1);
					shaderProgram->uniformInt("tex", texture->getSlot() - 0x84C0);
					shaderProgram->uniformInt("hasTexture", true);
				}
			}

			/*
			const auto texture = mesh->getTextures()[0];

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->getID());

			shaderProgram->uniformInt("hasTexture", true);
			shaderProgram->uniformInt("tex", 0);
			*/

		}else {
			shaderProgram->uniformInt("hasTexture", false);
		}

		shaderProgram->uniformMat4("model", model->getModelMatrix());
		model->draw();
	}
}

void Renderer::pan(int mouseX, int mouseY) 
{
	float dx = (mouseX - previousMouse.x()) / (static_cast<float>(viewportWidth) / 2);
	float dy = (mouseY - previousMouse.y()) / (static_cast<float>(viewportHeight) / 2);

	constexpr float panSensitivity = 100.0f;
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
	constexpr float sensitivity = 10.0f;
	camera->zoom(sensitivity * delta);
}

}