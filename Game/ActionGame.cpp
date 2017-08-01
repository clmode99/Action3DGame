/*-------------------------------------
�A�N�V�����Q�[���{��(ActionGame.cpp)

date  :2017.07.06
Author:Miyu Hara
-------------------------------------*/
/* �C���N���[�h�t�@�C�� */
#include "ActionGame.h"
#include "../CloverLib/Font.h"

/* ���O��Ԃ̎g�p */
using namespace CloverLib;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/* �����o�֐��̒�` */
/*-----------------------------------------------------
GetInstance

summary:�C���X�^���X�擾
param  :�Ȃ�(void)
return :�C���X�^���X(std::unique_ptr<Game>&)
-----------------------------------------------------*/
unique_ptr<ActionGame>& ActionGame::GetInstance()
{
	static unique_ptr<ActionGame> s_instance(new ActionGame());
	return s_instance;
}

/*-----------------------------------------------------
~ActionGame

summary:�f�X�g���N�^
param  :���݂��Ȃ�
return :���݂��Ȃ�
-----------------------------------------------------*/
ActionGame::~ActionGame()
{}

/*-----------------------------------------------------
Initialize

summary:������
param  :�E�C���h�E�n���h��(HWND)�A��(int)�A����(int)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::Initialize(HWND window, int width, int height)
{
	Game::Initialize(window, width, height);

	this->sprite_batch_ = make_unique<SpriteBatch>(this->m_d3dContext.Get());
	this->InitCamera(width, height);

	/* ���͌n������ */
	if (!(this->input_->IsConnected()))
		this->input_ = MyKeyboard::GetInstance();

	/* �I�u�W�F�N�g������ */
	this->skydome_ = Model3D::Create(L"Resources/Skydome.cmo");

	this->ground_ = Model3D::Create(L"Resources/Grass.cmo");

	this->player_ = make_unique<Player>();
	this->player_->SetColor(Vector3(0x33, 0x99, 0xff));

	// �G�P
	auto enemy_pos   = Vector3(-20.0f, 8.0f, 5.0f);
	auto enemy_scale = 2.0f;
	auto enemy_color = Vector3(0xee, 0xee, 0xee);
	this->CreateEnemy(enemy_pos, enemy_scale, enemy_color);

	// �G�Q
	enemy_pos   = Vector3(10.0f, 20.0f, -20.0f);
	enemy_scale = 1.0f;
	this->CreateEnemy(enemy_pos, enemy_scale, enemy_color);

	// �G�R
	enemy_pos   = Vector3(60.0f, 3.0f, 35.0f);
	enemy_scale = 5.0f;
	this->CreateEnemy(enemy_pos, enemy_scale, enemy_color);
}

/*-----------------------------------------------------
Update

summary:�X�V
param  :(const DX::StepTimer&)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::Update(DX::StepTimer const & timer)
{
	this->player_->ResetMove();

	this->Input();

	this->FixCameraHeight();

	/* �X�V */
	for (auto& v : this->enemy_)
		v->Update();
	this->player_->Update();
	this->camera_->Update();

	this->Collision();
}

/*-----------------------------------------------------
Render

summary:�`��
param  :�Ȃ�(void)
return :�Ȃ�(void)
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

summary:�R���X�g���N�^
param  :�Ȃ�(void)
return :���݂��Ȃ�
-----------------------------------------------------*/
ActionGame::ActionGame()
	:camera_(nullptr),
	 input_(MyGamePad::GetInstance()),
	 skydome_(nullptr),
	 is_debug_mode_(false)
{}

/*-----------------------------------------------------
InitCamera

summary:�J����������
param  :��(int)�A����(int)
return :����(true)�A���s(false)
-----------------------------------------------------*/
bool ActionGame::InitCamera(int width, int height)
{
	ViewMaterial vm;
	vm.eye    = Vector3(0.0f, 3.0f, 40.0f);
	vm.target = Vector3(0.0f, 3.0f, 0.0f);		// Player.h - FIRST_POSITION�Ɠ���(�Ȃ�Ƃ�������)
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

summary:�J�����̍������Œ�
param  :�Ȃ�(void)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::FixCameraHeight()
{
	Vector3 player_pos = this->player_->GetPosition();
	player_pos.y = 3.0f;			// �J�����̍���
	this->camera_->RotateY(player_pos, 0.0f);
}

/*----------------------------------------------------------------
CreateEnemy

summary:�G���쐬
param  :���W(const DirectX::SimpleMath::Vector3&)�A�傫��(float)�A
		�F(const DirectX::SimpleMath::Vector3&)
return :�Ȃ�(void)
----------------------------------------------------------------*/
void ActionGame::CreateEnemy(const Vector3& pos, float scale, const Vector3& color)
{
	auto enemy = make_unique<Enemy>(pos, scale, color);
	this->enemy_.push_back(move(enemy));
}

/*----------------------------------------------------------------
Input

summary:���͏���
param  :�Ȃ�(void)
return :�Ȃ�(void)
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

summary:�����蔻��
param  :�Ȃ�(void)
return :�Ȃ�(void)
----------------------------------------------------------------*/
void ActionGame::Collision()
{
	for (auto itr = this->enemy_.begin(); itr != this->enemy_.end();)
	{
		if ((*itr)->IsCollision(this->player_))
		{
			/* ���������̂ŏ��� */
			(*itr).reset();
			itr = this->enemy_.erase(itr);
		}
		else
			++itr;
	}
}

/*-----------------------------------------------------
DrawDebugMode

summary:�f�o�b�O���[�h���̕`��
param  :�Ȃ�(void)
return :�Ȃ�(void)
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

summary:����
param  :���񂷂�p�x(float)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::Turn(float angle_radian)
{
	this->player_->Turn(angle_radian);
}

/*-----------------------------------------------------
Move

summary:�ړ�
param  :�v���C���[�̑���(float)�A�J�����̑���(float)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::Move(float player_spd,float camera_spd)
{
	this->player_->Move(player_spd);		// ���J�����ݒ������ɏ�������

	Vector3 player_dir = this->player_->GetDirection();
	this->camera_->Move(-player_dir, camera_spd);
}

/*----------------------------------------------------------------
ChangeDebugMode

summary:�f�o�b�O���[�h�ؑ�
param  :�Ȃ�(void)
return :�Ȃ�(void)
----------------------------------------------------------------*/
void ActionGame::ChangeDebugMode()
{
	this->is_debug_mode_ ^= 1;		// ���]

	this->player_->SetVisibleCollision(this->is_debug_mode_);
	for (auto& v : this->enemy_)
		v->SetVisibleCollision(this->is_debug_mode_);
}
