#include "Renderer.h"

#include <boost/smart_ptr/shared_ptr.hpp>

#include "group/Mesh.h"


namespace tl
{

Renderer::Renderer(unsigned int _viewportWidth, unsigned int _viewportHeight)
	: viewportWidth(_viewportWidth), viewportHeight(_viewportHeight), 
	previousMouse({ 0, 0 }) {
}

void Renderer::init() 
{
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


	for (const auto& model : models) {

		model->getShaderProgram()->useProgram();

		model->getShaderProgram()->uniformMat4("view", camera->getViewMatrix());
		model->getShaderProgram()->uniformMat4("projection", camera->getProjectionMatrix());

		// Mesh uniforms
		if(model->getType() == ModelBase::Type::Mesh) {

			Mesh::Ptr mesh = std::static_pointer_cast<Mesh>(model);

			//Texture uniforms
			model->getShaderProgram()->uniformInt("hasTexture", false);
			for(auto& texture : mesh->getTextures()) {
			
				if (texture->getType() == Texture::Type::TextureDiffuse) {
			
					texture->bind();
					//model->getShaderProgram()->uniformInt("tex", texture->getID() - 1);
					model->getShaderProgram()->uniformInt("tex", texture->getSlot() - 0x84C0);
					model->getShaderProgram()->uniformInt("hasTexture", true);
				}
			}

			/*
			const auto texture = mesh->getTextures()[0];

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->getID());

			model->getShaderProgram()->uniformInt("hasTexture", true);
			model->getShaderProgram()->uniformInt("tex", 0);
			*/

		}else {
			model->getShaderProgram()->uniformInt("hasTexture", false);
		}

		model->getShaderProgram()->uniformMat4("model", model->getModelMatrix());
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
	constexpr float sensitivity = 1.0f;
	camera->zoom(sensitivity * delta);
}

}