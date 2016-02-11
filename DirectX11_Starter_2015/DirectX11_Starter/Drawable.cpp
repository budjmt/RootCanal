#include "Drawable.h"

Drawable::Drawable() : shader(_shader), colorLoc(_colorLoc) { }
Drawable::Drawable(const Drawable& other) : shader(_shader), colorLoc(_colorLoc) { shaderProg = other.shaderProg; colorLoc = other.colorLoc; }
Drawable& Drawable::operator=(const Drawable& other) { shaderProg = other.shaderProg; colorLoc = other.colorLoc; }
Drawable::~Drawable() { glDeleteVertexArrays(1, &vArray); glDeleteBuffers(1, &vBuffer); }

void Drawable::draw(GLfloat x, GLfloat y, GLfloat xScale, GLfloat yScale) {
	vec3 pos = vec3(x, y, 0);
	vec3 scale = vec3(xScale, yScale, 1);
	vec3 rotAxis = vec3(0, 0, 1);
	float rot = 0;
	setWorldMatrix(pos, scale, rotAxis, rot);

	//now the stuff done in init happens automatically since they were done
	//while it was active
	glBindVertexArray(vArray);

	//actual draw call is reserved for children
}

void Drawable::draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot) {
	setWorldMatrix(pos, scale, rotAxis, rot);

	//now the stuff done in init happens automatically since they were done
	//while it was active
	glBindVertexArray(vArray);
}

void Drawable::draw(Transform t) {
	draw(t.position, t.scale, t.rotAxis, t.rotAngle);
}

void Drawable::setWorldMatrix(vec3 pos, vec3 scaleV, vec3 rotAxis, float rot) {
	mat4 translate = mat4::translate(pos);
	mat4 scale = mat4::scale(scaleV);
	mat4 rotate = mat4::rotate(rot, rotAxis);
	glUniformMatrix4fv(worldMatrix, 1, GL_FALSE, &(translate * scale * rotate)[0][0]);
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