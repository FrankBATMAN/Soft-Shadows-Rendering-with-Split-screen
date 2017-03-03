#pragma once
#include "BoundingBoxBase.h"
#include "../GraphicsCommon/LBBML_GRAPHICS_INTERFACE_EXPORTS.h"

struct SFrustumPlane
{
	glm::vec3 Center;
	glm::vec3 Normal;
};

class LBBML_GRAPHICS_DLL_EXPORTS CLBBMLGraphicsFrustumBB: public CLBBMLGraphicsBoundingBoxBase
{
public:
	CLBBMLGraphicsFrustumBB(void);
	virtual ~CLBBMLGraphicsFrustumBB(void);

	void createV(const std::vector<glm::vec3>& vVertices) override;

	const std::vector<SFrustumPlane>& getFrustumPlanes() const {return m_Planes;}

private:
	void __calculateFrustumPlane(int vPlaneIndex, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4);
	
	std::vector<SFrustumPlane> m_Planes;
};