#pragma once

#include <vector>

#include "Game.h"
#include "SimpleShader.h"

#include "MarchMath.h"
#include "DXInfo.h"
#include "Drawable.h"
#include "Camera.h"
#include "MeshImporter.h"
#include "Mesh.h"

#define DEBUG false

const int MAX_VECTORS = 1000000;
const int MAX_SPHERES = 100000;
const int MAX_BOXES = 500000;

struct DebugMeshBuffer { std::vector<DebugVertex> meshArray = std::vector<DebugVertex>(); std::vector<uint32_t> meshElementArray = std::vector<uint32_t>(); };

struct Sphere { vec3 center; float rad; };

class DrawDebug
{
public:
    static DrawDebug& getInstance();
    void camera( Camera** c );

    //this is the actual draw call
    void draw();

    //these are called externally for drawing stuff
    void drawDebugVector( vec3 start, vec3 end, vec3 color = vec3( 0.7f, 1, 0 ) );
    void drawDebugSphere( vec3 pos, float rad );
	void drawDebugBox(vec3 pos, vec3 dims, vec3 color = vec3(1, 0, 0), bool filled = false);
private:
    DrawDebug();
    ~DrawDebug();
    DrawDebug( const DrawDebug& ) = delete;
    void operator=( const DrawDebug& ) = delete;

    void shaderSetup( DXInfo& d );
    void bufferSetup( DXInfo& d );
    DebugMeshBuffer genDebugMeshArrays( Mesh* m );

    //these are to separate the individual processes
    void drawVectors();
    void drawSpheres();
	void drawBoxes();

    ID3D11RasterizerState *fill, *wireframe;

    Camera** cam = nullptr;

    Mesh *sphere, *box, *arrow;
    ID3D11Buffer *svb, *sib, *sinstb
			   , *bvb, *bib, *binstb
			   , *avb, *aib, *ainstb
			   , *vvb;
    int sphereVerts;

    SimpleVertexShader *vecVert, *meshVert;
    SimplePixelShader *vecPixel, *meshPixel;

    std::vector<vec3> debugVectors, debugBoxes;
    std::vector<Sphere> debugSpheres;

    std::vector<DebugVector> vecBufferData;
    std::vector<DebugMesh> arrowBufferData, sphereBufferData, boxBufferData;
};