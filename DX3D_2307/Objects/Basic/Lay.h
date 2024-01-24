#pragma once

class Lay :public Transform
{
public:
	Lay();
	~Lay();

	void Update();
	void Render();

	void SetTarget(Transform* target) { this->target = target; MakeMesh(); mesh->CreateMesh();}

private:
	void MakeMesh();
private:
	Transform* target;

	Material* material;
	Mesh<VertexColor>* mesh;

	MatrixBuffer* worldBuffer;
};