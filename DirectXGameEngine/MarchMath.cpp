#include "MarchMath.h"
#include <cstdio>
#include <intrin.h>

//misc math functions
int sign( int i ) { return ( i > 0 ) ? 1 : -1; } float signf( float f ) { return ( f > 0 ) ? 1.f : -1.f; }
float maxf(float a, float b) { _mm_store_ss(&a, _mm_max_ss(_mm_set_ss(a), _mm_set_ss(b))); return a; }
float minf(float a, float b) { _mm_store_ss(&a, _mm_min_ss(_mm_set_ss(a), _mm_set_ss(b))); return a; }
float clampf(float val, float min, float max) { _mm_store_ss(&val, _mm_min_ss(_mm_max_ss(_mm_set_ss(val),_mm_set_ss(max)), _mm_set_ss(min))); return val; }
float lerpf( float a, float b, float t ) { return a * ( 1 - t ) + b * t; }

//vec3
vec3::vec3() : x(v[0]), y(v[1]), z(v[2]) { memset(v, 0, 3 * sizeof(float)); } vec3::~vec3() {}
vec3::vec3( const vec3& other ) : x( v[0] ), y( v[1] ), z( v[2] ) { for (int i = 0; i < 3; i++) v[i] = other.v[i]; }
vec3& vec3::operator=( const vec3& other ) { for (int i = 0; i < 3; i++) v[i] = other.v[i]; return *this; }
vec3& vec3::operator+=( const vec3& other ) { for (int i = 0; i < 3; i++) v[i] += other.v[i]; return *this; }
vec3& vec3::operator-=( const vec3& other ) { for (int i = 0; i < 3; i++) v[i] -= other.v[i]; return *this; }
vec3& vec3::operator*=( float f ) { for (int i = 0; i < 3; i++) v[i] *= f; return *this; }
vec3& vec3::operator/=(float f) { float _f = 1.f / f; for (int i = 0; i < 3; i++) v[i] *= _f; return *this; }
vec3::vec3( float _x, float _y, float _z ) : x( v[0] ), y( v[1] ), z( v[2] ) { v[0] = _x; v[1] = _y; v[2] = _z; }
vec3::vec3( float* _v ) : x( v[0] ), y( v[1] ), z( v[2] ) { for (int i = 0; i < 3; i++) v[i] = _v[i]; }
vec3::vec3( vec4 _v ) : x( v[0] ), y( v[1] ), z( v[2] ) { for (int i = 0; i < 3; i++) v[i] = _v[i]; }

bool vec3::operator==( const vec3& other ) { return v[0] == other.v[0] && v[1] == other.v[1] && v[2] == other.v[2]; }
bool vec3::operator<( const vec3& other ) { return v[0] < other.v[0] && v[1] < other.v[1] && v[2] < other.v[2]; }
float vec3::operator[]( int i ) const { return v[i]; } float& vec3::operator[]( int i ) { return v[i]; }
vec3 vec3::operator+( const vec3& other ) const { return vec3( v[0] + other.v[0], v[1] + other.v[1], v[2] + other.v[2] ); }
vec3 vec3::operator-( const vec3& other ) const { return vec3( v[0] - other.v[0], v[1] - other.v[1], v[2] - other.v[2] ); }
vec3 vec3::operator-() const { return vec3( -v[0], -v[1], -v[2] ); }
vec3 vec3::operator*( float f ) const { return vec3( v[0] * f, v[1] * f, v[2] * f ); }
vec3 vec3::operator/(float f) const { float _f = 1.f / f; return vec3(v[0] * _f, v[1] * _f, v[2] * _f); }
vec3 operator*( float f, const vec3& v ) { return vec3( v[0] * f, v[1] * f, v[2] * f ); }
vec3 operator/(float f, const vec3& v) { float _f = 1.f / f; return vec3(v[0] * _f, v[1] * _f, v[2] * _f); }

vec3 vec3::operator*(const mat3& m) { vec3 result; for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) result[i] += v[j] * m[i][j]; return result; }

float vec3::length( const vec3 v ) { return sqrtf( v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2] ); }
float vec3::dot( const vec3 a, const vec3 b ) { return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2]; }
vec3 vec3::cross( const vec3 a, const vec3 b ) { return vec3( a.v[1] * b.v[2] - a.v[2] * b.v[1], a.v[2] * b.v[0] - a.v[0] * b.v[2], a.v[0] * b.v[1] - a.v[1] * b.v[0] ); }
vec3 vec3::lerp( const vec3 a, const vec3 b, float t ) { return vec3( lerpf( a.v[0], b.v[0], t ), lerpf( a.v[1], b.v[1], t ), lerpf( a.v[2], b.v[2], t ) ); }

//vec4
vec4::vec4() : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] ) { memset(v, 0, 4 * sizeof(float)); } vec4::~vec4() {}
vec4::vec4( const vec4& other ) : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] ) { for (int i = 0; i < 4; i++) v[i] = other.v[i]; }
vec4& vec4::operator=( const vec4& other ) { for (int i = 0; i < 4; i++) v[i] = other.v[i]; return *this; }
vec4::vec4( float _x, float _y, float _z, float _w ) : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] ) { v[0] = _x; v[1] = _y; v[2] = _z; v[3] = _w; }
vec4::vec4( float* _v ) : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] ) { for (int i = 0; i < 3; i++) v[i] = _v[i]; }
vec4::vec4( vec3 _v, float _w ) : x( v[0] ), y( v[1] ), z( v[2] ), w( v[3] ) { for (int i = 0; i < 3; i++) v[i] = _v[i]; v[3] = _w; }

float vec4::operator[]( int i ) const { return v[i]; } float& vec4::operator[]( int i ) { return v[i]; }
vec4 vec4::operator+( const vec4& other ) { return vec4( x + other.x, y + other.y, z + other.z, w + other.w ); }
vec4 vec4::operator-( const vec4& other ) { return vec4( x - other.x, y - other.y, z - other.z, w - other.w ); }
vec4 vec4::operator*( float f ) { return vec4( x * f, y * f, z * f, w * f ); }
vec4 vec4::operator/( float f ) { return vec4( x / f, y / f, z / f, w / f ); }

vec4 vec4::operator*(const mat4& m) { vec4 result; for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) result[i] += v[j] * m[i][j]; return result; }

float vec4::length( vec4 v ) { return sqrtf( v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w ); }
float vec4::dot( vec4 a, vec4 b ) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
vec4 vec4::lerp( vec4 a, vec4 b, float t ) { return vec4( lerpf( a.x, b.x, t ), lerpf( a.y, b.y, t ), lerpf( a.z, b.z, t ), lerpf( a.w, b.w, t ) ); }

//mat3
//the zero matrix
mat3::mat3() { memset(m, 0, 9 * sizeof(float)); } mat3::~mat3() {}
//sets the diagonal to f, everything else 0
mat3::mat3(float f) { memset(m, 0, 9 * sizeof(float)); for (int i = 0; i < 3; i++) m[i << 2] = f; }
mat3::mat3(float r1, float r2, float r3) { memset(m, 0, 9 * sizeof(float)); m[0] = r1; m[4] = r2; m[8] = r3; }
mat3::mat3(vec3 c1, vec3 c2, vec3 c3) { for (int i = 0; i < 3; i++) { m[i] = c1[i]; m[i + 3] = c2[i]; m[i + 6] = c3[i]; } }
mat3::mat3(float* _m) { for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) m[i * 3 + j] = _m[i * 3 + j]; }

//accessor is [col][row]... which is weird, I know
const float* mat3::operator[](int i) const { return &m[(i << 1) + i]; } float* mat3::operator[](int i) { return &m[(i << 1) + i]; }
mat3 mat3::operator+(const mat3& other) { mat3 r(m); for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) r[i][j] += other[i][j]; return r; }
mat3 mat3::operator-(const mat3& other) { mat3 r(m); for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) r[i][j] -= other[i][j]; return r; }
mat3 mat3::operator*(float f) { mat3 r(m); for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) r[i][j] *= f; return r; }
mat3 mat3::operator/(float f) { mat3 r(m); for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) r[i][j] /= f; return r; }

mat3 mat3::operator*(const mat3& other) { mat3 r; for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) for (int k = 0; k < 3; k++) r[j][i] += m[k * 3 + i] * other[j][k]; return r; }

mat3 mat3::transpose(mat3& m) { mat3 r; for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) r[i][j] = m[j][i]; return r; }

//mat4
//the zero matrix
mat4::mat4() { memset(m, 0, sizeof(float) << 4); } mat4::~mat4() {}
//sets the diagonal to f, everything else 0
mat4::mat4( float f ) { memset(m, 0, sizeof(float) << 4); for( int i = 0; i < 4; i++ ) m[( i << 2 ) + i] = f; }
mat4::mat4(float r1, float r2, float r3, float r4) { memset(m, 0, sizeof(float) << 4); m[0] = r1; m[5] = r2; m[10] = r3; m[15] = r4; }
mat4::mat4(vec4 c1, vec4 c2, vec4 c3, vec4 c4) { for (int i = 0; i < 4; i++) { m[i] = c1[i]; m[i + 4] = c2[i]; m[i + 8] = c3[i]; m[i + 12] = c4[i]; } }
mat4::mat4( float* _m ) { for( int i = 0; i < 4; i++ ) for( int j = 0; j < 4; j++ ) m[( i << 2 ) + j] = _m[( i << 2 ) + j]; }

//accessor is [col][row]... which is weird, I know
const float* mat4::operator[]( int i ) const { return &m[i << 2]; } float* mat4::operator[]( int i ) { return &m[i << 2]; }
mat4 mat4::operator+( const mat4& other ) { mat4 r( m ); for( int i = 0; i < 4; i++ ) for( int j = 0; j < 4; j++ ) r[i][j] += other[i][j]; return r; }
mat4 mat4::operator-( const mat4& other ) { mat4 r( m ); for( int i = 0; i < 4; i++ ) for( int j = 0; j < 4; j++ ) r[i][j] -= other[i][j]; return r; }
mat4 mat4::operator*( float f ) { mat4 r( m ); for( int i = 0; i < 4; i++ ) for( int j = 0; j < 4; j++ ) r[i][j] *= f; return r; }
mat4 mat4::operator/( float f ) { mat4 r( m ); for( int i = 0; i < 4; i++ ) for( int j = 0; j < 4; j++ ) r[i][j] /= f; return r; }

mat4 mat4::operator*( const mat4& other ) { mat4 r; for( int i = 0; i < 4; i++ ) for( int j = 0; j < 4; j++ ) for( int k = 0; k < 4; k++ ) r[j][i] += m[( k << 2 ) + i] * other[j][k]; return r; }

mat4 mat4::transpose( mat4& m ) { mat4 r; for( int i = 0; i < 4; i++ ) for( int j = 0; j < 4; j++ ) r[i][j] = m[j][i]; return r; }
//based on the transform inverse shortcut where the mat is [M 0, v 1] (rows) and the inverse is [M^-1 0, -M^-1*v 1]
mat4 mat4::inv_tp_tf( mat4& m ) {
    //the reason we do it this way is to save operations
    //the mat4 [] returns a pointer to m[i<<2]
    float* c1 = m[0], *c2 = m[1], *c3 = m[2];
    float a = *( c1 ), b = *( c2 ), c = *( c3 ), d = *( ++c1 ), e = *( ++c2 ), f = *( ++c3 ), g = *( ++c1 ), h = *( ++c2 ), i = *( ++c3 );
    float ei_fh = e * f - f * h, fg_di = f * g - d * i, dh_eg = d * h - e * g;
    float det = a * ei_fh + b *  fg_di + c * dh_eg, _det = 1.f / det;
    //this is already transposed
    float r[] = {
        ei_fh * _det,	( c*h - b*i ) * _det, ( b*f - c*e ) * _det, 0,
        fg_di * _det,	( a*i - c*g ) * _det, ( c*d - a*f ) * _det, 0,
        dh_eg * _det,	( b*g - a*h ) * _det, ( a*e - b*d ) * _det, 0,
        0,				0,					0,					1
    };
    float x = *( ++c1 ), y = *( ++c2 ), z = *( ++c3 );
    r[12] = -( r[0] * x + r[4] * y + r[8] * z );
    r[13] = -( r[1] * x + r[5] * y + r[9] * z );
    r[14] = -( r[2] * x + r[6] * y + r[10] * z );
    return mat4( r );
}

mat4 mat4::translate( vec3 v ) { mat4 t = mat4( 1 ); t[0][3] = v.x; t[1][3] = v.y; t[2][3] = v.z; return t; }
mat4 mat4::rotate( float theta, vec3 a ) {
    float c = cosf( theta ), t = 1 - c, s = sinf( theta );
    float tx = t * a.x, ty = t * a.y, txy = tx * a.y, txz = tx * a.z, tyz = ty * a.z;
    float sx = s * a.x, sy = s * a.y, sz = s * a.z;
    float m[] = {
        tx * a.x + c,	txy + sz,		txz - sy,			0,
        txy - sz,		ty * a.y + c,	tyz + sx,			0,
        txz + sy, 		tyz - sx,		t * a.z * a.z + c,	0,
        0,				0,				0,					1
    };
    return mat4( m );
}
mat4 mat4::scale( vec3 v ) { mat4 s = mat4(); s[0][0] = v.x; s[1][1] = v.y; s[2][2] = v.z; s[3][3] = 1.f; return s; }

mat4 mat4::lookAt( vec3 eye, vec3 target, vec3 up ) {
    vec3 f = target - eye; f /= vec3::length( f );
    vec3 s = vec3::cross( up, f ); s /= vec3::length( s );
    vec3 u = vec3::cross( f, s );
    float r[] = {
        s.x,	s.y,	s.z,	-vec3::dot( eye, s ),
        u.x,	u.y,	u.z,	-vec3::dot( eye, u ),
        f.x,	f.y, 	f.z,	-vec3::dot( eye, f ),
        0,		0, 		0,		1
    };
    return mat4( r );
}
mat4 mat4::perspectiveFOV( float fovy, float aspect, float zNear, float zFar ) {
    mat4 r;
    float frustDepth = zFar - zNear, _fd = zFar / frustDepth, hfov = fovy * 0.5f;
    float uh = cosf( hfov ) / sinf( hfov ), uw = uh / aspect;
    r[0][0] = uw; r[1][1] = uh;	r[3][2] = 1; r[2][2] = _fd; r[2][3] = -zNear * _fd;
    return r;
}
//TODO: fix below
mat4 mat4::perspective( float width, float height, float zNear, float zFar ) {
    mat4 r;
    float frustDepth = zFar - zNear, _fd = zFar / frustDepth;
    r[0][0] = 2.f * zNear / width; r[1][1] = 2.f * zNear / height; r[3][2] = 1;
    r[2][2] = _fd; r[2][3] = -zNear * _fd;
    return r;
}
mat4 mat4::orthographic( float right, float left, float bottom, float top, float zNear, float zFar ) {
    mat4 r;
    float rl = right - left, _rl = 1.f / rl, tb = top - bottom, _tb = 1.f / tb, frustDepth = zFar - zNear, _fd = 1 / frustDepth;
    r[0][0] = 2.f * _rl; r[1][1] = 2.f * _tb; r[2][2] = 1.f * _fd; r[3][3] = 1.f;
    r[3][0] = -( right + left ) * _rl; r[3][1] = -( top + bottom ) * _tb; r[3][2] = -zNear * _fd;
    return r;
}
//assumes symmetric view frustrum
mat4 mat4::orthographic( float width, float height, float zNear, float zFar ) {
    mat4 r;
    float frustDepth = zFar - zNear, _fd = 1.f / frustDepth;
    r[0][0] = 2.f / width; r[1][1] = 2.f / height; r[3][3] = 1.f;
    r[2][2] = 1.f * _fd; r[3][2] = -zNear * _fd;
    return r;
}

//quat
#include <assert.h>
quat::quat() : _v( vec3() ), v( _v ), w( v0 ) { v0 = 1; _theta = 0; _axis = vec3( 0, 0, 1 ); } quat::~quat() {}
quat::quat( const quat& other ) : _v( other._v ), v( _v ), w( v0 ) { v0 = other.v0; _theta = other._theta; _axis = other._axis; }
quat& quat::operator=( const quat& other ) { v0 = other.v0; _v = other._v; _theta = other._theta; _axis = other._axis; return *this; }
quat::quat( float _x, float _y, float _z, float _w ) : _v( vec3(_x, _y, _z) ), v( _v ), w( v0 ) { clampf(_w, -1.f, 1.f); v0 = _w; _theta = acosf( v0 ); assert(!NaN_CHECK(_theta)); sin_t_half = sinf( _theta ); _axis = ( sin_t_half ) ? _v / sin_t_half : vec3( 0, 0, 1 ); _axis /= vec3::length( _axis ); _theta *= 2; }
quat::quat(float _v0, vec3 v1) : _v(v1), v(_v), w(v0) { clampf(_v0, -1.f, 1.f); v0 = _v0; _theta = acosf(v0); assert(!NaN_CHECK(_theta)); sin_t_half = sinf(_theta); _axis = (sin_t_half) ? _v / sin_t_half : vec3(0, 0, 1); _axis /= vec3::length(_axis); _theta *= 2; }
quat::quat( vec3 a, float t ) : _v( vec3() ), v( _v ), w( v0 ) { _theta = t; t *= 0.5f; _axis = a; sin_t_half = sinf( t ); v0 = cosf( t ); _v = a * sin_t_half; }

float quat::theta() const { return _theta; } void quat::theta( float t ) { _theta = t; t *= 0.5f; sin_t_half = sinf( t ); v0 = cosf( t ); _v = _axis * sin_t_half; }
vec3 quat::axis() const { return _axis; } void quat::axis( vec3 a ) { _axis = a; v = a * sin_t_half; }

quat quat::operator+( const quat& other ) { return quat( v0 + other.v0, _v + other._v ); } quat quat::operator-( const quat& other ) { return quat( v0 - other.v0, _v - other._v ); }
quat quat::operator*( float f ) { return quat( v0 * f, _v * f ); } quat quat::operator/( float f ) { return quat( v0 / f, _v / f ); }
quat quat::operator*( const quat& other ) const { return quat( v0 * other.v0 - vec3::dot( _v, other._v ), other._v * v0 + _v * other.v0 + vec3::cross( _v, other._v ) ); }

float quat::length(const quat& q) { return sqrtf(q.v0 * q.v0 + q._v[0] * q._v[0] + q._v[1] * q._v[1] + q._v[2] * q._v[2]); }

quat quat::pow( const quat& q, float e ) { return quat::rotation( q._theta * e, q._axis ); }
quat quat::inverse( const quat& q ) { return quat( q.v0, -q._v ); }
quat quat::rotate( const quat& q, float theta, vec3 axis ) { return q * quat( axis, theta ); }

quat quat::slerp( const quat& a, const quat& b, float t ) { return quat::pow( b * quat::inverse( a ), t ) * a; }
quat quat::rotation( float theta, vec3 axis ) { return quat( axis, theta ); }
quat quat::point( float x, float y, float z ) { return quat( x, y, z, 0 ); }