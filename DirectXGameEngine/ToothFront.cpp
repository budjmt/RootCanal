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

}


ToothFront::~ToothFront()
{
	//delete opacityRadius; 
	delete compute;
}


void ToothFront::draw(ID3D11DeviceContext* deviceContext) {

	vec3 scale = transform.getComputed().scale();
	cornerPos = transform.getComputed().position() - scale/2;

	//vec3 shipScreenPos = vec4(ship->transform.getComputed().position(),1) * (*camera)->getCamMat();
	//opacityRadius->draw(shipScreenPos, _shape->material()->texture()->samplerState);

	vec3 shipPos = ship->transform.getComputed().position();
	vec3 relativePos = shipPos - cornerPos;
	relativePos.x /= scale.x;
	relativePos.y /= scale.y;

	compute->dispatch(relativePos);
	GameObject::draw(deviceContext);
	_shape->material()->unbindSRV();
}