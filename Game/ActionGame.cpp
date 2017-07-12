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
	assert(this->InitCamera(width, height));

	this->input_ = make_unique<MyKeyboard>();

	/* ���f�������� */
	this->skydome_ = Model3D::Create(L"Resources/Skydome.cmo");
	this->skydome_->SetLighting(false);
	this->skydome_->SetTranslation(Vector3::Zero);

	this->grass_ = Model3D::Create(L"Resources/Grass.cmo");
	this->grass_->SetTranslation(Vector3::Zero);
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
	const float ROTATE_RADIAN = XMConvertToRadians(3.0f);
	if (this->input_->IsLeft())
		this->camera_->SetRotateY(ROTATE_RADIAN);

	if (this->input_->IsRight())
		this->camera_->SetRotateY(-ROTATE_RADIAN);

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
