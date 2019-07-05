#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include <d3d11.h>
#include <SimpleMath.h>

#pragma once
class Frustum
{
public:
	Frustum();
	~Frustum();

	void ConstructFrustum(float screenDepth, DirectX::SimpleMath::Matrix projectionMatrix, DirectX::SimpleMath::Matrix viewMatrix);

	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
private:

	DirectX::SimpleMath::Plane m_planes[6];
};


#endif 
