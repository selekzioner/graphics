#pragma once
#include "Object.h"

class Rect final : public Object
{
public:
	explicit Rect(std::string&& name);

	GLfloat width = 0.5f, height = 20.f;

protected:
	bool setMesh() override;
};

