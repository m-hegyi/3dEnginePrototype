#include "Light.h"

using namespace DirectX;

Light::Light()
{
}

Light::~Light()
{
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor.x = red;
	m_diffuseColor.y = green;
	m_diffuseColor.z = blue;
	m_diffuseColor.w = alpha;
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction.x = x;
	m_direction.y = y;
	m_direction.z = z;
}