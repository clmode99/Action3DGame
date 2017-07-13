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
	this->InitCamera(width, height);
	this->input_ = make_unique<MyKeyboard>();		// とりあえずキーボードで

	/* モデル初期化 */
	this->skydome_ = Model3D::Create(L"Resources/Skydome.cmo");
	this->skydome_->SetLighting(false);

	this->grass_ = Model3D::Create(L"Resources/Grass.cmo");

	this->player_ = make_unique<Player>();

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
	const float ROTATE_RADIAN = XMConvertToRadians(2.0f);
	const float SPEED  = 0.15f;

	if (this->input_->IsLeft())		this->Turn(ROTATE_RADIAN);
	if (this->input_->IsRight())	this->Turn(-ROTATE_RADIAN);
	if (this->input_->IsUp())		this->Move(-SPEED, -SPEED);
	if (this->input_->IsDown())		this->Move(SPEED, -SPEED);
	if (this->input_->IsControlLeft())	this->camera_->RotateY(this->player_->GetPosition(),ROTATE_RADIAN);
	if (this->input_->IsControlRight())	this->camera_->RotateY(this->player_->GetPosition(),-ROTATE_RADIAN);

	this->player_->Update();
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
	this->player_->Draw();

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
	vm.eye    = Vector3(0.0f, 2.0f, 20.0f);
	vm.target = Vector3(0.0f, 2.0f, 0.0f);
	vm.up     = Vector3::Up;
	// 上から見下ろすカメラにするときは下のコメント外す
	//vm.eye = Vector3(0.0f, 50.0f, 0.0f);
	//vm.target = Vector3::Zero;
	//vm.up = Vector3(0.0f, 0.0f, -1.0f);
	ProjectionMaterial pm;
	pm.fov_radians = XMConvertToRadians(60.0f);
	pm.aspect      = static_cast<float>(width) / height;
	pm.near_clip   = 0.1f;
	pm.far_clip    = 500.0f;

	this->camera_ = make_unique<Camera>(vm, pm);

	return (this->camera_ ? true : false);
}

void ActionGame::Turn(float angle_radian)
{
	this->player_->Turn(angle_radian);
}

void ActionGame::Move(float player_spd,float camera_spd)
{
	this->player_->Move(player_spd);		// カメラ設定よりも先に処理する

	Vector3 player_dir = this->player_->GetDirection();
	this->camera_->Move(-player_dir, camera_spd);
}
