#pragma once
#include <GLM/glm.hpp>

namespace  LBBMLGraphicsStructure
{
	struct STransform
	{
		glm::vec3 m_Scale;
		glm::vec3 m_Position;
		glm::vec3 m_RotAxis;
		float m_RotAngle;
	};

	struct SFrustumPlane
	{
		glm::vec3 Center;
		glm::vec3 Normal;
	};

	struct SOBBAxis
	{
		glm::vec3 X_Axis;
		glm::vec3 Y_Axis;
		glm::vec3 Z_Axis;
	};

	struct SOBBHalfLength
	{
		float X_HalfLength;
		float Y_HalfLength;
		float Z_HalfLength;
	};

	struct SVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct SPerspectiveFrustum
	{
		float Fovy;
		float Aspect;
		float ZNear;
		float ZFar;
	};
}