// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
    float4 position		: SV_POSITION;
    float2 uv           : TEXCOORD0;
};

cbuffer Data : register( c0 ) {
    float thresholdMin;
    float thresholdMax;
    float pixelWidth;
    float pixelHeight;
    float outlineWidth;
}

// Textures and such
Texture2D pixels		: register( t0 );
SamplerState trilinear	: register( s0 );

float Threshold( float val ) {
    if( val < thresholdMin )
        return 0;
    return 1;
}

float pixelIntensity( float4 inColor ) {
    //simple average of rgb, may change later
    return pow( ( inColor.r + inColor.g + inColor.b ) / 3, 1 );
}

float edgeDetect( float2 uv ) {
    //how far in the uv is one pixel?
    if( pixelIntensity( pixels.SampleLevel( trilinear, uv, 0 ) ) < 0.1f ) {
        return 0.f;
    }

    float dx = pixelWidth;
    float dy = pixelHeight;

    float pixel[9];
    int k = -1;
    float delta;

    //get neighbor monochrome intensities
    for( int i = -1; i < 2; i++ ) {
        for( int j = -1; j < 2; j++ ) {
            k++;
            float2 tempUV = uv + float2( float( i ) * dx * outlineWidth, float( j ) * dy * outlineWidth );
            tempUV.x = saturate( tempUV.x );
            tempUV.y = saturate( tempUV.y );
            pixel[k] = pixelIntensity( pixels.SampleLevel( trilinear, tempUV, 0 ) );
        }
    }

    delta = (
        abs( pixel[1] - pixel[7] ) +
        abs( pixel[5] - pixel[3] ) +
        abs( pixel[0] - pixel[8] ) +
        abs( pixel[2] - pixel[6] )
        ) / 4;

    //delta = 0.5f;

    return Threshold( saturate( 1.8*delta ) );
}

//Code inspired by
//http://coding-experiments.blogspot.com/2010/06/edge-detection.html
float4 main( VertexToPixel input ) : SV_TARGET
{
    float4 color = float4( 0,0,0,1 );
    color.xyz = 1 - edgeDetect( input.uv ).xxx;
    return color * pixels.SampleLevel( trilinear, input.uv, 0 );
}