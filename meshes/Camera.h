#pragma once
#include <QMatrix4x4>

class Camera final
{
public:
  QMatrix4x4 getViewMatrix() const;
	
  QVector3D pos   { 0.f,  0.f,  5.f },
						front { 0.f,  0.f, -1.f },
						up		{ 0.f,  1.f,  0.f };
  
};

