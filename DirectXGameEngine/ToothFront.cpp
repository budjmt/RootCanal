#include "ToothFront.h"

ToothFront::ToothFront(Mesh* mesh, Material* material, Camera** cam) : ColliderObject(mesh, material)
{
	rigidBody().solid(0);
	
	opacityRadius = OpacityRadius((*cam)->windowWidth, (*cam)->windowHeight);

	opacity = new DrawMesh(mesh, nullptr, DXInfo::getInstance().device);
	Texture* t = new Texture;
	t->addTex(opacityRadius.resourceView);
	material->texture()->addTex(opacityRadius.resourceView);
	opacity->material(Material::createMaterial(L"opacityMat", t, Shader::getShader<SimpleVertexShader>(L"BasicVertex"), Shader::getShader<SimplePixelShader>(L"OpacityMap"), cam));
}


ToothFront::~ToothFront()
{
	delete ((DrawMesh*)opacity)->material()->texture();
	delete opacity;
}


void ToothFront::draw(ID3D11DeviceContext* deviceContext) {
	opacityRadius.draw(ship->transform.position(), opacity->material()->texture()->samplerState);
	opacity->draw(&transform, deviceContext);
}