#include "Material.h"

Material::Material() {}
Material::~Material() {}

void Material::vertexShader(SimpleVertexShader* v) { vertex = v; }
void Material::pixelShader(SimplePixelShader* p) { pixel = p; }
void Material::camera(Camera** c) { _camera = c; }

void Material::updateMaterial(mat4& world) {
	//vertex->SetMatrix4x4("world", &world[0][0]);
	vertex->SetMatrix4x4("inv_trans_world", &mat4::inv_tp_tf(world)[0][0]);
	(*_camera)->updateCamMat(vertex);
}

void Material::setActive(bool b) {
	vertex->SetShader(b);
	pixel->SetShader(b);
}