#include "Camera.h"

void Camera::updateCamMat(ISimpleShader* shader) {
	//we pre-transpose projection because it doesn't change very often
	shader->SetMatrix4x4("projection", &projection[0][0]);
	shader->SetMatrix4x4("view", &view[0][0]);

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
	view = mat4::lookAt(transform.position(), getLookAt(), getUp());
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
	return transform.position() + getForward();
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
			float dx = signf(xDiff) * xDiff * xDiff / width * rot;
			dx = min(rot, dx);
			float yDiff = (float)(mouse->curr.y - mouse->prev.y);
			float dy = signf(yDiff) * yDiff * yDiff / height * rot;
			dy = min(rot, dy);
			vec3 look = camera->getLookAt();
			camera->turn(dx, dy);
			camera->transform.position( look - camera->getForward() );
		}
		else if (mouse->btnState & 0x0002) {
			float avg = (float)((mouse->curr.y - mouse->prev.y) + (mouse->curr.x - mouse->prev.x)) * 8 * dt;
			vec3 newPos = camera->transform.position() + camera->getForward() * avg;
			camera->transform.position(newPos);
		}
		else if (mouse->btnState & 0x0010) {
			vec3 newPos = camera->transform.position();
			newPos += camera->getRight() * ((float)(mouse->curr.x - mouse->prev.x) * dt * 8);
			newPos += camera->getUp() * ((float)(mouse->curr.y - mouse->prev.y) * dt * 8);
			camera->transform.position(newPos);
		}
		//std::cout << "Position: " << camera->transform.position.x << "," << camera->transform.position.y << "," << camera->transform.position.z << std::endl << "Pitch: " << camera->pitch << std::endl << "Yaw: " << camera->yaw << std::endl;
	}

    Keyboard& keys = Keyboard::getInstance();

	if ( keys.isDown( 'W' ) ) {
		vec3 newPos = camera->transform.position() + camera->getForward() * (5.f * (float)dt);
		camera->transform.position(newPos);
	}
	else if ( keys.isDown( 'S' ) ) {
		vec3 newPos = camera->transform.position() + camera->getForward() * (-5.f * (float)dt);
		camera->transform.position(newPos);
	}
	if ( keys.isDown( 'D' ) ) {
		vec3 newPos = camera->transform.position() + camera->getRight() * (-5.f * (float)dt);
		camera->transform.position(newPos);
	}
	else if ( keys.isDown( 'A' ) ) {
		vec3 newPos = camera->transform.position() + camera->getRight() * (5.f * (float)dt);
		camera->transform.position(newPos);
	}

	if ( keys.isDown( VK_UP ) || keys.isDown( VK_SPACE ) ) {
		vec3 newPos = camera->transform.position() + vec3(0, 1, 0) * (5.f * (float)dt);
		camera->transform.position(newPos);
	}
	else if ( keys.isDown( VK_DOWN ) || keys.isDown( 'X' ) ) {
		vec3 newPos = camera->transform.position() + vec3(0, 1, 0) * (-5.f * (float)dt);
		camera->transform.position(newPos);
	}
	if ( keys.isDown( VK_RIGHT ) ) {
		vec3 newPos = camera->transform.position() + vec3(1, 0, 0) * (-5.f * (float)dt);
		camera->transform.position(newPos);
	}
	else if ( keys.isDown( VK_LEFT ) ) {
		vec3 newPos = camera->transform.position() + vec3(1, 0, 0) * (5.f * (float)dt);
		camera->transform.position(newPos);
	}
}