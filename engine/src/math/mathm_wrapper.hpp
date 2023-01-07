#include "mathm.h"

LIB_INLINE vec4 operator*(vec4 a, vec4 b)
{
	return vec4_mul(a, b);
}

LIB_INLINE vec4 operator+(vec4 a, vec4 b)
{
	return vec4_add(a, b);
}

LIB_INLINE vec4 operator-(vec4 a, vec4 b)
{
	return vec4_sub(a, b);
}

LIB_INLINE vec3 operator*(vec3 a, vec3 b)
{
	return vec3_mul(a, b);
}

LIB_INLINE vec3 operator+(vec3 a, vec3 b)
{
	return vec3_add(a, b);
}

LIB_INLINE vec3 operator-(vec3 a, vec3 b)
{
	return vec3_sub(a, b);
}

LIB_INLINE void operator*=(vec3& a, vec3 b)
{
	a = vec3_mul(a, b);
}

LIB_INLINE void operator*=(vec3& a, f32 b)
{
	a = vec3_mul_scalar(a, b);
}

LIB_INLINE vec3 operator*(vec3 a, f32 b)
{
	return vec3_mul_scalar(a, b);
}

LIB_INLINE vec3 operator*(f32 b, vec3 a)
{
	return vec3_mul_scalar(a, b);
}

LIB_INLINE void operator+=(vec3& a, vec3 b)
{
	a = vec3_add(a, b);
}

LIB_INLINE void operator-=(vec3& a, vec3 b)
{
	a = vec3_sub(a, b);
}

LIB_INLINE bool operator==(vec3 a, vec3 b)
{
	return vec3_compare(a, b, 0.01f);
}

LIB_INLINE vec2 operator-(vec2 a, vec2 b)
{
	return vec2_sub(a, b);
}

LIB_INLINE mat4 operator*(mat4 a, mat4 b)
{
	return mat4_mul(a, b);
}

LIB_INLINE mat4 mat4_cast(quat q)
{
	return quat_to_mat4(q);
}

LIB_INLINE vec3 cross(vec3 a, vec3 b)
{
	return vec3_cross(a, b);
}

LIB_INLINE vec3 normalize(vec3 a)
{
	return vec3_normalized(a);
}

LIB_INLINE quat normalize(quat a)
{
	return quat_normalize(a);
}

LIB_INLINE quat quat_lookAt(vec3 pos, vec3 target, vec3 up)
{
	return quat_look_at(pos, target, up);
}

LIB_INLINE f32 length(vec3 a)
{
	return vec3_length(a);
}

LIB_INLINE mat4 translate(mat4 start_m, vec3 pos)
{
	return start_m * mat4_translation(pos);
}

LIB_INLINE vec3 operator-(vec3 a)
{
	return vec3_mul_scalar(a, -1.0f);
}

LIB_INLINE mat4 yawPitchRoll(f32 yaw, f32 pitch, f32 roll)
{
	f32 tmp_ch = math_cos(yaw);
	f32 tmp_sh = math_sin(yaw);
	f32 tmp_cp = math_cos(pitch);
	f32 tmp_sp = math_sin(pitch);
	f32 tmp_cb = math_cos(roll);
	f32 tmp_sb = math_sin(roll);

	mat4 Result;
	Result.data[0] = tmp_ch * tmp_cb + tmp_sh * tmp_sp * tmp_sb;
	Result.data[1] = tmp_sb * tmp_cp;
	Result.data[2] = -tmp_sh * tmp_cb + tmp_ch * tmp_sp * tmp_sb;
	Result.data[3] = 0.0f;
	Result.data[4] = -tmp_ch * tmp_sb + tmp_sh * tmp_sp * tmp_cb;
	Result.data[5] = tmp_cb * tmp_cp;
	Result.data[6] = tmp_sb * tmp_sh + tmp_ch * tmp_sp * tmp_cb;
	Result.data[7] = 0.0f;
	Result.data[8] = tmp_sh * tmp_cp;
	Result.data[9] = -tmp_sp;
	Result.data[10] = tmp_ch * tmp_cp;
	Result.data[11] = 0.0f;
	Result.data[12] = 0.0f;
	Result.data[13] = 0.0f;
	Result.data[14] = 0.0f;
	Result.data[15] = 1.0f;
	return Result;
}

//vec3 vec3(f32 a)
//{
//	return vec3_create(a, a, a);
//}

LIB_INLINE vec3 vec3_create(f32 a)
{
	vec3 v;
	v.x = a;
	v.y = a;
	v.z = a;
	return v;
}