#pragma once

#include "Mesh.h"

class Object;
using ObjectPtr = std::shared_ptr<Object>;

class Object {
public:
	explicit Object(std::string&& name);
	virtual ~Object() = default;
	
	void Initialize();
	void Render(QOpenGLShaderProgram& program);

	void SetPos(const QVector3D& pos);
	void SetRotation(const QVector4D& rot);
	void Rotate(const QVector4D& rot);

	QVector3D GetPos() const;
	std::string Name() const;

protected:
	virtual bool setMesh();
	
	Mesh _mesh;
	QVector3D _pos;
	QMatrix4x4 _modelMatrix;
	QVector4D _rotationParam;

private:
	std::string _name;
};

