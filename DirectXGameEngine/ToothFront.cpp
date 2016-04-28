#include "ToothFront.h"

ToothFront::ToothFront(Mesh* mesh, Material* material, Camera** cam) : GameObject(new DrawMesh(mesh,nullptr,DXInfo::getInstance().device))
{
	_shape->material(material);
	camera = cam;
	opacityRadius = new OpacityRadius((*cam)->windowWidth, (*cam)->windowHeight);

	opacity = new DrawMesh(mesh, nullptr, DXInfo::getInstance().device);
	Texture* t = new Texture;
	t->addTex(opacityRadius->resourceView);
	material->texture()->addTex(opacityRadius->resourceView);
	opacity->material(Material::createMaterial(L"opacityMat", t, Shader::getShader<SimpleVertexShader>(L"BasicVertex"), Shader::getShader<SimplePixelShader>(L"OpacityMap"), cam));
}


ToothFront::~ToothFront()
{
	delete opacity;
	delete opacityRadius;
}


void ToothFront::draw(ID3D11DeviceContext* deviceContext) {
	vec3 shipScreenPos = vec4(ship->transform.getComputed().position(),1) * (*camera)->getCamMat();
	opacityRadius->draw(shipScreenPos, opacity->material()->texture()->samplerState);
	opacity->draw(&transform, deviceContext);
	GameObject::draw(deviceContext);
}