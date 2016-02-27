#include "Drawable.h"

Drawable::Drawable() { }
Drawable::~Drawable() { ReleaseMacro(vertexBuffer);	ReleaseMacro(indexBuffer); }

Material* Drawable::material() const { return _material; } void Drawable::material(Material* m) { _material = m; }

void Drawable::draw(float x, float y, float z, float xScale, float yScale, float zScale, ID3D11DeviceContext* deviceContext) {
	vec3 pos = vec3(x, y, z);
	vec3 scale = vec3(xScale, yScale, zScale);
	vec3 rotAxis = vec3(0, 0, 1);
	float rot = 0;
	mat4 world = genWorldMatrix(pos, scale, rotAxis, rot);
	_material->updateMaterial(world);
	//actual draw call is reserved for children
}

//#include "DirectXGameCore.h"
//using namespace DirectX;
void Drawable::draw(Transform* t, ID3D11DeviceContext* deviceContext) { draw(t->position, t->scale, t->rotAxis, t->rotAngle, deviceContext); }
void Drawable::draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext) {
	mat4 world = genWorldMatrix(pos, scale, rotAxis, rot);
	/*XMFLOAT3 af = XMFLOAT3(rotAxis.x, rotAxis.y, rotAxis.z);
	XMVECTOR a = XMLoadFloat3(&af);
	XMMATRIX W, T = XMMatrixTranslation(pos.x, pos.y, pos.z), R = XMMatrixRotationAxis(a, -rot), S = XMMatrixScaling(scale.x, scale.y, scale.z);
	W = XMMatrixMultiply(S, R); W = XMMatrixMultiply(W, T);*/
	_material->updateMaterial(world);
}

mat4 Drawable::genWorldMatrix(vec3 pos, vec3 scaleV, vec3 rotAxis, float rot) {
	mat4 translate = mat4::translate(pos);
	mat4 scale = mat4::scale(scaleV);
	mat4 rotate = mat4::rotate(rot, rotAxis);
	return scale * (rotate * translate);
}

//std::map<const char*, uint32_t> Drawable::loadedTextures;

/*uint32_t Drawable::genTexture(const char* texFile) {
//check if the image was already loaded
if (loadedTextures.find(texFile) != loadedTextures.end()) {
return loadedTextures[texFile];
}
FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(texFile), texFile);
//we convert the 24bit bitmap to 32bits
FIBITMAP* image = FreeImage_ConvertTo32Bits(bitmap);
//delete the 24bit bitmap from memory
FreeImage_Unload(bitmap);
int w = FreeImage_GetWidth(image);
int h = FreeImage_GetHeight(image);
GLubyte* textureData = FreeImage_GetBits(image);

GLuint texture = 0;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
//the texture is loaded in BGRA format
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)textureData);
FreeImage_Unload(image);

loadedTextures[texFile] = texture;
return texture;
}*/