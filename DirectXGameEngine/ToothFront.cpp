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

	cornerPos = transform.getComputed().position()-((DrawMesh*)_shape)->mesh()->getDims();
}


ToothFront::~ToothFront()
{
	delete opacityRadius;
}


void ToothFront::draw(ID3D11DeviceContext* deviceContext) {
	vec3 shipScreenPos = vec4(ship->transform.getComputed().position(),1) * (*camera)->getCamMat();
	//opacityRadius->draw(shipScreenPos, _shape->material()->texture()->samplerState);

	vec3 relativePos = cornerPos - ship->transform.getComputed().position();
	relativePos = relativePos / vec3::length(((DrawMesh*)_shape)->mesh()->getDims()) * 2;

	compute->dispatch({ 0,0,0 });
	GameObject::draw(deviceContext);
	_shape->material()->unbindSRV();
}