#pragma once
#include "Object.h"

class Rect final : public Object
{
public:
	explicit Rect(std::string&& name);

	GLfloat width = 1.f, height = 1000.f;

protected:
	bool setMesh() override;
};

