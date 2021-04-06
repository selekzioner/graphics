#pragma once

#pragma once

#include <string>
#include "Vertex.h"

enum class MLResult { mlOK = 1, mlFAIL = 0 };

class MeshLoader final {
public:
	static MLResult Load(const std::string& path, Vertices& vertices, Indices& indices);
};