#include "Drawable.h"

Drawable::Drawable() { }
Drawable::~Drawable() { ReleaseMacro(vertexBuffer);	ReleaseMacro(indexBuffer); }

void Drawable::vertexShader(SimpleVertexShader* s) { _vertexShader = s; } void Drawable::pixelShader(SimplePixelShader* s) { _pixelShader = s; }

void Drawable::draw(float x, float y, float z, float xScale, float yScale, float zScale, ID3D11DeviceContext* deviceContext) {
	vec3 pos = vec3(x, y, z);
	vec3 scale = vec3(xScale, yScale, zScale);
	vec3 rotAxis = vec3(0, 0, 1);
	float rot = 0;
	setWorldMatrix(pos, scale, rotAxis, rot);
	//actual draw call is reserved for children
}

void Drawable::draw(Transform* t, ID3D11DeviceContext* deviceContext) { draw(t->position, t->scale, t->rotAxis, t->rotAngle, deviceContext); }
void Drawable::draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext) {
	setWorldMatrix(pos, scale, rotAxis, rot);
}

#include <DirectXMath.h>
void Drawable::setWorldMatrix(vec3 pos, vec3 scaleV, vec3 rotAxis, float rot) {
	mat4 translate = mat4::translate(pos);
	mat4 scale = mat4::scale(scaleV);
	mat4 rotate = mat4::rotate(rot, rotAxis);
	DirectX::XMMATRIX w = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(w));
	//_vertexShader->SetMatrix4x4("world", &mat4::transpose(translate * rotate * scale)[0][0]);
	//_vertexShader->SetMatrix4x4("world", world);
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