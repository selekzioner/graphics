#include "Rect.h"

Rect::Rect(std::string&& name)
	: Object(std::move(name))
{
}

bool Rect::setMesh()
{
  const auto stepCountW = static_cast<unsigned>(width * 10),
						 stepCountH = static_cast<unsigned>(height * 10);
	
  const auto stepW = width / static_cast<GLfloat>(stepCountW),
						 stepH = height / static_cast<GLfloat>(stepCountH);

  const auto texStep = 0.02f;
	auto texCoordX = 0.f, texCoordY = 0.f;
  auto xTexDir = 1, yTexDir = 1;

  for (unsigned j = 0; j <= stepCountH; ++j) {
    for (unsigned i = 0; i <= stepCountW; ++i) {
      texCoordX += texStep * xTexDir * i;
    	if (texCoordX > 1.f) {
        texCoordX = 1.f - texStep;
        xTexDir = -1;
    	}
      if (texCoordX < 0.f) {
        texCoordX = 0.f;
        xTexDir = 1;
      }
      texCoordY += texStep * yTexDir * j;
      if (texCoordY > 1.f) {
        texCoordY = 1.f - texStep;
        yTexDir = -1;
      }
      if (texCoordY < 0.f) {
        texCoordY = 0.f;
        yTexDir = 1;
      }
    	
      _mesh.vertices.push_back({ { -width / 2 + stepW * i, -height / 2 + stepH * j, 0 },
														{ 0, 0, 1 }, { texCoordX, texCoordY}
																		});
    }
  }

  for (unsigned i = 0; i < stepCountW; ++i) {
    for (unsigned j = 0; j < stepCountH; ++j) {

      auto first = i + (j + 1) * (stepCountW + 1);
      auto second = i + j * (stepCountW + 1);

      _mesh.indices.push_back(first); _mesh.indices.push_back(second); _mesh.indices.push_back(second + 1);

      _mesh.indices.push_back(second + 1); _mesh.indices.push_back(first + 1); _mesh.indices.push_back(first);
    }
  }
  return true;
}
