/*
 * Copyright (C) 2013-2015 Sergey Kosarevsky (sk@linderdaum.com)
 * Copyright (C) 2013-2015 Viktor Latypov (vl@linderdaum.com)
 * Based on Linderdaum Engine http://www.linderdaum.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must display the names 'Sergey Kosarevsky' and
 *    'Viktor Latypov' in the credits of the application, if such credits exist.
 *    The authors of this work must be notified via email (sk@linderdaum.com) in
 *    this case of redistribution.
 *
 * 3. Neither the name of copyright holders nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <numeric>

#include "math/mathm_wrapper.hpp"

// #include <glm/glm.hpp>
// #include <glm/ext.hpp>

/// Virtual trackball for user interaction with rotations
class VirtualTrackball
{
public:
	VirtualTrackball() = default;

	/**
		Get rotation matrix for new mouse point
	**/
	mat4 dragTo(vec2 screenPoint, float speed, bool keyPressed)
	{
		if (keyPressed && !isDraggingActive_)
		{
			startDragging(screenPoint);

			isDraggingActive_ = keyPressed;

			return mat4_identity();
		}

		isDraggingActive_ = keyPressed;

		if (!keyPressed) return mat4_identity();

		pointCur_ = projectOnSphere(screenPoint);

		const vec3 direction = vec3_sub(pointCur_, pointPrev_);
		const float shift = vec3_length(direction);

		mat4 rotMatrix = mat4_identity();

		if (shift > std::numeric_limits<float>::epsilon())
		{
			const vec3 axis = vec3_cross( pointPrev_, pointCur_);
      rotMatrix = quat_to_mat4(quat_from_axis_angle(axis, shift * speed, false));
			// rotMatrix = glm::rotate(mat4_identity(), shift * speed, axis);
		}

		rotationDelta_ = rotMatrix;

		return rotMatrix;
	}

	const mat4& getRotationDelta() const
	{
		return rotationDelta_;
	};

	/**
		Get current rotation matrix
	**/
	mat4 getRotationMatrix() const
	{
		return rotation_ * rotationDelta_;// mat4_mul(rotation_, rotationDelta_);
	}

private:
	void startDragging(vec2 screenPoint)
	{
		rotation_ = rotation_ * rotationDelta_;// mat4_mul(rotation_, rotationDelta_);
		rotationDelta_ = mat4_identity();
		pointCur_ = projectOnSphere(screenPoint);
		pointPrev_ = pointCur_;		
	}

	vec3 projectOnSphere(vec2 ScreenPoint)
	{
		// convert to -1.0...1.0 range
		vec3 proj = vec3_create(
			+(2.0f * ScreenPoint.x - 1.0f),
			-(2.0f * ScreenPoint.y - 1.0f),
			0.0f
		);

		const float Length = MIN(vec3_length(proj), 1.0f);

		proj.z = sqrtf(1.001f - Length * Length);
    
		return vec3_normalized(proj);
	}
	vec3 pointCur_ = vec3_zero();
	vec3 pointPrev_ = vec3_zero();
	mat4 rotation_ = mat4_identity();
	mat4 rotationDelta_ = mat4_identity();
	bool isDraggingActive_ = false;
};
