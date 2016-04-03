#include "Camera.h"

Camera::Camera()
{
    _position = DirectX::XMFLOAT3( 0, 0, -5 );
    _forward = DirectX::XMFLOAT3( 0, 0, 1 );
    _viewMatrix = DirectX::XMFLOAT4X4();
    _projectionMatrix = DirectX::XMFLOAT4X4();

    _xRotation = 0;
    _yRotation = 0;
}

Camera::~Camera()
{
}

void Camera::Update( float dt )
{
    // Load data for vectors to perform camera update calculations
    DirectX::XMVECTOR rotationQuaternion = DirectX::XMQuaternionRotationRollPitchYaw( _xRotation, _yRotation, 0 );
    DirectX::XMVECTOR forward = DirectX::XMLoadFloat3( &_forward );
    DirectX::XMVECTOR position = DirectX::XMLoadFloat3( &_position );
    DirectX::XMVECTOR up = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0, 1, 0 ) );
    DirectX::XMVECTOR right = DirectX::XMVector3Cross( up, forward );
    DirectX::XMVECTOR displacement = DirectX::XMLoadFloat3( &DirectX::XMFLOAT3( 0, 0, 0 ) );

    // Calculate the new view direction and right direction
    DirectX::XMVECTOR viewDirection = DirectX::XMVector3Rotate( forward, rotationQuaternion );
    DirectX::XMVECTOR newRightDirection = DirectX::XMVector3Rotate( right, rotationQuaternion );

    // Handle input
    Keyboard& input = Keyboard::getInstance();
    
    if( input.isDown( 'W' ) )
    {
        displacement = DirectX::XMVectorAdd( displacement, viewDirection );
    }

    if( input.isDown( 'S' ) )
    {
        displacement = DirectX::XMVectorSubtract( displacement, viewDirection );
    }

    if( input.isDown( 'D' ) )
    {
        displacement = DirectX::XMVectorAdd( displacement, newRightDirection );
    }

    if( input.isDown( 'A' ) )
    {
        displacement = DirectX::XMVectorSubtract( displacement, newRightDirection );
    }

    if( input.isDown( ' ' ) )
    {
        displacement = DirectX::XMVectorAdd( displacement, up );
    }

    if( input.isDown( 'X' ) )
    {
        displacement = DirectX::XMVectorSubtract( displacement, up );
    }

    // Scale the displacement
    displacement = DirectX::XMVectorScale( displacement, 5 * dt );

    position = DirectX::XMVectorAdd( position, displacement );

    // Calculate the new view matrix
    DirectX::XMMATRIX newViewMatrix = DirectX::XMMatrixLookToLH( position, viewDirection, up );

    // Store the newly transformed data
    DirectX::XMStoreFloat4x4( &_viewMatrix, XMMatrixTranspose( newViewMatrix ) );
    DirectX::XMStoreFloat3( &_position, position );
}

void Camera::Rotate( float dx, float dy )
{
    _xRotation += dx;
    _yRotation += dy;

    float pi = DirectX::XM_PI;

    // Clamp the pitch value to be from -PI/2 to PI/2
    _xRotation = max( -pi * 0.5f, min( pi * 0.5f, _xRotation ) );

    // Wrap the yaw value to be from 0 to 2PI
    while( _yRotation > 2.0f * pi )
    {
        _yRotation -= 2.0f * pi;
    }
    while( _yRotation < 0 )
    {
        _yRotation += 2.0f * pi;
    }
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
    return _viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
    return _projectionMatrix;
}

void Camera::SetProjectionAspectRatio( float aspectRatio )
{
    // Update our projection matrix since the window size changed
    DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
        0.25f * 3.1415926535f,	// Field of View Angle
        aspectRatio,		  	// Aspect ratio
        0.1f,				  	// Near clip plane distance
        100.0f );			  	// Far clip plane distance
    DirectX::XMStoreFloat4x4( &_projectionMatrix, XMMatrixTranspose( P ) ); // Transpose for HLSL!
}
