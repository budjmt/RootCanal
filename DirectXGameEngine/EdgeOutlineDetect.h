#pragma once
#include "PostProcessBase.h"

class EdgeOutlineDetect : public PostProcessBase
{
public:
    EdgeOutlineDetect( UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext );
    virtual SRV* draw( SRV* ppSRV );
    void setBlur( bool _blur );
    void setSamples( int _samples );
    void setOutlineWidth( float _width );
    ~EdgeOutlineDetect();
private:
    // Post process stuff
    RTV* edgeRTV;
    SRV* edgeSRV;

    RTV* blurRTV;
    SRV* blurSRV;

    bool blur;
    int blurSamples;
    float outlineWidth;

    SimplePixelShader* edgePS;
    SimplePixelShader* blurPS;
};

