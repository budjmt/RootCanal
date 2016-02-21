#include "Camera.h"

Camera::Camera() {}
Camera::~Camera() {}

void Camera::updateCamMat(ISimpleShader* shader) {
	//we pre-transpose projection because it doesn't change very often
	shader->SetMatrix4x4("projection", &projection[0][0]);
	shader->SetMatrix4x4("view", &view[0][0]);
}

void Camera::update(float dt, Mouse* mouse) {
	view = mat4::lookAt(transform.position, getLookAt(), getUp());
}

void Camera::draw(ID3D11DeviceContext* deviceContext) {
	//does NOTHING because it's a CAMERA
	//or maybe there's debug here
	//who knows
}

void Camera::turn(float dx, float dy) {
	transform.rotate(dy, dx, 0);
}

vec3 Camera::getLookAt() {
	return transform.position + getForward();
}

void Camera::updateProjection(int width, int height, float aspect) {
	float znear = 0.01f;
	float zfar = 1000.f;
	//transpose for direct x
	//projection = mat4::perspective((float)width, (float)height, znear, zfar);
	projection = mat4::perspectiveFOV(CAM_FOV, aspect, znear, zfar);
	//projection = mat4::transpose(mat4::orthographic((float)width, (float)height, znear, zfar));
	//DirectX::XMMATRIX x = DirectX::XMMatrixPerspectiveFovLH(CAM_FOV, aspect, znear, zfar);
}

vec3 Camera::getForward() { return transform.forward(); }
vec3 Camera::getUp() { return transform.up(); }
vec3 Camera::getRight() { return transform.right(); }

void Camera::mayaCam(int width, int height, float dt, Mouse* mouse, Camera* camera) {
	//normally there is a Mouse struct to contain mouse info, I'll deal with this later
	if (mouse->down) {
		if (mouse->btnState & 0x0001) {
			float rot = (float)(PI / 2 / dt);
			float xDiff = (float)(mouse->curr.x - mouse->prev.x);
			float dx = sign(xDiff) * xDiff * xDiff / width * rot;
			dx = min(PI / 2, dx);
			float yDiff = (float)(mouse->curr.y - mouse->prev.y);
			float dy = sign(yDiff) * yDiff * yDiff / height * rot;
			dy = min(PI / 2, dy);
			vec3 look = camera->getLookAt();
			camera->turn(dx, dy);
			camera->transform.position = look - camera->getForward();
		}
		else if (mouse->btnState & 0x0002) {
			float avg = (float)((mouse->curr.y - mouse->prev.y) + (mouse->curr.x - mouse->prev.x)) / 2;
			camera->transform.position += camera->getForward() * avg;
		}
		else if (mouse->btnState & 0x0010) {
			camera->transform.position += camera->getRight() * (float)(mouse->curr.x - mouse->prev.x);
			camera->transform.position += camera->getUp() * (float)(mouse->curr.y - mouse->prev.y);
		}
		//std::cout << "Position: " << camera->transform.position.x << "," << camera->transform.position.y << "," << camera->transform.position.z << std::endl << "Pitch: " << camera->pitch << std::endl << "Yaw: " << camera->yaw << std::endl;
	}

	if (GetAsyncKeyState('W') & 0x8000) {
		camera->transform.position += camera->getForward() * 5.f * (float)dt;
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		camera->transform.position += camera->getForward() * -5.f * (float)dt;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		camera->transform.position += camera->getRight() * 5.f * (float)dt;
	}
	else if (GetAsyncKeyState('A') & 0x8000) {
		camera->transform.position += camera->getRight() * -5.f * (float)dt;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		camera->transform.position += vec3(0, 1, 0) * 5.f * (float)dt;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		camera->transform.position += vec3(0, 1, 0) * -5.f * (float)dt;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		camera->transform.position += vec3(1, 0, 0) * 5.f * (float)dt;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		camera->transform.position += vec3(1, 0, 0) * -5.f * (float)dt;
	}
}