#include "MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

MLResult MeshLoader::Load(const std::string& path, Vertices& vertices, Indices& indices)
{
  Assimp::Importer import;
  const auto* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    return MLResult::mlFAIL;
  }

  auto* mesh = scene->mMeshes[0];

  vertices.clear();
  indices.clear();

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex{};
    vertex.pos = { mesh->mVertices[i].x,
                   mesh->mVertices[i].y,
                   mesh->mVertices[i].z };

    vertex.normal = { mesh->mNormals[i].x,
                      mesh->mNormals[i].y,
                      mesh->mNormals[i].z };

    if (mesh->mTextureCoords[0]) {
      vertex.texCoord = { mesh->mTextureCoords[0][i].x,
                          mesh->mTextureCoords[0][i].y };
    }
    vertices.push_back(vertex);
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const auto face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }
  return MLResult::mlOK;
}

/*#include "MeshLoader.h"

#include <fstream>

MLResult MeshLoader::Load(const std::string& path, Vertices& vertices, Indices& indices, GLMode& mode)
{
  std::ifstream ifstream(path);
  if (!ifstream) {
    return MLResult::mlFAIL;
  }

	
	
  std::vector<QVector3D> aVertices, aNormals;
  std::vector<QVector2D> aTextures;
  auto isTexturesFirst = false;

  for (std::string line; std::getline(ifstream, line); ) {

    if (line.substr(0, 2) == "vn" && line[2] == ' ') {
      QVector3D n;
      std::string::size_type prevPos = 3u;
      auto nextPos = line.find_first_of(' ', prevPos);
      n.setX(std::stof(line.substr(prevPos, nextPos - prevPos)));

      prevPos = nextPos + 1;
      nextPos = line.find_first_of(' ', prevPos);
      n.setY(std::stof(line.substr(prevPos, nextPos - prevPos)));

      prevPos = nextPos + 1;
      nextPos = line.find_first_of(' ', prevPos);
      n.setZ(std::stof(line.substr(prevPos, nextPos - prevPos)));
    	
      aNormals.push_back(n);
    }
    else if (line.substr(0, 2) == "vt" && line[2] == ' ') {
      QVector2D t;
      std::string::size_type prevPos = 3u;
      auto nextPos = line.find_first_of(' ', prevPos);
      t.setX(std::stof(line.substr(prevPos, nextPos - prevPos)));

      prevPos = nextPos + 1;
      nextPos = line.find_first_of(' ', prevPos);
      t.setY(std::stof(line.substr(prevPos, nextPos - prevPos)));

      aTextures.push_back(t);
    }
    else if (line[0] == 'v' && line[1] == ' ') {
      QVector3D v;
      std::string::size_type prevPos = line[2] == ' ' ? 3u : 2u;
      auto nextPos = line.find_first_of(' ', prevPos);
      v.setX(std::stof(line.substr(prevPos, nextPos - prevPos)));

      prevPos = nextPos + 1;
      nextPos = line.find_first_of(' ', prevPos);
      v.setY(std::stof(line.substr(prevPos, nextPos - prevPos)));

      prevPos = nextPos + 1;
      nextPos = line.find_first_of(' ', prevPos);
      v.setZ(std::stof(line.substr(prevPos, nextPos - prevPos)));

      aVertices.push_back(v);
    }
    else if (line[0] == 'f' && line[1] == ' ') {
      std::string::size_type prevPos = 2;
      auto nextPos = prevPos - 1;
      auto counter = 0u;
    	
    	while(nextPos < line.length()) {
        prevPos = nextPos + 1;
        nextPos = line.find_first_of('/', prevPos);
        const auto vIdx = std::stoul(line.substr(prevPos, nextPos - prevPos)) - 1;

        prevPos = nextPos + 1;
        nextPos = line.find_first_of('/', prevPos);
        if (nextPos != prevPos) {
          const auto nIdx = std::stoul(line.substr(prevPos, nextPos - prevPos)) - 1;
        }

        prevPos = nextPos + 1;
        nextPos = line.find_first_of(' ', prevPos);
        const auto tIdx = std::stoul(line.substr(prevPos, nextPos - prevPos)) - 1;

        if (vIdx > aVertices.size() || nIdx > aNormals.size() || tIdx > aTextures.size()) {
          return MLResult::mlFAIL;
        }
        if (isTexturesFirst) {
          vertices.push_back({ aVertices[vIdx], aNormals[tIdx], aTextures[nIdx] });
        }
        else {
          vertices.push_back({ aVertices[vIdx], aNormals[nIdx], aTextures[tIdx] });
        }
        ++counter;
    	}
      mode = counter == 3 ? GLMode::TRIANGLES : GLMode::QUADS;
    }
    else if (line.substr(0, 3) == "s 1") {
      isTexturesFirst = true;
    }
  }
	
  for (auto i = 0u; i < vertices.size(); ++i) {
    indices.push_back(i);
  }
  return MLResult::mlOK;
}*/