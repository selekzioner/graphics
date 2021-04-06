#include "Rect.h"

Rect::Rect(std::string&& name)
	: Object(std::move(name))
{
}

bool Rect::setMesh()
{
  const auto texWidth = 1.f, texHeight = 20.f;
	
  Vertex v;
  v.normal = { 0,1,0 };

  v.pos = { -width,0,height };
  v.texCoord = { texWidth,0 };
  _mesh.vertices.push_back(v);

  v.pos = { width,0,height };
  v.texCoord = { 0,0 };
  _mesh.vertices.push_back(v);

  v.pos = { -width,0,-height };
  v.texCoord = { texWidth,texHeight };
  _mesh.vertices.push_back(v);

  v.pos = { -width,0,-height };
  v.texCoord = { texWidth,texHeight };
  _mesh.vertices.push_back(v);

  v.pos = { width,0,height };
  v.texCoord = { 0,0 };
  _mesh.vertices.push_back(v);

  v.pos = { width,0,-height };
  v.texCoord = { 0,texHeight };
  _mesh.vertices.push_back(v);

	for (auto i = 0u; i < _mesh.vertices.size(); ++i) {
    _mesh.indices.push_back(i);
	}

  return true;
}
