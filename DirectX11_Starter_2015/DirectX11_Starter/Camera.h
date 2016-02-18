#pragma once

#include "DirectXGameCore.h"

#include "SimpleShader.h"

#include "MarchMath.h"
#include "Entity.h"
//#include "Mouse.h"

const float CAM_FOV = 2 * PI / 5;

class Camera : public Entity {
public:
	Camera();
	~Camera();
	void updateCamMat(ISimpleShader* shader);
	void update(float dt);
	void draw(ID3D11DeviceContext* deviceContext);

	float zoom;
	vec3 getForward();
	vec3 getUp();
	vec3 getRight();
	void turn(float dx, float dy);
	vec3 getLookAt();
	void updateProjection(int width, int height, float aspect);

	static void mayaCam(int width, int height, double dt, Camera* camera);

private:
	mat4 projection, view;
};
