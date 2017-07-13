/*-------------------------------------
�A�N�V�����Q�[���{��(ActionGame.cpp)

date  :2017.07.06
Author:Miyu Hara
-------------------------------------*/
/* �C���N���[�h�t�@�C�� */
#include "ActionGame.h"

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
	static unique_ptr<ActionGame> instance(new ActionGame());
	return instance;
}

/*-----------------------------------------------------
~ActionGame

summary:�f�X�g���N�^
param  :���݂��Ȃ�
return :���݂��Ȃ�
-----------------------------------------------------*/
ActionGame::~ActionGame()
{
}

/*-----------------------------------------------------
Initialize

summary:������
param  :�E�C���h�E�n���h��(HWND)�A��(int)�A����(int)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::Initialize(HWND window, int width, int height)
{
	Game::Initialize(window, width, height);

	// TODO:������
	this->InitCamera(width, height);
	this->input_ = make_unique<MyKeyboard>();		// �Ƃ肠�����L�[�{�[�h��

	/* ���f�������� */
	this->skydome_ = Model3D::Create(L"Resources/Skydome.cmo");
	this->skydome_->SetLighting(false);

	this->grass_ = Model3D::Create(L"Resources/Grass.cmo");

	this->player_ = make_unique<Player>();

}

/*-----------------------------------------------------
Update

summary:�X�V
param  :(const DX::StepTimer&)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::Update(DX::StepTimer const & timer)
{
	// TODO:�X�V
	this->input_->Update();

	// ������
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

summary:�`��
param  :�Ȃ�(void)
return :�Ȃ�(void)
-----------------------------------------------------*/
void ActionGame::Render()
{
	Clear();

	// TODO:�`��
	this->skydome_->Draw();
	this->grass_->Draw();
	this->player_->Draw();

	Present();
}

/*-----------------------------------------------------
ActionGame

summary:�R���X�g���N�^
param  :�Ȃ�(void)
return :���݂��Ȃ�
-----------------------------------------------------*/
ActionGame::ActionGame()
	:camera_(nullptr),
	 input_(nullptr),
	 skydome_(nullptr)
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
	vm.eye    = Vector3(0.0f, 2.0f, 20.0f);
	vm.target = Vector3(0.0f, 2.0f, 0.0f);
	vm.up     = Vector3::Up;
	// �ォ�猩���낷�J�����ɂ���Ƃ��͉��̃R�����g�O��
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
	this->player_->Move(player_spd);		// �J�����ݒ������ɏ�������

	Vector3 player_dir = this->player_->GetDirection();
	this->camera_->Move(-player_dir, camera_spd);
}
