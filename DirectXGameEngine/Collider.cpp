#include "Collider.h"
#include <iostream>

Collider::Collider( void )
    : type( _type )
{
    _dims = vec3();
	base_aabb.center = _transform->getComputed().position();
	transformed_aabb = base_aabb;
    _radius = 0;
}

Collider::Collider( Transform* t, vec3 d, bool fudge )
    : type( _type )
{
    _transform = t;
	dims(d);
	base_aabb.center = _transform->getComputed().position();
	transformed_aabb = base_aabb;
	fudgeAABB = fudge;
	updateDims(_transform);
    _type = ColliderType::BOX;
    //the order is important;
    //edges depend on the gauss map, which depends on the normals
    genNormals();
    genGaussMap();
    genEdges();
    update();
}

Collider::Collider( Mesh* m, Transform* t )
    : type( _type )
{
    mesh = m;
    _transform = t;
    dims( m->getDims() );
	base_aabb.center = _transform->getComputed().position();
	transformed_aabb = base_aabb;
    updateDims( _transform );
    _type = ColliderType::MESH;
    //the order is important;
    //edges depend on the gauss map, which depends on the normals
    genNormals();
    genGaussMap();
    genEdges();
    update();
}

Collider::Collider( const Collider& other )
    : type( _type )
{
    _transform = new Transform( *other.transform() );
    dims( other.dims() );
	fudgeAABB = other.fudgeAABB;
	base_aabb = other.base_aabb;
	transformed_aabb = other.transformed_aabb;
    _radius = other.radius();
    _framePos = other.framePos();
    type = other.type;
}

Collider::~Collider( void )
{
    if( _type == ColliderType::BOX )
        delete mesh;
}

Transform* Collider::transform() const { return _transform; }
vec3 Collider::framePos() const { return _framePos; }

vec3 Collider::dims() const { return _dims; } 
void Collider::dims(vec3 v) { 
	_dims = v; 
	_radius = maxf(maxf(_dims.x, _dims.y), _dims.z); 
	base_aabb.halfDims = v; 
}
AABB& Collider::aabb() { return transformed_aabb; }
float Collider::radius() const { return _radius; }

//makes sure the radius is up to date
void Collider::updateDims( Transform* t ) {
    vec3 scale = t->getComputed().scale();
    _radius = maxf( maxf( _dims.x * scale.x, _dims.y * scale.y ), _dims.z * scale.z );
	transformed_aabb.halfDims = base_aabb.halfDims;
	if (fudgeAABB) {
		for (int i = 0; i < 3; i++)
			transformed_aabb.halfDims[i] *= scale[i] * 1.f;//they're big right now
	}
}

void Collider::update() {
	_framePos = _transform->getComputed().position();
	base_aabb.center = _framePos;
	transformed_aabb.center = base_aabb.center;
	updateDims(_transform);
	updateNormals();
	updateEdges();
	DrawDebug::getInstance().drawDebugSphere(_framePos, _radius);
}

//gets the vertex of the collider furthest in the direction of dir
SupportPoint Collider::getSupportPoint( vec3 dir ) {
    auto verts = mesh->verts();
    Transform trans = _transform->getComputed();

    SupportPoint support;
    support.point = trans.getTransformed( verts[0] );
    support.proj = vec3::dot( support.point, dir );

    for( int i = 1, numVerts = verts.size(); i < numVerts; i++ ) {
        vec3 vert = trans.getTransformed( verts[i] );
        float proj = vec3::dot( vert, dir );
        if( proj > support.proj ) {
            support.point = vert;
            support.proj = proj;
        }
    }
    return support;
}

/*
----------------------------------------------------------------------
- Returns the normal and vertex on this collider with the greatest penetration into the other collider
- This is [possibly] the axis of least penetration for the collision (confusingly enough)
- The reasoning is that if the value is negative, there is penetration,
so the greatest NEGATIVE value has the least penetration

- If the value is positive, then there is no penetration i.e. there is a separating axis
----------------------------------------------------------------------
*/
FaceManifold Collider::getAxisMinPen( Collider* other ) {
    FaceManifold axis;
    axis.originator = this;
    axis.other = other;
    //axis.pen = -FLT_MAX;//keeping this to display initial value

    int numAxes = currNormals.size();
    auto meshVerts = mesh->verts();
    auto faceVerts = mesh->faces().verts;
    Transform trans = _transform->getComputed();
    for( int i = 0; axis.pen < 0 && i < numAxes; i++ ) {
        vec3 norm = currNormals[i];
        SupportPoint support = other->getSupportPoint( -norm );
        vec3 vert = trans.getTransformed( meshVerts[faceVerts[i * FLOATS_PER_VERT]] );

        float pen = vec3::dot( norm, support.point - vert );//point-plane signed distance, negative if penetrating, positive if not
        if( pen > axis.pen ) {
            axis.norm = i;
            axis.pen = pen;
        }
    }
    if( axis.pen > -FLT_MAX )
        axis.axis = currNormals[axis.norm];
    return axis;
}

/*
------------------------------------------------------------------------------------------------------------------------------------------------------------------
The principles of using Gauss Maps are as follows:
- A Gauss Map is defined here as the conversion of all the face normals on a body to points on the unit sphere,
and representing adjacencies (i.e. edges) between the faces the normals refer to as [greater] arcs on the sphere.
- Practically speaking, it's a mapping of Euclidean (R^3 aka 3D) space onto the unit sphere (S^2)
- We can make a Gauss map for any given 3D surface X because the Gauss map is mathematically defined as N: X -> S^2
such that N(p) is a normal to X at point p. The only requirement is that X is orientable so we can determine its normals.

- By overlaying the gauss maps of two colliders, one can ascertain which edges on each collider will actually need to be compared for Separating Axis Theorem.

- We determine this by checking for intersecting arcs; recall that arcs represent adjacencies between faces, or in other words common edges.
Any arcs that intersect indicate that those edges form a face on the Minkowski difference of the two colliders, and therefore their cross product is a normal that must be tested.

- We determine whether two arcs A and B intersect by performing 3 separating axis tests:
- The vertices P and Q of arc B fall on opposite sides of the plane through arc A
- The vertices R and S of arc A fall on opposite sides of the plane through arc B
- Vertices P and S are on the same side of the plane formed by Q and R (this is a hemisphere test, as the tests will fail if they are not on the same side of the sphere)
If all 3 tests are passed, then we know the edges form a face on the Minkowski difference
- I should probably explain the Minkowski difference. Essentially, it's the "point cloud" formed by subtracting all the vertices of one body from each of the vertices from another.

- It is a useful tool for collision detection, as all the faces of both original bodies are present in its convex hull,
in addition to faces formed by edges that may potentially be separating axes.

- It also has its own form of collision detection; it can be said if the origin is contained within the Minkowski difference, there is overlap between the bodies that form it,
as it means at least two points in them are located at the same position in space

- For practical purposes, however, it is almost useless as assembling it is extremely expensive, (you have to form a new one each frame, for each collision check)
which is why Gauss maps are valuable; they only need to be assembled once, as all they represent are associations and current data can be easily referenced.

- This means that we gain the benefit of not having to find a support point for every possible combination of axes on the two bodies that the Minkowski difference offers
while not having to actually assemble one.

- Face normals can be tested as they would normally, and should be tested before overlaying the gauss maps, as it requires fewer comparisons.
(though the necessity of the support points may offset this)
- Incidentally, overlaying Gauss maps is also referred to as checking for Voronoi region overlap.
- I recommend http://twvideo01.ubm-us.net/o1/vault/gdc2013/slides/822403Gregorius_Dirk_TheSeparatingAxisTest.pdf if you're interested in reading more on this technique,
as this is where most of my research originates.
------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/
EdgeManifold Collider::overlayGaussMaps( Collider* other ) {
    EdgeManifold manifold;
    manifold.originator = this;
    manifold.other = other;
    //manifold.pen = -FLT_MAX;//initial value

    GaussMap othergauss = other->getGaussMap();
    std::vector<vec3> otherNormals = other->getCurrNormals();

    Transform trans = _transform->getComputed();
    Transform otherTrans = other->transform()->getComputed();

    for( std::pair<std::string, std::vector<Adj>> pair : gauss.adjacencies ) {
        for( int i = 0, numAdj = pair.second.size(); i < numAdj; i++ ) {

            Adj curr = pair.second[i];
            vec3 a = currNormals[curr.f1], b = currNormals[curr.f2];

            for( std::pair<std::string, std::vector<Adj>> otherPair : othergauss.adjacencies ) {
                for( int j = 0, othernumAdj = otherPair.second.size(); j < othernumAdj; j++ ) {

                    //we found a separating axis boys
                    if( manifold.pen > 0 )
                        return manifold;

                    Adj otherCurr = otherPair.second[j];
                    vec3 c = -otherNormals[otherCurr.f1], d = -otherNormals[otherCurr.f2];//these must be negative to account for the minkowski DIFFERENCE

                                                                                               //checks if the arcs between arc(a,b) and arc(c,d) intersect
                    vec3 bxa = vec3::cross( b, a );
                    float cba = vec3::dot( c, bxa )
                        , dba = vec3::dot( d, bxa );

                    //if c and d are on different sides of arc ba
                    if( cba * dba < 0 ) {

                        vec3 dxc = vec3::cross( d, c );
                        float adc = vec3::dot( a, dxc )
                            , bdc = vec3::dot( b, dxc );

                        //if a and b are on different sides of arc dc &&
                        //if a and d are on the same side of the plane formed by b and c (c . (b x a) * b . (d x c) > 0)
                        //(this works because a . (b x c) == c . (b x a) and d . (b x c) == b . (d x c))(scalar triple product identity)
                        //[scalar triple product of a,b,c == [abc] = a . (b x c)]
                        if( adc * bdc < 0 && cba * bdc > 0 ) {

                            vec3 edge = getEdge( curr.edge ), otherEdge = other->getEdge( otherCurr.edge );
                            //if edges are parallel, we don't care since they don't define a plane
                            if( fuzzyParallel( edge, otherEdge ) )
                                continue;

                            //check distance from plane defined by edge normal and one vertex on this body's edge
                            vec3 cross = vec3::cross( edge, otherEdge );
                            vec3 edgeNormal = cross / vec3::length( cross );

                            vec3 v1 = getVert( curr.edge[0] ),
                                v2 = other->getVert( otherCurr.edge[0] );
                            v1 = trans.getTransformed( v1 );
                            v2 = otherTrans.getTransformed( v2 );

                            edgeNormal *= signf( vec3::dot( edgeNormal, v1 - trans.position() ) );//make sure the edge normal is facing outwards from the body
                            float pen = vec3::dot( edgeNormal, v2 - v1 );//does this work regardless of the edges' points used?
                            if( pen > manifold.pen ) {
                                manifold.edgePair[0] = curr;
                                manifold.edgePair[1] = otherCurr;
                                manifold.pen = pen;
                                manifold.axis = edgeNormal;
                            }
                        }
                    }//end edge culling

                }
            }//end other gauss loop

        }
    }//end gauss loop
    return manifold;
}

//I should mention that the assumption that models are centered at the origin is what's breaking the algorithm at all right now
//it works perfectly for meshes centered at the origin
Manifold Collider::intersects( Collider* other ) {
    //this contains the collision data
    //this will be replaced by the valid collision manifold if there is a collision
    //otherwise, the originator will be nullptr, and we know there was no collision
    Manifold manifold;

    //quick sphere collision optimization
    vec3 d = _framePos - other->framePos();//ignores displaced colliders for now (it's the one thing still breaking the algorithm)
    float distSq = d.x * d.x + d.y * d.y + d.z * d.z;
    float rad = _radius + other->radius();//need to add scale handling code to this optimization
    if( distSq > rad * rad )
        return manifold;

    //separating axis theorem 2: electric boogaloo
    //axis of min pen on this collider
    FaceManifold minAxis = getAxisMinPen( other );
    if( minAxis.pen > 0 ) {
        //std::cout << "This: " << minAxis.pen << "; " << minAxis.axis.x << ", " << minAxis.axis.y << ", " << minAxis.axis.z << std::endl;
        return manifold;
    }

    //axis of min pen on other collider
    FaceManifold otherMinAxis = other->getAxisMinPen( this );
    if( otherMinAxis.pen > 0 ) {
        //std::cout << "Other: " << otherMinAxis.pen << "; " << otherMinAxis.axis.x << ", " << otherMinAxis.axis.y << ", " << otherMinAxis.axis.z << std::endl;
        return manifold;
    }

    //closest penetrating edges on both colliders
    EdgeManifold minEdge = overlayGaussMaps( other );
    if( minEdge.pen > 0 ) {
        //debug code
        Transform t = _transform->getComputed(), ot = other->transform()->getComputed();
        vec3 v1 = t.getTransformed( getVert( minEdge.edgePair[0].edge[0] ) ), v2 = t.getTransformed( getVert( minEdge.edgePair[0].edge[1] ) )
            , ov1 = ot.getTransformed( other->getVert( minEdge.edgePair[1].edge[0] ) ), ov2 = ot.getTransformed( other->getVert( minEdge.edgePair[1].edge[1] ) );
        DrawDebug::getInstance().drawDebugVector( v1, v2, vec3( 1, 0, 0 ) );
        DrawDebug::getInstance().drawDebugVector( ov1, ov2, vec3( 1, 1, 0 ) );
        //std::cout << "Edge: " << minEdge.pen << std::endl;

        return manifold;
    }

    FaceManifold minFace = ( minAxis.pen > otherMinAxis.pen ) ? minAxis : otherMinAxis;

    //edge-edge collision
    if( minEdge.pen > minFace.pen ) {
        std::cout << "EDGE ";
        Adj e1 = minEdge.edgePair[0], e2 = minEdge.edgePair[1];
        Transform t = _transform->getComputed()
            , ot = other->transform()->getComputed();
        //get the points defining both edges in the collision in world space
        vec3 p0 = t.getTransformed( getVert( e1.edge[0] ) )
            , p1 = t.getTransformed( getVert( e1.edge[1] ) )
            , q0 = ot.getTransformed( other->getVert( e2.edge[0] ) )
            , q1 = ot.getTransformed( other->getVert( e2.edge[1] ) );

        //find the closest point between the two edges
        minEdge.colPoints.push_back( closestPointBtwnSegments( p0, p1, q0, q1 ) );

        DrawDebug::getInstance().drawDebugSphere( minEdge.colPoints[0], 0.2f );

        return minEdge;
    }
    //face-* collision
    else {
		assert(minFace.axis.x || minFace.axis.y || minFace.axis.z);// Axis has not been set, collision probably got through with NaN errors
        //find the possible incident faces
		//the axis is the reference normal
        auto incidents = minFace.other->getIncidentFaces(minFace.axis);
		assert(incidents.size());// There were no incident faces! Somehow!
        //clip the incident face(s) against the reference face
        minFace.originator->clipPolygons( minFace, incidents );

        for( int i = 0, numCols = minFace.colPoints.size(); i < numCols; i++ )
            DrawDebug::getInstance().drawDebugSphere( minFace.colPoints[i], 0.1f );

        return minFace;
    }
}

//Gets the indexes of the faces on this body that are most antiparallel to the reference normal
std::vector<int> Collider::getIncidentFaces( vec3 refNormal ) {
    std::vector<int> faces;
    float antiProj = vec3::dot( currNormals[0], refNormal );
    for( int i = 1, numFaces = currNormals.size(); i < numFaces; i++ ) {
        float proj = vec3::dot( currNormals[i], refNormal );

        //if + -, diff > 0; if - +, diff < 0; if same sign, magnitude of projection determines sign
        float diff = antiProj - proj;

        //if the face has close to 0 difference with the last anti-normal projection, then it's another incident face
        if( EPS_CHECK(diff) )
            faces.push_back( i );

        //if the face is more antiparallel than the previous, we replace our previous incident faces with this one
        else if( diff > 0 ) {
            faces = std::vector<int>();
            faces.push_back( i );
            antiProj = proj;
        }
    }
    return faces;
}

/*
------------------------------------------------------------------------------------------------------------------------------------
- Finds collision points based on Sutherland-Hodgman Clipping
- For each possible incident face on the other collider
- Clip the vertices against the side planes of the reference face on this collider
- Reject results that fall outside, include intersections and points inside
- Add final results to the set of collision points

- http://gamedevelopment.tutsplus.com/tutorials/understanding-sutherland-hodgman-clipping-for-physics-engines--gamedev-11917
------------------------------------------------------------------------------------------------------------------------------------
*/
void Collider::clipPolygons( FaceManifold& reference, std::vector<int>& incidents ) {
    Transform trans = _transform->getComputed()
        , otherTrans = reference.other->transform()->getComputed();

    vec3 refCenter, refNorm = reference.axis;

    //get the transformed center point of the reference face
    auto vertFaces = mesh->faces().verts;
    for( int i = 0; i < 3; i++ )
        refCenter += getVert( vertFaces[reference.norm * 3 + i] );
    refCenter /= 3;
    refCenter = trans.getTransformed( refCenter );

    //DrawDebug::getInstance().drawDebugVector(refCenter, refCenter + refNorm);

    //get the side planes of the reference face
    //These are supposed to be the normals of the faces adjacent to the reference face, at least according to Bullet
    //I use the actual side planes of the face, 
    //i.e. normals from the edges perpendicular to the edge and face normal facing outward from the face's center
    std::vector<vec3> sidePlanes, sideVerts;
    for( int i = 0; i < 3; i++ ) {
        int e[] = {
            (int)vertFaces[reference.norm * 3 + i]
            , (int)vertFaces[reference.norm * 3 + ( i + 1 ) % 3]
        };

        vec3 vert = trans.getTransformed( getVert( e[0] ) )
            , edge = getEdge( e );

        vec3 norm = vec3::cross( refNorm, edge );
        norm *= signf( vec3::dot( norm, vert - refCenter ) );

        sidePlanes.push_back( norm );
        sideVerts.push_back( vert );

        //DrawDebug::getInstance().drawDebugVector(vert, vert + edge, vec3(1, 1, 0));
        //DrawDebug::getInstance().drawDebugVector(vert, vert + norm, vec3(1, 0, 1));
    }

    for( int i = 0, numIncidents = incidents.size(); i < numIncidents; i++ ) {

        std::vector<vec3> incidentVerts = {
            otherTrans.getTransformed( reference.other->getVert( reference.other->getFaceVert( incidents[i] * 3 ) ) ),
            otherTrans.getTransformed( reference.other->getVert( reference.other->getFaceVert( incidents[i] * 3 + 1 ) ) ),
            otherTrans.getTransformed( reference.other->getVert( reference.other->getFaceVert( incidents[i] * 3 + 2 ) ) )
        };

        std::vector<vec3> clipped = incidentVerts;
        for( int s = 0; clipped.size() > 0 && s < 3; s++ ) {
            std::vector<vec3> input = clipped;
            clipped = clipPolyAgainstEdge( input, sidePlanes[s], sideVerts[s], refNorm, refCenter );
        }

        reference.colPoints.insert( reference.colPoints.end(), clipped.begin(), clipped.end() );
    }
}

/*
------------------------------------------------------------------------------------------------------------------------------------
- Takes an input of a set of vertices, clips them against a single edge of the reference face (a side plane) defined by sidePlane (the normal) and sideVert (point on plane)

- Culls those that don't fall below the reference face, defined by refNorm and refCenter (normal and point on plane respectively)
- Clipping has 4 cases to determine which vertices are kept, going through sets of start and end pts:
- Start and End both inside the reference face (here defined as behind it, since these are side planes facing outward): we keep only the end pt
- The reason we keep the end point and not the start point is because this algorithm ensures that all start pts will be end pts at one point, and vice versa
- This means enclosed start pts will be added at some point no matter what, so we only worry about end pts
- This case is more important for telling us there will be no intersection between the plane and the edge between start and end, so we don't need to add it

- Start and End both outside: we keep nothing, as nothing falls within the clip polygon

- Start inside and End outside: we keep the intersection between the edge and the side plane
- Because the points are on different side of the plane, there must be an intersection with the plane

- Start outside and End inside: we keep both the intersection and the end pt
- Because of how the first case actually works (telling us there is no intersection), this case and the first can be condensed together
- All we're saying is that since the start pt is NOT inside and the end pt IS, there must be an intersection, so we have to add that on top of the end pt
- Another way of describing these cases is to base it on the positioning of the end pt
- If the end pt is inside, we'll be keeping the end pt, otherwise we won't since it falls outside
- If the start pt is on the opposite side of the plane of the end pt, we also keep the intersection between the plane and the edge between the two pts

- These 2 simpler cases with possible overlap lead to the more complex explanation and the original 4 cases

- This is the version I use; it's less for optimization and more for reducing obfuscation, as the meaning here is easier to remember than the original 4 cases
- I should mention that it isn't really any less efficient than the original version; it adds a bitwise xor between startInside and endInside, but that's extremely negligible
- It's also less code, as the intersection code doesn't need to be duplicated

- There is one additional case that occurs for our purposes to account for floating point error
- Either the Start or End falls ON the side plane: we keep the end pt
- We define this as the dot product of the side plane normal and the vector from the plane to either vert being within -FLT_EPSILON and FLT_EPSILON
- We call this using thick planes, rather than planes of indeterminably small thickness which is what we normally use
------------------------------------------------------------------------------------------------------------------------------------
*/
std::vector<vec3> Collider::clipPolyAgainstEdge( std::vector<vec3>& input, vec3 sidePlane, vec3 sideVert, vec3 refNorm, vec3 refCenter ) {

    std::vector<vec3> output;

    //regular conditions protect against this, but just to be safe
    if( !input.size() )
        return output;

    vec3 startpt = input[input.size() - 1];
    vec3 endpt;

    for( int i = 0, numInputs = input.size(); i < numInputs; i++, startpt = endpt ) {
        endpt = input[i];

        float clipStart = vec3::dot( sidePlane, startpt - sideVert );
        float clipEnd = vec3::dot( sidePlane, endpt - sideVert );

        //the edge is "on the plane" (thick planes); keep end pt if it falls below reference face
        if( EPS_CHECK( clipStart ) || EPS_CHECK( clipEnd ) ) {
            if( vec3::dot( refNorm, endpt - refCenter ) < 0 )
                output.push_back( endpt );
            continue;
        }

        bool startInside = clipStart < 0, endInside = clipEnd < 0;//if they fall behind the side plane, they're inside the clip polygon

                                                                  //end pt is inside, keep it if it falls below the reference face
        if( endInside ) {
            if( vec3::dot( refNorm, endpt - refCenter ) < 0 )
                output.push_back( endpt );
        }

        //start pt and end pt fall on different sides of the plane, keep intersection if it falls below the reference face
        if( startInside != endInside ) {
            //find intersection
            float e = vec3::dot( sidePlane, startpt - endpt );
            float t = ( e != 0 ) ? clipStart / e : 0;
            //float t = clipStart * 1.f / (clipStart - clipEnd);
            vec3 intersect = vec3::lerp( startpt, endpt, t );

            if( vec3::dot( refNorm, intersect - refCenter ) < 0 )
                output.push_back( intersect );
        }
    }

    return output;
}

/*
------------------------------------------------------------------------------------------------------------------------------------
- Finds the closest point between two line segments

- For such a seemingly simple task, the solution is quite complex, and requires a bit of explanation
- Too much for here, I wrote a long post on the topic on my blog: *insert link*
- You can also find a pdf copy in the proofs folder in the root of the repository
- As an alternative, my main source was http://geomalgorithms.com/a07-_distance.html

- The general thought process is that you start with the problem of finding an intersection between two lines
- The lines are defined by p = { p0, u = p1 - p0 } and q = { q0, v = q1 - q0 }
- This uses the parametric equations for the lines and the vector between them, w.
- The smallest w is defined as w(sc, tc) or wc
	- sc = (be - cd) / (ac - b^2)
	- tc = (ae - bd) / (ac - b^2)
- You can find the values for a,b,c,d,e in the code below
- If ac - b^2 = 0, then the lines are parallel and sc = 0, tc = e / c
- The closest point to q on p is p0 + sc * u
- The closest point to p on q is q0 + tc * v
- To get the closest point in space we just compute p0 - wc / 2 or q0 + wc / 2

- The line problem isn't complex enough to account for the fact that these are segments
- The closest point between the two lines may not occur within the 0 to 1 range dictated by their segments
- We account for this ourselves by doing some range checks
- When s or t goes outside the 0 to 1 range, we have to change how we're solving for the other value
	- if s < 0, t =  e / c. if s > 1, t = ( e + b) / c
	- if t < 0, s = -d / a. if t > 1, s = (-d + b) / a
- After accounting for these, just do a basic clamp on these values to the 0 to 1 range
- We can save computations by just storing the numerators and denominators and doing the range checks with them
- That way there's only two divisions (sc and tc) at the end
------------------------------------------------------------------------------------------------------------------------------------
*/
vec3 Collider::closestPointBtwnSegments( vec3 p0, vec3 p1, vec3 q0, vec3 q1 ) {
    vec3 u = p1 - p0, v = q1 - q0, w0 = p0 - q0;

    float a = vec3::dot( u, u )
        , b = vec3::dot( u, v )
        , c = vec3::dot( v, v )
        , d = vec3::dot( u, w0 )
        , e = vec3::dot( v, w0 )
        , D = a * c - b * b;

    float sc, sNumer, sDenom = D;
    float tc, tNumer, tDenom = D;

    //if D ~ 0, i.e. segments are parallel
    if( D < FLT_EPSILON ) {
        sNumer = 0;	sDenom = 1;
        tNumer = e;	tDenom = c;
    }
    else {
        sNumer = b * e - c * d;
        tNumer = a * e - b * d;

        //if sc < 0, sc = 0 and tc = e / c
        if( sNumer < 0 ) {
            sNumer = 0;
            tNumer = e; tDenom = c;
        }
        //if sc > 1, sc = 1 and tc = (e + b) / c
        else if( sNumer > sDenom ) {
            sNumer = sDenom;
            tNumer = e + b; tDenom = c;
        }
    }

    //if tc < 0, tc = 0 and sc = -d / a
    if( tNumer < 0 ) {
        tNumer = 0;
        //if sc < 0, sc = 0; tc = 0
        if( -d < 0 )
            sNumer = 0;
        //if sc > 1, sc = 1; tc = 0
        else if( -d > a )
            sNumer = sDenom;
        else {
            sNumer = -d; sDenom = a;
        }
    }
    //if tc > 1, tc = 1 and sc = (-d + b) / a
    else if( tNumer > tDenom ) {
        tNumer = tDenom;
        //if sc < 0, sc = 0; tc = 1
        if( -d + b < 0 )
            sNumer = 0;
        //if sc > 1, sc = 1; tc = 1
        else if( -d + b > a )
            sNumer = sDenom;
        else {
            sNumer = -d + b; sDenom = a;
        }
    }

    //prevents possible divide by zero
    sc = ( EPS_CHECK( sNumer ) ) ? 0 : sNumer / sDenom;
    tc = ( EPS_CHECK( tNumer ) ) ? 0 : tNumer / tDenom;

    vec3 wc = w0 + ( sc * u ) - ( tc * v );//the vec between the 2 closest pts on the 2 segments
    return q0 + tc * v + wc * 0.5f;//the closest point between the 2 segments in the world
}

//the code below will need updating when the system is updated for 3D
/*void Collider::setCorners(std::vector<vec3> c) {
corners = c;
}*/

void Collider::genVerts() {
    if( _type != ColliderType::BOX )
        return;
    std::vector<vec3> verts, norms, uvs;//norms and uvs are empty
    Face faces;
    verts.push_back( vec3( _dims.x, _dims.y, _dims.z ) );
    verts.push_back( vec3( -_dims.x, _dims.y, _dims.z ) );
    verts.push_back( vec3( _dims.x, -_dims.y, _dims.z ) );
    verts.push_back( vec3( -_dims.x, -_dims.y, _dims.z ) );
    verts.push_back( vec3( _dims.x, _dims.y, -_dims.z ) );
    verts.push_back( vec3( -_dims.x, _dims.y, -_dims.z ) );
    verts.push_back( vec3( _dims.x, -_dims.y, -_dims.z ) );
    verts.push_back( vec3( -_dims.x, -_dims.y, -_dims.z ) );

    faces.verts.push_back( 7 ); faces.verts.push_back( 3 ); faces.verts.push_back( 0 );
    faces.verts.push_back( 7 ); faces.verts.push_back( 4 ); faces.verts.push_back( 0 );

    faces.verts.push_back( 6 ); faces.verts.push_back( 7 ); faces.verts.push_back( 4 );
    faces.verts.push_back( 6 ); faces.verts.push_back( 5 ); faces.verts.push_back( 4 );

    faces.verts.push_back( 2 ); faces.verts.push_back( 6 ); faces.verts.push_back( 5 );
    faces.verts.push_back( 2 ); faces.verts.push_back( 1 ); faces.verts.push_back( 5 );

    faces.verts.push_back( 3 ); faces.verts.push_back( 2 ); faces.verts.push_back( 1 );
    faces.verts.push_back( 3 ); faces.verts.push_back( 0 ); faces.verts.push_back( 1 );

    faces.verts.push_back( 0 ); faces.verts.push_back( 1 ); faces.verts.push_back( 5 );
    faces.verts.push_back( 0 ); faces.verts.push_back( 4 ); faces.verts.push_back( 5 );

    faces.verts.push_back( 7 ); faces.verts.push_back( 6 ); faces.verts.push_back( 2 );
    faces.verts.push_back( 7 ); faces.verts.push_back( 3 ); faces.verts.push_back( 2 );

    mesh = new Mesh( verts, norms, uvs, faces );
}

void Collider::genNormals() {
    switch( _type ) {
    case ColliderType::SPHERE:
        break;
    case ColliderType::BOX:
        faceNormals.push_back( vec3( 1, 0, 0 ) );//need to define some kind of vertex array for box colliders
        faceNormals.push_back( vec3( 0, 0, -1 ) );
        faceNormals.push_back( vec3( -1, 0, 0 ) );
        faceNormals.push_back( vec3( 0, 0, 1 ) );
        faceNormals.push_back( vec3( 0, 1, 0 ) );
        faceNormals.push_back( vec3( 0, -1, 0 ) );
        break;
    case ColliderType::MESH:
        //generate the face normals from the mesh's vertices
        std::vector<uint32_t>& faceVerts = mesh->faces().verts;
        std::vector<vec3> meshVerts = mesh->verts();
        int numFaces = faceVerts.size();
        for( int i = 0; i < numFaces; i += 3 ) {
            vec3 normal, e1, e2, v, cross;
            v = meshVerts[faceVerts[i]];
            e1 = meshVerts[faceVerts[i + 1]] - v;
            e2 = meshVerts[faceVerts[i + 2]] - v;
            cross = vec3::cross( e1, e2 );
            normal = cross / vec3::length( cross );
			//when iterating over normals, to retrieve the vertices of the face corresponding to the normal at index i,
			//the nth (0, 1, or 2) vertex in the face is meshVerts[faceVerts[i * 3 + n]]
			//alternatively, if you aren't getting meshVerts, use the function getVert(faceVerts[i * 3 + n])
            faceNormals.push_back( normal );
        }
        break;
    }
    currNormals = std::vector<vec3>( faceNormals.size() );
}

void Collider::genEdges() {
    switch( _type ) {
    case ColliderType::SPHERE:
        break;
    case ColliderType::BOX:
    case ColliderType::MESH:
        for( std::pair<std::string, std::vector<Adj>> pair : gauss.adjacencies ) {
            for( int i = 0, numAdj = pair.second.size(); i < numAdj; i++ ) {
                setEdge( pair.second[i].edge, edges.size() );
                edges.push_back( getVert( pair.second[i].edge[1] ) - getVert( pair.second[i].edge[0] ) );
            }
        }
        break;
    }
    currEdges = std::vector<vec3>( edges.size() );
}

void Collider::genGaussMap() {
    switch( _type ) {
    case ColliderType::SPHERE:
        break;
    case ColliderType::BOX:
        //need to set up proper handling for box colliders for vertices
        //gauss.addAdj(faceNormals[0], Adj{  });
        //gauss.adjacencies[faceNormals[1]] = { Adj{ 2, {,} } };
        break;
    case ColliderType::MESH:
        //set up the edge associations
        std::vector<uint32_t>& faceVerts = mesh->faces().verts;
        int numFaces = faceVerts.size();
        //adjacencies = std::vector<std::vector<Adj>>(numFaces / 3, std::vector<Adj>());
        for( int i = 0; i < numFaces; i += 3 ) {
            for( int j = i + 3; j < numFaces; j += 3 ) {

                //if (fuzzyParallel(faceNormals[i / 3], faceNormals[j / 3]))
                //continue;
                Adj a;
                a.edge[0] = -1; a.edge[1] = -1;

                bool added = false;
                for( int p1 = 0; !added && p1 < 3; p1++ ) {
                    for( int p2 = 0; p2 < 3; p2++ ) {

                        //checks if a pair of vertices match
                        if( faceVerts[i + p1] == faceVerts[j + p2] ) {
                            //if a match hasn't been found yet, just record it
                            if( a.edge[0] < 0 )
                                a.edge[0] = faceVerts[i + p1];
                            //otherwise, record it, set the normal, push it back, and end the loop
                            else {
                                a.edge[1] = faceVerts[i + p1]; a.f1 = i / 3; a.f2 = j / 3;
                                if( a.edge[0] > a.edge[1] ) { a.edge[1] = a.edge[0]; a.edge[0] = faceVerts[i + p1]; }
                                gauss.addAdj( faceNormals[a.f1], a );

                                //adjacencies[a.f1].push_back(a);
                                //adjacencies[a.f2].push_back(a);

                                added = true;
                            }
                            break;//none of the other verts can be equal to this one now, so move to the next one
                        }//end vert comparison

                    }
                }//end edge loop

            }
        }//end face loop
        break;
    }
}

bool Collider::fuzzyParallel( vec3 v1, vec3 v2 ) {
    if( v1 == v2 )
        return true;
    float propx = ( v1.x ) ? v2.x / v1.x : 0;
    float propy = ( v1.y ) ? v2.y / v1.y : 0;
    float eps = FLT_EPSILON;
    bool para = fabs( ( propy - propx ) / propx ) < eps 
			 && fabs( ( ( ( v1.z ) ? v2.z / v1.z : 0 ) - propx ) / propx ) < eps;
    return para;
}

void Collider::updateNormals() {
    switch( _type ) {
    case ColliderType::SPHERE:
        break;
    case ColliderType::BOX:
    case ColliderType::MESH:
        Transform t = _transform->getComputed();
        mat4 rot = mat4::rotate( t.rotAngle(), t.rotAxis() );
        //auto faceVerts = mesh->faces().verts;
        for( int i = 0, numNormals = faceNormals.size(); i < numNormals; i++ ) {
            currNormals[i] = ( vec3 )( vec4( faceNormals[i], 1 ) * rot );

            /*vec3 a = t.getTransformed(getVert(faceVerts[i * 3])), b = t.getTransformed(getVert(faceVerts[i * 3 + 1])), c = t.getTransformed(getVert(faceVerts[i * 3 + 2]));
            vec3 center = a + b + c;
            center /= 3;
            DrawDebug::getInstance().drawDebugVector(center, center + currNormals[i]);*/
        }
        break;
    }
}

void Collider::updateEdges() {
    switch( _type ) {
    case ColliderType::SPHERE:
        break;
    case ColliderType::BOX:
    case ColliderType::MESH:
        Transform t = _transform->getComputed();
        mat4 rot = mat4::rotate( t.rotAngle(), t.rotAxis() );
        mat4 scale = mat4::scale( t.scale() );
        int numEdges = edges.size();
        for( int i = 0; i < numEdges; i++ ) {
            currEdges[i] = ( vec3 )( vec4( edges[i], 1 ) * scale * rot );//this is probably slow
        }
        /*
        for (std::pair<std::string, std::vector<Adj>> pair : gauss.adjacencies) {
        for (int i = 0, numAdj = pair.second.size(); i < numAdj; i++) {
        Adj a = pair.second[i];
        vec3 s = t.getTransformed(getVert(a.edge[0]));
        vec3 edge = getEdge(a.edge);
        DrawDebug::getInstance().drawDebugVector(s,s + edge);
        }
        }*/
        break;
    }
}

const std::vector<vec3>& Collider::getCurrNormals() const { return currNormals; }
const std::vector<vec3>& Collider::getEdges() const { return currEdges; }

int Collider::getFaceVert( int index ) const { return mesh->faces().verts[index]; }
vec3 Collider::getVert( int index ) const { return mesh->verts()[index]; }
vec3 Collider::getNormal( int index ) const { return currNormals[index]; }
vec3 Collider::getEdge( int( &e )[2] ) {
    if( e[1] < e[0] ) { int temp = e[0]; e[0] = e[1]; e[1] = temp; }
    std::string key = std::to_string( e[0] ) + "," + std::to_string( e[1] );
    return currEdges[edgeMap[key]];
}
void Collider::setEdge( int( &e )[2], int index ) {
    if( e[1] < e[0] ) { int temp = e[0]; e[0] = e[1]; e[1] = temp; }
    std::string key = std::to_string( e[0] ) + "," + std::to_string( e[1] );
    edgeMap[key] = index;
}

const GaussMap& Collider::getGaussMap() const { return gauss; }

std::vector<Adj>& GaussMap::getAdjs( vec3 v ) {
    std::string key = std::to_string( v.x ) + "," + std::to_string( v.y ) + "," + std::to_string( v.z );
    return adjacencies[key];
}

void GaussMap::addAdj( vec3 v, Adj a ) {
    std::string key = std::to_string( v.x ) + "," + std::to_string( v.y ) + "," + std::to_string( v.z );
    adjacencies[key].push_back( a );
}

bool AABB::intersects(const AABB& other) {
    bool xSeparate = ( center.x - halfDims.x > other.center.x + other.halfDims.x ) || ( center.x + halfDims.x < other.center.x - other.halfDims.x );
    bool ySeparate = ( center.y - halfDims.y > other.center.y + other.halfDims.y ) || ( center.y + halfDims.y < other.center.y - other.halfDims.y );
    bool zSeparate = ( center.z - halfDims.z > other.center.z + other.halfDims.z ) || ( center.z + halfDims.z < other.center.z - other.halfDims.z );
    return !( xSeparate || ySeparate || zSeparate );
}