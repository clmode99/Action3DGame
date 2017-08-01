/*-------------------------------------
アクションゲーム本体(ActionGame.cpp)

date  :2017.07.06
Author:Miyu Hara
-------------------------------------*/
/* インクルードファイル */
#include "ActionGame.h"
#include "../CloverLib/Font.h"

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
	static unique_ptr<ActionGame> s_instance(new ActionGame());
	return s_instance;
}

/*-----------------------------------------------------
~ActionGame

summary:デストラクタ
param  :存在しない
return :存在しない
-----------------------------------------------------*/
ActionGame::~ActionGame()
{}

/*-----------------------------------------------------
Initialize

summary:初期化
param  :ウインドウハンドル(HWND)、幅(int)、高さ(int)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Initialize(HWND window, int width, int height)
{
	Game::Initialize(window, width, height);

	this->sprite_batch_ = make_unique<SpriteBatch>(this->m_d3dContext.Get());
	this->InitCamera(width, height);

	/* 入力系初期化 */
	if (!(this->input_->IsConnected()))
		this->input_ = MyKeyboard::GetInstance();

	/* オブジェクト初期化 */
	this->skydome_ = Model3D::Create(L"Resources/Skydome.cmo");

	this->ground_ = Model3D::Create(L"Resources/Grass.cmo");

	this->player_ = make_unique<Player>();
	this->player_->SetColor(Vector3(0x33, 0x99, 0xff));

	// 敵１
	auto enemy_pos   = Vector3(-20.0f, 8.0f, 5.0f);
	auto enemy_scale = 2.0f;
	auto enemy_color = Vector3(0xee, 0xee, 0xee);
	this->CreateEnemy(enemy_pos, enemy_scale, enemy_color);

	// 敵２
	enemy_pos   = Vector3(10.0f, 20.0f, -20.0f);
	enemy_scale = 1.0f;
	this->CreateEnemy(enemy_pos, enemy_scale, enemy_color);

	// 敵３
	enemy_pos   = Vector3(60.0f, 3.0f, 35.0f);
	enemy_scale = 5.0f;
	this->CreateEnemy(enemy_pos, enemy_scale, enemy_color);
}

/*-----------------------------------------------------
Update

summary:更新
param  :(const DX::StepTimer&)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Update(DX::StepTimer const & timer)
{
	this->player_->ResetMove();

	this->Input();

	this->FixCameraHeight();

	/* 更新 */
	for (auto& v : this->enemy_)
		v->Update();
	this->player_->Update();
	this->camera_->Update();

	this->Collision();
}

/*-----------------------------------------------------
Render

summary:描画
param  :なし(void)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Render()
{
	this->Clear();

	this->skydome_->Draw();
	this->ground_ ->Draw();
	this->player_ ->Draw();
	for (auto& v : this->enemy_)
		v->Draw();

	this->DrawDebugFont();
	if (this->enemy_.empty())
	{
		const wchar_t* FONT_FILE = L"Assets/azuki_14.spritefont";
		auto font = make_unique<Font>(FONT_FILE);
		font->SetPosition(400, 300);
		font->Print(L"GAME CLEAR");
	}

	this->Present();
}

/*-----------------------------------------------------
ActionGame

summary:コンストラクタ
param  :なし(void)
return :存在しない
-----------------------------------------------------*/
ActionGame::ActionGame()
	:camera_(nullptr),
	 input_(MyGamePad::GetInstance()),
	 skydome_(nullptr),
	 is_debug_mode_(false)
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
	vm.eye    = Vector3(0.0f, 3.0f, 40.0f);
	vm.target = Vector3(0.0f, 3.0f, 0.0f);		// Player.h - FIRST_POSITIONと同じ(なんとかしたい)
	vm.up     = Vector3::Up;
	ProjectionMaterial pm;
	pm.fov_radians = XMConvertToRadians(60.0f);
	pm.aspect      = static_cast<float>(width) / height;
	pm.near_clip   = 0.1f;
	pm.far_clip    = 250.0f;

	this->camera_ = make_unique<Camera>(vm, pm);

	return (this->camera_ ? true : false);
}

/*-----------------------------------------------------
FixCameraHeight

summary:カメラの高さを固定
param  :なし(void)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::FixCameraHeight()
{
	Vector3 player_pos = this->player_->GetPosition();
	player_pos.y = 3.0f;			// カメラの高さ
	this->camera_->RotateY(player_pos, 0.0f);
}

/*----------------------------------------------------------------
CreateEnemy

summary:敵を作成
param  :座標(const DirectX::SimpleMath::Vector3&)、大きさ(float)、
		色(const DirectX::SimpleMath::Vector3&)
return :なし(void)
----------------------------------------------------------------*/
void ActionGame::CreateEnemy(const Vector3& pos, float scale, const Vector3& color)
{
	auto enemy = make_unique<Enemy>(pos, scale, color);
	this->enemy_.push_back(move(enemy));
}

/*----------------------------------------------------------------
Input

summary:入力処理
param  :なし(void)
return :なし(void)
----------------------------------------------------------------*/
void ActionGame::Input()
{
	const auto ROTATE_RADIAN = XMConvertToRadians(2.0f);
	const auto SPEED         = 0.3f;
	const auto JUMP_SPEED    = 2.0f;

	this->input_->Update();

	if (this->input_->IsLeft())		this->Turn(ROTATE_RADIAN);
	if (this->input_->IsRight())	this->Turn(-ROTATE_RADIAN);
	if (this->input_->IsUp())		this->Move(-SPEED, -SPEED);
	if (this->input_->IsDown())		this->Move(SPEED, -SPEED);

	if (this->input_->IsControlLeft())	this->camera_->RotateY(this->player_->GetPosition(), ROTATE_RADIAN);
	if (this->input_->IsControlRight())	this->camera_->RotateY(this->player_->GetPosition(), -ROTATE_RADIAN);

	if (this->input_->IsJump() && !this->player_->IsJump())		this->player_->Jump(JUMP_SPEED);

	if (this->input_->IsDebug())	this->ChangeDebugMode();
}

/*----------------------------------------------------------------
Collision

summary:当たり判定
param  :なし(void)
return :なし(void)
----------------------------------------------------------------*/
void ActionGame::Collision()
{
	for (auto itr = this->enemy_.begin(); itr != this->enemy_.end();)
	{
		if ((*itr)->IsCollision(this->player_))
		{
			/* 当たったので消す */
			(*itr).reset();
			itr = this->enemy_.erase(itr);
		}
		else
			++itr;
	}
}

/*-----------------------------------------------------
DrawDebugMode

summary:デバッグモード時の描画
param  :なし(void)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::DrawDebugFont()
{
	if(!this->is_debug_mode_)
		return;

	const wchar_t* FONT_FILE = L"Assets/azuki_14.spritefont";
	auto msg = make_unique<Font>(FONT_FILE);
	msg->Print(L"Debug Mode");
}

/*-----------------------------------------------------
Turn

summary:旋回
param  :旋回する角度(float)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Turn(float angle_radian)
{
	this->player_->Turn(angle_radian);
}

/*-----------------------------------------------------
Move

summary:移動
param  :プレイヤーの速さ(float)、カメラの速さ(float)
return :なし(void)
-----------------------------------------------------*/
void ActionGame::Move(float player_spd,float camera_spd)
{
	this->player_->Move(player_spd);		// ※カメラ設定よりも先に処理する

	Vector3 player_dir = this->player_->GetDirection();
	this->camera_->Move(-player_dir, camera_spd);
}

/*----------------------------------------------------------------
ChangeDebugMode

summary:デバッグモード切替
param  :なし(void)
return :なし(void)
----------------------------------------------------------------*/
void ActionGame::ChangeDebugMode()
{
	this->is_debug_mode_ ^= 1;		// 反転

	this->player_->SetVisibleCollision(this->is_debug_mode_);
	for (auto& v : this->enemy_)
		v->SetVisibleCollision(this->is_debug_mode_);
}
