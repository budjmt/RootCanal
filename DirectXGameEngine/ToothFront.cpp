#include "ToothFront.h"

ToothFront::ToothFront(Mesh* mesh, Material* material, Camera** cam, ID3D11Device* _device) : GameObject(new DrawMesh(mesh,nullptr,DXInfo::getInstance().device))
{
	camera = cam;
	//opacityRadius = new OpacityRadius((*cam)->windowWidth, (*cam)->windowHeight,material->texture()->samplerState,cam);

	compute = new OpacityCompute(_device);

	//material->texture()->addTex(opacityRadius->resourceView);
	material->texture()->addTex(compute->getSRV());
	material->texture()->addTex(compute->getSRV());
	_shape->material(material);

	vec3 dims = ((DrawMesh*)_shape)->mesh()->getDims();
	vec3 scale = transform.getComputed().scale();
	dims.x *= scale.x;
	dims.y *= scale.y;
	dims.z *= scale.z;

	cornerPos = transform.getComputed().position() - dims;
}


ToothFront::~ToothFront()
{
	delete opacityRadius;
}


void ToothFront::draw(ID3D11DeviceContext* deviceContext) {
	vec3 shipScreenPos = vec4(ship->transform.getComputed().position(),1) * (*camera)->getCamMat();
	//opacityRadius->draw(shipScreenPos, _shape->material()->texture()->samplerState);

	vec3 shipPos = ship->transform.getComputed().position();
	vec3 relativePos = cornerPos - shipPos;
	relativePos = -relativePos / vec3::length(((DrawMesh*)_shape)->mesh()->getDims()) * 2;

	compute->dispatch(relativePos);
	GameObject::draw(deviceContext);
	_shape->material()->unbindSRV();
}