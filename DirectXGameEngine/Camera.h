#pragma once

#include <DirectXMath.h>
#include "Keyboard.h"

class Camera
{
public:
    Camera();
    ~Camera();

    void Update( float dt );

    void Rotate( float dx, float dy );

    DirectX::XMFLOAT4X4 GetViewMatrix();
    DirectX::XMFLOAT4X4 GetProjectionMatrix();
    void SetProjectionAspectRatio( float aspectRatio );

private:
    DirectX::XMFLOAT4X4 _viewMatrix;
    DirectX::XMFLOAT4X4 _projectionMatrix;

    DirectX::XMFLOAT3 _position;
    DirectX::XMFLOAT3 _forward;
    float _xRotation;
    float _yRotation;
};

