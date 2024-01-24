
class Tower : public Transform
{
public:
	Tower(Transform* transform, UINT indexNum);
	~Tower();

	void Update();
	void Render();
	void GUIRender();

	UINT GetIndex() { return index; }
	SphereCollider* GetCollider() { return collider; }
	void SetTarget(Enemy* target) { this->target = target; }
	bool isTarget() { return target; }

	void Rotate();
	void Attack();

private:
	SphereCollider* collider; //���ݹ����� ���� �浹ü
	Enemy* target = nullptr; // ���� Ÿ���� Ʈ������
	Lay* targetLay = nullptr;

	UINT index; // modelinstancing index
	float rotSpeed = 5;
	float targetDistance;
	float attackDelay = 2;
	float delay = 2;
};