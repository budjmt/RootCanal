#pragma once

#include <string>
#include <vector>
#include <map>

#include "MarchMath.h"
#include "Transform.h"
#include "Mesh.h"

#include "DrawDebug.h"

class Collider;

enum class ColliderType {
    SPHERE,
    BOX,
    MESH
};

class AABB {
public:
	vec3 center = vec3(), halfDims = vec3();
	bool intersects(const AABB& other);
};

struct Adj {
    int f1, f2;
    int edge[2];
};

struct GaussMap {
    //std::vector<vec3> normals;
    std::map<std::string, std::vector<Adj>> adjacencies;//keys are untransformed normals, adjs use indices because of rotations
    void addAdj( vec3 v, Adj a );
    std::vector<Adj>& getAdjs( vec3 v );
};

struct SupportPoint {
    vec3 point;
    float proj;
};

struct Manifold {
    Collider* originator = nullptr, *other = nullptr;
    std::vector<vec3> colPoints;
    float pen = -FLT_MAX;
    vec3 axis;
};

struct FaceManifold : Manifold {
    int norm;
};

struct EdgeManifold : Manifold {
    Adj edgePair[2];
};

class Collider
{
public:
    Collider( void );
    Collider( Transform* t, vec3 b );
    Collider( Mesh* m, Transform* t );
    Collider( const Collider& other );
    ~Collider( void );
    ColliderType& type;
    Transform* transform() const;
    vec3 framePos() const;
    vec3 dims() const; void dims( vec3 v );
	AABB& aabb();
    void updateDims( Transform* t );
    float radius() const;

    Manifold intersects( Collider* other );

    std::vector<int> getIncidentFaces( vec3 refNormal );
    void clipPolygons( FaceManifold& reference, std::vector<int>& incidents );
    std::vector<vec3> clipPolyAgainstEdge( std::vector<vec3>& input, vec3 sidePlane, vec3 sideVert, vec3 refNorm, vec3 refCenter );
    vec3 closestPointBtwnSegments( vec3 p0, vec3 p1, vec3 q0, vec3 q1 );

    void update();

    //void setCorners(std::vector<vec3> c);
    void genVerts();//only used for box colliders right now
    void genNormals();
    void genEdges();
    void genGaussMap();

    const std::vector<vec3>& getCurrNormals() const;
    const std::vector<vec3>& getEdges() const;

    int getFaceVert( int index ) const;
    vec3 getVert( int index ) const;
    vec3 getNormal( int index ) const;
    vec3 getEdge( int( &e )[2] );

    const GaussMap& getGaussMap() const;

    void updateNormals();
    void updateEdges();

    bool fuzzyParallel( vec3 v1, vec3 v2 );

    SupportPoint getSupportPoint( vec3 dir );
    FaceManifold getAxisMinPen( Collider* other );
    EdgeManifold overlayGaussMaps( Collider* other );

private:
    Transform* _transform;
    vec3 _framePos;
	vec3 _dims; AABB _aabb, true_aabb;
    float _radius;
    ColliderType _type;

    std::vector<vec3> faceNormals, currNormals, edges, currEdges;//these are vec3s to avoid constant typecasting, and b/c cross product doesn't work for 4d vectors
    std::map<std::string, int> edgeMap;//maps the edge pairs to the indices in edges
    GaussMap gauss;
    //std::vector<std::vector<Adj>> adjacencies;
    Mesh* mesh;

    void setEdge( int( &e )[2], int index );
};