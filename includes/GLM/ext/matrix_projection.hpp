/// @ref ext_matrix_projection
/// @file glm/ext/matrix_projection.hpp
///
/// @defgroup ext_matrix_projection GLM_EXT_matrix_projection
/// @ingroup ext
///
/// Functions that generate common projection transformation matrices.
///
/// The matrices generated by this extension use standard OpenGL fixed-function
/// conventions. For example, the lookAt function generates a transform from world
/// space into the specific eye space that the projective matrix functions
/// (perspective, ortho, etc) are designed to expect. The OpenGL compatibility
/// specifications defines the particular layout of this eye space.
///
/// Include <glm/ext/matrix_projection.hpp> to use the features of this extension.
///
/// @see ext_matrix_transform
/// @see ext_matrix_clip_space

#pragma once

// Dependencies
#include "GLM/gtc/constants.hpp"
#include "GLM/geometric.hpp"
#include "GLM/trigonometric.hpp"
#include "GLM/matrix.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_EXT_matrix_projection extension included")
#endif

namespace glm
{
	/// @addtogroup ext_matrix_projection
	/// @{

	/// Map the specified object coordinates (obj.x, obj.y, obj.z) into window coordinates.
	/// The near and far clip planes correspond to z normalized device coordinates of 0 and +1 respectively. (Direct3D clip volume definition)
	///
	/// @param obj Specify the object coordinates.
	/// @param model Specifies the current modelview matrix
	/// @param proj Specifies the current projection matrix
	/// @param viewport Specifies the current viewport
	/// @return Return the computed window coordinates.
	/// @tparam T Native type used for the computation. Currently supported: half (not recommended), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	///
	/// @see <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluProject.xml">gluProject man page</a>
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> projectZO(
		vec<3, T, Q> const& obj, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	/// Map the specified object coordinates (obj.x, obj.y, obj.z) into window coordinates.
	/// The near and far clip planes correspond to z normalized device coordinates of -1 and +1 respectively. (OpenGL clip volume definition)
	///
	/// @param obj Specify the object coordinates.
	/// @param model Specifies the current modelview matrix
	/// @param proj Specifies the current projection matrix
	/// @param viewport Specifies the current viewport
	/// @return Return the computed window coordinates.
	/// @tparam T Native type used for the computation. Currently supported: half (not recommended), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	///
	/// @see <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluProject.xml">gluProject man page</a>
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> projectNO(
		vec<3, T, Q> const& obj, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	/// Map the specified object coordinates (obj.x, obj.y, obj.z) into window coordinates using default near and far clip planes definition.
	/// To change default near and far clip planes definition use GLM_FORCE_DEPTH_ZERO_TO_ONE.
	///
	/// @param obj Specify the object coordinates.
	/// @param model Specifies the current modelview matrix
	/// @param proj Specifies the current projection matrix
	/// @param viewport Specifies the current viewport
	/// @return Return the computed window coordinates.
	/// @tparam T Native type used for the computation. Currently supported: half (not recommended), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	///
	/// @see <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluProject.xml">gluProject man page</a>
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> project(
		vec<3, T, Q> const& obj, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	/// Map the specified window coordinates (win.x, win.y, win.z) into object coordinates.
	/// The near and far clip planes correspond to z normalized device coordinates of 0 and +1 respectively. (Direct3D clip volume definition)
	///
	/// @param win Specify the window coordinates to be mapped.
	/// @param model Specifies the modelview matrix
	/// @param proj Specifies the projection matrix
	/// @param viewport Specifies the viewport
	/// @return Returns the computed object coordinates.
	/// @tparam T Native type used for the computation. Currently supported: half (not recommended), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	///
	/// @see <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluUnProject.xml">gluUnProject man page</a>
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> unProjectZO(
		vec<3, T, Q> const& win, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	/// Map the specified window coordinates (win.x, win.y, win.z) into object coordinates.
	/// The near and far clip planes correspond to z normalized device coordinates of -1 and +1 respectively. (OpenGL clip volume definition)
	///
	/// @param win Specify the window coordinates to be mapped.
	/// @param model Specifies the modelview matrix
	/// @param proj Specifies the projection matrix
	/// @param viewport Specifies the viewport
	/// @return Returns the computed object coordinates.
	/// @tparam T Native type used for the computation. Currently supported: half (not recommended), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	///
	/// @see <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluUnProject.xml">gluUnProject man page</a>
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> unProjectNO(
		vec<3, T, Q> const& win, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	/// Map the specified window coordinates (win.x, win.y, win.z) into object coordinates using default near and far clip planes definition.
	/// To change default near and far clip planes definition use GLM_FORCE_DEPTH_ZERO_TO_ONE.
	///
	/// @param win Specify the window coordinates to be mapped.
	/// @param model Specifies the modelview matrix
	/// @param proj Specifies the projection matrix
	/// @param viewport Specifies the viewport
	/// @return Returns the computed object coordinates.
	/// @tparam T Native type used for the computation. Currently supported: half (not recommended), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	///
	/// @see <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluUnProject.xml">gluUnProject man page</a>
	template<typename T, typename U, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> unProject(
		vec<3, T, Q> const& win, mat<4, 4, T, Q> const& model, mat<4, 4, T, Q> const& proj, vec<4, U, Q> const& viewport);

	/// Define a picking region
	///
	/// @param center Specify the center of a picking region in window coordinates.
	/// @param delta Specify the width and height, respectively, of the picking region in window coordinates.
	/// @param viewport Rendering viewport
	/// @tparam T Native type used for the computation. Currently supported: half (not recommended), float or double.
	/// @tparam U Currently supported: Floating-point types and integer types.
	///
	/// @see <a href="https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPickMatrix.xml">gluPickMatrix man page</a>
	template<typename T, qualifier Q, typename U>
	GLM_FUNC_DECL mat<4, 4, T, Q> pickMatrix(
		vec<2, T, Q> const& center, vec<2, T, Q> const& delta, vec<4, U, Q> const& viewport);

	/// @}
}//namespace glm

#include "matrix_projection.inl"
