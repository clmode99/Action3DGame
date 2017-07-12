/*-------------------------------------
アクションゲーム本体(ActionGame.cpp)

date  :2017.07.06
Author:Miyu Hara
-------------------------------------*/
/* インクルードファイル */
#include "ActionGame.h"

/* 名前空間の使用 */
using namespace CloverLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/* メンバ関数の定義 */
/*-----------------------------------------------------
GetInstance

summary:インスタンス取得
param  :なし(void)
return :インスタンス(std::unique_ptr<Game>&)
-----------------------------------------------------*/
unique_ptr<ActionGame>& ActionGame::GetInstance()
{
	static unique_ptr<ActionGame> instance(new ActionGame());
	return instance;
}

/*-----------------------------------------------------
~ActionGame

summary:デストラクタ
param  :存在しない
return :存在しない
-----------------------------------------------------*/
ActionGame::~ActionGame()
{
}

/*-----------------------------------------------------
Initialize

summary:初期化
param  :ウインドウハンドル(HWND)、幅(int)、高さ(int)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Initialize(HWND window, int width, int height)
{
	Game::Initialize(window, width, height);

	// TODO:初期化
	assert(this->InitCamera(width, height));

	this->input_ = make_unique<MyKeyboard>();

	/* モデル初期化 */
	this->skydome_ = Model3D::Create(L"Resources/Skydome.cmo");
	this->skydome_->SetLighting(false);
	this->skydome_->SetTranslation(Vector3::Zero);

	this->grass_ = Model3D::Create(L"Resources/Grass.cmo");
	this->grass_->SetTranslation(Vector3::Zero);
}

/*-----------------------------------------------------
Update

summary:更新
param  :(const DX::StepTimer&)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Update(DX::StepTimer const & timer)
{
	// TODO:更新
	this->input_->Update();

	// 仮処理
	const float ROTATE_RADIAN = XMConvertToRadians(3.0f);
	if (this->input_->IsLeft())
		this->camera_->SetRotateY(ROTATE_RADIAN);

	if (this->input_->IsRight())
		this->camera_->SetRotateY(-ROTATE_RADIAN);

	this->camera_->Update();
}

/*-----------------------------------------------------
Render

summary:描画
param  :なし(void)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Render()
{
	Clear();

	// TODO:描画
	this->skydome_->Draw();
	this->grass_->Draw();

	Present();
}

/*-----------------------------------------------------
ActionGame

summary:コンストラクタ
param  :なし(void)
return :存在しない
-----------------------------------------------------*/
ActionGame::ActionGame()
	:camera_(nullptr),
	 input_(nullptr),
	 skydome_(nullptr)
{}

/*-----------------------------------------------------
InitCamera

summary:カメラ初期化
param  :幅(int)、高さ(int)
return :成功(true)、失敗(false)
-----------------------------------------------------*/
bool ActionGame::InitCamera(int width, int height)
{
	ViewMaterial vm;
	vm.eye    = Vector3::Up;
	vm.target = Vector3(0.0f, 0.0f, 5.0f);
	vm.up     = Vector3::Up;
	ProjectionMaterial pm;
	pm.fov_radians = XMConvertToRadians(60.0f);
	pm.aspect      = static_cast<float>(width) / height;
	pm.near_clip   = 0.1f;
	pm.far_clip    = 500.0f;

	this->camera_ = make_unique<Camera>(vm, pm);

	return (this->camera_ ? true : false);
}
