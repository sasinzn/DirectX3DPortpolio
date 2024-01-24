
class CoinManager : public Singleton<CoinManager>
{
public:
	CoinManager();
	~CoinManager();

	void Update();
	void Render();
	void GUIRender();

	void Create(UINT poolSize);
	void Spawn(Vector3 pos);
	bool Collision(Collider* playerItemGetCol, Collider* player);


private:
	ModelInstancing* model;
	UINT instanceIndex;

	vector<Coin*> coins;
};