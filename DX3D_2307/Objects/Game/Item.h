#pragma once

class Item : public Model
{
public:
	Item(string file);
	~Item();

	void Update();
	void Render();
	void GUIRender();

	ItemData* GetData() { return &data; }
	void SetData(ItemData data) { this->data = data; }

private:
	ItemData data;
};