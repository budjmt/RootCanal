#include "Camera.h"

Camera::Camera() {}
Camera::~Camera() {}

void Camera::updateCamMat(ISimpleShader* shader) {
	//we pre-transpose projection because it doesn't change very often
	shader->SetMatrix4x4("projection",	&projection[0][0]);
	shader->SetMatrix4x4("view",		&view[0][0]);

	/*vec3 l = getLookAt(), u = getUp();
	DirectX::XMVECTOR eye = { transform.position.x, transform.position.y, transform.position.z };
	DirectX::XMVECTOR target = { l.x, l.y, l.z };
	DirectX::XMVECTOR up = { u.x, u.y, u.z };
	DirectX::XMMATRIX x = DirectX::XMMatrixLookAtLH(eye, target, up);
	DirectX::XMFLOAT4X4 v;
	XMStoreFloat4x4(&v, XMMatrixTranspose(x));
	shader->SetMatrix4x4("view", v);*/
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
	if (mouse->down) {
		if (mouse->btnState & 0x0001) {
			float rot = (float)(PI * 20 * dt);
			float xDiff = (float)(mouse->curr.x - mouse->prev.x);
			float dx = sign(xDiff) * xDiff * xDiff / width * rot;
			dx = min(rot, dx);
			float yDiff = (float)(mouse->curr.y - mouse->prev.y);
			float dy = sign(yDiff) * yDiff * yDiff / height * rot;
			dy = min(rot, dy);
			vec3 look = camera->getLookAt();
			camera->turn(dx, dy);
			camera->transform.position = look - camera->getForward();
		}
		else if (mouse->btnState & 0x0002) {
			float avg = (float)((mouse->curr.y - mouse->prev.y) + (mouse->curr.x - mouse->prev.x)) * 8 * dt;
			camera->transform.position += camera->getForward() * avg;
		}
		else if (mouse->btnState & 0x0010) {
			camera->transform.position += camera->getRight() * ((float)(mouse->curr.x - mouse->prev.x) * dt * 8);
			camera->transform.position += camera->getUp() * ((float)(mouse->curr.y - mouse->prev.y) * dt * 8);
		}
		//std::cout << "Position: " << camera->transform.position.x << "," << camera->transform.position.y << "," << camera->transform.position.z << std::endl << "Pitch: " << camera->pitch << std::endl << "Yaw: " << camera->yaw << std::endl;
	}

	if (GetAsyncKeyState('W') & 0x8000) {
		camera->transform.position += camera->getForward() * (5.f * (float)dt);
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		camera->transform.position += camera->getForward() * (-5.f * (float)dt);
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		camera->transform.position += camera->getRight() * (-5.f * (float)dt);
	}
	else if (GetAsyncKeyState('A') & 0x8000) {
		camera->transform.position += camera->getRight() * (5.f * (float)dt);
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState(VK_SPACE) & 0x8000) {
		camera->transform.position += vec3(0, 1, 0) * (5.f * (float)dt);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('X') & 0x8000) {
		camera->transform.position += vec3(0, 1, 0) * (-5.f * (float)dt);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		camera->transform.position += vec3(1, 0, 0) * (-5.f * (float)dt);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		camera->transform.position += vec3(1, 0, 0) * (5.f * (float)dt);
	}
}