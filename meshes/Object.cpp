#include "Object.h"
#include "MeshLoader.h"
#include "TextureLoader.h"

Object::Object(std::string&& name)
	: _name(name)
{
}

void Object::Initialize()
{
	if (!setMesh()) {
		return;
	}
	_mesh.Initialize();

	TextureLoader::Load(std::string(
		"Textures/" + _name + ".jpg"), _mesh.texture, QOpenGLTexture::Nearest);

	TextureLoader::Load(std::string(
		"Textures/" + _name + "_NormalMap.jpg"), _mesh.normalMap, QOpenGLTexture::Nearest);
}

void Object::Render(QOpenGLShaderProgram& program)
{
	_modelMatrix.rotate(_rotationParam.w(), 
		_rotationParam.x(), _rotationParam.y(), _rotationParam.z());
	
	program.setUniformValue("normalMatrix", _modelMatrix.inverted().transposed());
	program.setUniformValue("model", _modelMatrix);
	
	_mesh.Render(program);
}

void Object::SetPos(const QVector3D& pos)
{
	_pos = pos;
	_modelMatrix.translate(_pos);
}

void Object::SetRotation(const QVector4D& rot)
{
	_rotationParam = rot;
}

void Object::Rotate(const QVector4D& rot)
{
	_modelMatrix.rotate(rot.w(), rot.x(), rot.y(), rot.z());
}

QVector3D Object::GetPos() const
{
	return _pos;
}

std::string Object::Name() const
{
	return _name;
}

bool Object::setMesh()
{
	if (MeshLoader::Load(std::string("Meshes/" + _name + ".obj"),
		_mesh.vertices, _mesh.indices/*, _mesh.mode*/) == MLResult::mlFAIL) {
		return false;
	}
	return true;
}

