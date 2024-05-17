#pragma once

#include <iostream>
#include <functional>

#include "RayCaster.h"

#include "tidop/viewer/group/ModelBase.h"
#include "tidop/viewer/group/PointCloud.h"


namespace tl
{

class Picker : public RayCaster
{
	GENERATE_SHARED_PTR(Picker)

public:

	struct Result
	{
		ModelBase::Ptr modelBase;
		Vector3f point;
		bool intersects;
	};

	using Listener = std::function<void(const Result& result)>;

private:

	Listener listener;
	float radius;

public:

	Picker(const Camera::Ptr& camera, unsigned int width, unsigned int height, float _radius)
		: RayCaster(camera, width, height), radius(_radius) {
	}

	Picker() = default;
	~Picker() = default;

public:

	void setListener(const Listener& listener)
	{
		this->listener = listener;
	}

	void setCamera(const Camera::Ptr& camera)
	{
		this->camera = camera;
	}

	void pick(const std::vector<ModelBase::Ptr>& models, const Vector2i& mousePosition)
	{
		Vector3f point{ 0.0f, 0.0f, 0.0f };
		bool intersects = false;

		RayCaster raycaster(camera, width, height);
		Line ray = raycaster.ray(mousePosition);

		float cameraDistance = std::numeric_limits<float>::max();

		ModelBase::Ptr modelBase = nullptr;

		for(auto& model : models)
		{
			if(instanceof<PointCloud>(model.get()))
			{
				PointCloud::Ptr pointcloud = std::dynamic_pointer_cast<PointCloud>(model);

				for(auto& vertex : pointcloud->getPoints())
				{
					float distance = ray.distance(vertex.position);

					if(distance <= radius)
					{
						Vector3f eye = camera->getEye();
						Vector3f pointToEye = vertex.position - eye;

						//if(pointToEye.dotProduct(ray.getDirection()) > 0.0)
						//{
							float _cameraDistance = pointToEye.module();
							if(_cameraDistance < cameraDistance)
							{
								cameraDistance = _cameraDistance;
								point = vertex.position;
								modelBase = model;
							}
							intersects = true;
						//}
					}
				}
			}
		}

		Result result;
		result.modelBase = modelBase;
		result.point = point;
		result.intersects = intersects;

		listener(result);
	}
};

}
