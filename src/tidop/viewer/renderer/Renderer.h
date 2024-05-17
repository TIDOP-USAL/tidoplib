#pragma once

#include <iostream>
#include <vector>

#include <tidop/viewer/opengl/shader/Shader.h>

#include <tidop/viewer/renderer/TrackballCamera.h>

#include "tidop/viewer/group/ModelBase.h"

namespace tl
{

class Renderer
{
	GENERATE_SHARED_PTR(Renderer)

private:

	ShaderProgram::Ptr shaderProgram;

	TrackballCamera::Ptr camera;

	std::vector<ModelBase::Ptr> models;

	unsigned int viewportWidth, viewportHeight;

	Vector2i previousMouse;

public:

	Renderer() = default;
	Renderer(unsigned int _viewportWidth, unsigned int _viewportHeight);
	~Renderer() = default;

public:

	void init();
	virtual void resize(int w, int h);
	void render();

	void pan(int mouseX, int mouseY);
	void rotate(int mouseX, int mouseY);
	void zoom(int delta);

public:

	void addModel(const ModelBase::Ptr& model) { models.push_back(model); }
	std::vector<ModelBase::Ptr> getModels() const { return models; }

	void setPreviousMouse(const Vector2i& previousMouse) { this->previousMouse = previousMouse; }
	Vector2i getPreviousMouse() { return previousMouse; }

	Camera::Ptr getCamera() { return std::dynamic_pointer_cast<Camera>(camera); }
};

}