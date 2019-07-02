#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <d3d11.h>
#include <SimpleMath.h>

#pragma once
class Light
{
public:
	Light();
	~Light();

	void SetAmbientColor(float red, float green, float blue, float alpha);
	void SetDiffuseColor(float red, float green, float blue, float alpha);
	void SetDirection(float x, float y, float z);

	DirectX::SimpleMath::Vector4 GetAmbientColor() const { return m_ambientColor; }
	DirectX::SimpleMath::Vector4 GetDiffuseColor() const { return m_diffuseColor; }
	DirectX::SimpleMath::Vector3 GetDirection() const { return m_direction; }

private:
	DirectX::SimpleMath::Vector4 m_ambientColor;
	DirectX::SimpleMath::Vector4 m_diffuseColor;
	DirectX::SimpleMath::Vector3 m_direction;
};

#endif