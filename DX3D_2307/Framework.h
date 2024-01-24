#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define START_X 0
#define START_Y 0

#define CENTER_X (WIN_WIDTH * 0.5f)
#define CENTER_Y (WIN_HEIGHT * 0.5f)

#define MAX_FRAME 512
#define MAX_BONE 256
#define MAX_INSTANCE 128
#define MAX_LIGHT 10

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA Timer::Get()->GetElapsedTime()
#define KEY Keyboard::Get()
#define SCENE SceneManager::Get()
#define DIALOG ImGuiFileDialog::Instance()
#define CAM Environment::Get()->GetMainCamera()

#define FOR(n) for(int i = 0 ; i < n ; i++)

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <assert.h>
#include <functional>
#include <fstream>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//DirectXTex
#include <DirectXTex/DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")

//ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>
#pragma comment(lib, "ImGui.lib")

//Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#pragma comment(lib, "assimp-vc143-mtd.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::TriangleTests;

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;
typedef XMFLOAT4X4 Float4x4;
typedef function<void()> Event;
typedef function<void(void*)> ParamEvent;

//Framework Header
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/Keyboard.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/Utility.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/Font.h"

using namespace Utility;

#include "Framework/Device/Device.h"

#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/GeometryShader.h"
#include "Framework/Shader/HullShader.h"
#include "Framework/Shader/DomainShader.h"
#include "Framework/Shader/ComputeShader.h"

#include "Framework/Buffer/VertexLayouts.h"
#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/RawBuffer.h"
#include "Framework/Buffer/StructuredBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"

#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Render/DepthStencil.h"
#include "Framework/Render/RenderTarget.h"
#include "Framework/Render/GBuffer.h"

#include "Framework/State/SamplerState.h"
#include "Framework/State/RasterizerState.h"
#include "Framework/State/BlendState.h"
#include "Framework/State/DepthStencilState.h"

#include "Framework/Math/Vector3.h"
#include "Framework/Math/GameMath.h"
#include "Framework/Math/Transform.h"

#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelAnimatorInstancing.h"

#include "Framework/Collision/Collider.h"
#include "Framework/Collision/BoxCollider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/CapsuleCollider.h"

using namespace GameMath;

#include "Framework/Environment/Camera.h"
#include "Framework/Environment/Environment.h"
#include "Framework/Environment/Reflection.h"
#include "Framework/Environment/Refraction.h"
#include "Framework/Environment/Shadow.h"

//GameObject Header
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Quad.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Block.h"
#include "Objects/Basic/Cylinder.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Lay.h"

#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/SkyBox.h"

#include "Objects/Algorithm/Node.h"
#include "Objects/Algorithm/Heap.h"
#include "Objects/Algorithm/AStar.h"

#include "Objects/UI/ProgressBar.h"
#include "Objects/UI/Button.h"

#include "Objects/Particle/Particle.h"
#include "Objects/Particle/Sprite.h"
#include "Objects/Particle/Spark.h"
#include "Objects/Particle/Rain.h"
#include "Objects/Particle/Snow.h"
#include "Objects/Particle/ParticleSystem.h"

#include "Objects/Manager/DataManager.h"

#include "Objects/Game/Car.h"
#include "Objects/Game/Coin.h"
#include "Objects/Game/CoinManager.h"
#include "Objects/Game/Enemy.h"
#include "Objects/Game/EnemyManager.h"
#include "Objects/Game/Box.h"
#include "Objects/Game/BoxManager.h"
#include "Objects/Game/MineCraftUI.h"
#include "Objects/Game/Sword.h"
#include "Objects/Game/Bot.h"
#include "Objects/Game/Fox.h"
#include "Objects/Game/Item.h"
#include "Objects/Game/Bullet.h"
#include "Objects/Game/BulletManager.h"
#include "Objects/Game/Wall.h"
#include "Objects/Game/WallManager.h"
#include "Objects/Game/Tower.h"
#include "Objects/Game/TowerManager.h"
#include "Objects/Game/Player.h"

//Scene Header
#include "Scenes/Scene.h"
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"

extern HWND hWnd;
extern Vector3 mousePos;

