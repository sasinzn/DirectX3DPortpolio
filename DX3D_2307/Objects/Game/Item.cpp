#include "Framework.h"

Item::Item(string file)
	:Model(file)
{

}

Item::~Item()
{
}

void Item::Update()
{
	UpdateWorld();
}

void Item::Render()
{
	if (!isActive) return;

	Model::Render();
}

void Item::GUIRender()
{
	Model::GUIRender();
}
