#include "MarchMath.h"

//misc math functions
float lerpf(float a, float b, float t) { return a * (1 - t) + b * t; }

//vec3
vec3::vec3() : x(v[0]), y(v[1]), z(v[2]) { for (int i = 0; i < 3; i++) v[i] = 0; } vec3::~vec3() {}
vec3::vec3(const vec3& other) : x(v[0]), y(v[1]), z(v[2]) { x = other.x; y = other.y; z = other.z; }
vec3& vec3::operator=(const vec3& other) { x = other.x; y = other.y; z = other.z; return *this; }
vec3::vec3(float _x, float _y, float _z) : x(v[0]), y(v[1]), z(v[2]) { x = _x; y = _y; z = _z; }
vec3::vec3(float* _v) : x(v[0]), y(v[1]), z(v[2]) { x = _v[0]; y = _v[1]; z = _v[2]; }

float vec3::operator[](int i) const { return v[i]; } float& vec3::operator[](int i) { return v[i]; }
vec3 vec3::operator+(const vec3& other) { return vec3(x + other.x, y + other.y, z + other.z); }
vec3 vec3::operator-(const vec3& other) { return vec3(x - other.x, y - other.y, z - other.z); }
vec3 vec3::operator*(float f) { return vec3(x * f, y * f, z * f); }
vec3 vec3::operator/(float f) { return vec3(x / f, y / f, z / f); }

float vec3::length(vec3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
float vec3::dot(vec3& a, vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 vec3::cross(vec3& a, vec3& b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
vec3 vec3::lerp(vec3& a, vec3& b, float t) { return vec3(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t)); }

//vec4
vec4::vec4() : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { for (int i = 0; i < 4; i++) v[i] = 0; } vec4::~vec4() {}
vec4::vec4(const vec4& other) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { x = other.x; y = other.y; z = other.z; w = other.w; }
vec4& vec4::operator=(const vec4& other) { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
vec4::vec4(float _x, float _y, float _z, float _w) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { x = _x; y = _y; z = _z; w = _w; }
vec4::vec4(float* _v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { x = _v[0]; y = _v[1]; z = _v[2]; w = _v[3]; }
vec4::vec4(vec3 _v, float _w) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { x = _v.x; y = _v.y; z = _v.z; w = _w; }

float vec4::operator[](int i) const { return v[i]; } float& vec4::operator[](int i) { return v[i]; }
vec4 vec4::operator+(const vec4& other) { return vec4(x + other.x, y + other.y, z + other.z, w + other.w); }
vec4 vec4::operator-(const vec4& other) { return vec4(x - other.x, y - other.y, z - other.z, w - other.w); }
vec4 vec4::operator*(float f) { return vec4(x * f, y * f, z * f, w * f); }
vec4 vec4::operator/(float f) { return vec4(x / f, y / f, z / f, w / f); }

float vec4::length(vec4& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); }
float vec4::dot(vec4& a, vec4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
vec4 vec4::lerp(vec4& a, vec4& b, float t) { return vec4(lerpf(a.x, b.x, t), lerpf(a.y, b.y, t), lerpf(a.z, b.z, t), lerpf(a.w, b.w, t)); }

//mat4
mat4::mat4() { for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m[i * 4 + j] = 0; } mat4::~mat4() {}
mat4::mat4(float f) { for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m[i * 4 + j] = f; }
mat4::mat4(float* _m) { for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) m[i * 4 + j] = _m[i * 4 + j]; }

const float* mat4::operator[](int i) const { return &m[i * 4]; } float* mat4::operator[](int i) { return &m[i * 4]; }
mat4 mat4::operator+(const mat4& other) { mat4 r(m); for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) r[i][j] += other[i][j]; return r; }
mat4 mat4::operator-(const mat4& other) { mat4 r(m); for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) r[i][j] -= other[i][j]; return r; }
mat4 mat4::operator*(float f) { mat4 r(m); for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) r[i][j] *= f; return r; }
mat4 mat4::operator/(float f) { mat4 r(m); for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) r[i][j] /= f; return r; }

mat4 mat4::operator*(const mat4& other) {
	mat4 r;
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) for (int k = 0; k < 4; k++)
		r[j][i] += m[k * 4 + i] * other[j][k];
	return r;
}
vec4 mat4::operator*(const vec4& v) { vec4 result; for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) result[j] += m[j * 4 + i] * v[i]; return result; }

mat4 mat4::transpose(mat4& m) { mat4 r; for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) r[i][j] = r[j][i]; return r; }

//quat
quat::quat() : v(_v), x(v.x), y(v.y), z(v.z), w(v0) { v0 = 0; v = vec3(); _theta = 0; _axis = vec3(); } quat::~quat() {}
quat::quat(const quat& other) : v(_v), x(v.x), y(v.y), z(v.z), w(v0) { w = other.w; v = other.v; _theta = other.theta(); _axis = other.axis(); }
quat& quat::operator=(const quat& other) { w = other.w; v = other.v; _theta = other.theta(); _axis = other.axis(); }
quat::quat(float _x, float _y, float _z, float _w) : v(_v), x(v.x), y(v.y), z(v.z), w(v0) { w = _w; v = vec3(_x, _y, _z); _theta = acosf(v0); sin_t_half = sinf(_theta); _axis = v / sin_t_half; _theta *= 2; }
quat::quat(float _v0, vec3 _v) : v(_v), x(v.x), y(v.y), z(v.z), w(v0) { v0 = _v0; v = _v; _theta = acosf(v0); sin_t_half = sinf(_theta); _axis = v / sin_t_half; _theta *= 2; }
quat::quat(vec3 a, float t) : v(_v), x(v.x), y(v.y), z(v.z), w(v0) { _theta = t * 0.5f; _axis = a; sin_t_half = sinf(_theta); v0 = cosf(_theta); v = a * sin_t_half; }

float quat::theta() const { return _theta; } void quat::theta(float t) { t *= 0.5f; _theta = t; sin_t_half = sinf(t); v0 = cosf(t); v = _axis * sin_t_half; }
vec3 quat::axis() const { return _axis; } void quat::axis(vec3 a) { _axis = a; v = a * sin_t_half; }

quat quat::operator+(const quat& other) {} quat quat::operator-(const quat& other) {}
quat quat::operator*(float f) {} quat quat::operator/(float f) {}

quat quat::operator*(const quat& other) const { return quat(w * other.w - vec3::dot(v, other.v), other.v * w + v * other.w + vec3::cross(v, other.v)); }
vec4 quat::operator*(const vec4& v) const { quat q = quat(v.x, v.y, v.z, v.w); q = *this * q; return vec4(q.x, q.y, q.z, q.w); }

quat quat::pow(const quat& q, int e) { return quat::rotation(q.theta() * e, q.axis()); }
quat quat::inverse(const quat& q) { return quat(q.w, q.v * -1); }

quat quat::slerp(const quat& a, const quat& b, float t) { return quat::pow(b * quat::inverse(a), t) * a; }
quat quat::rotation(float theta, vec3 axis) { return quat(axis, theta); }
quat quat::point(float x, float y, float z) { return quat(x, y, z, 0); }