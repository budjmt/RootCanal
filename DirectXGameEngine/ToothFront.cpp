#include "ToothFront.h"

ToothFront::ToothFront(Mesh* mesh, Material* material, Camera** cam) : GameObject(new DrawMesh(mesh,nullptr,DXInfo::getInstance().device))
{
	camera = cam;
	opacityRadius = new OpacityRadius((*cam)->windowWidth, (*cam)->windowHeight,material->texture()->samplerState,cam);

	material->texture()->addTex(opacityRadius->resourceView);
	_shape->material(material);

}


ToothFront::~ToothFront()
{
	delete opacityRadius;
}


void ToothFront::draw(ID3D11DeviceContext* deviceContext) {
	vec3 shipScreenPos = vec4(ship->transform.getComputed().position(),1) * (*camera)->getCamMat();
	opacityRadius->draw(shipScreenPos, _shape->material()->texture()->samplerState);
	GameObject::draw(deviceContext);
	_shape->material()->unbindSRV();
}