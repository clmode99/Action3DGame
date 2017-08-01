/*----------------------------------------------
�L�[�{�[�h(MyKeyboard.cpp)

date  :2017.07.11
Author:Miyu Hara
----------------------------------------------*/
/* �C���N���[�h�t�@�C�� */
#include "MyKeyboard.h"

/* ���O��Ԃ̎g�p */
using namespace DirectX;
using namespace std;

/* �����o�֐��̒�` */
namespace CloverLib
{
	/*---------------------------------------------------------------
	GetInstance

	summary:�C���X�^���X�擾
	param  :�Ȃ�(void)
	return :�C���X�^���X(std::unique_ptr<InputInterface>&)
	---------------------------------------------------------------*/
	shared_ptr<InputInterface>& MyKeyboard::GetInstance()
	{
		static shared_ptr<InputInterface> s_instance(new MyKeyboard());
		return s_instance;
	}

	/*---------------------------------------------------------------
	~MyKeyboard

	summary:�f�X�g���N�^
	param  :���݂��Ȃ�
	return :���݂��Ȃ�
	---------------------------------------------------------------*/
	MyKeyboard::~MyKeyboard()
	{}

	/*---------------------------------------------------------------
	Update

	summary:�X�V
	param  :�Ȃ�(void)
	return :�Ȃ�(vid)
	---------------------------------------------------------------*/
	void MyKeyboard::Update()
	{
		this->key_state_ = this->keyboard_->GetState();
		this->key_tracker_.Update(this->key_state_);
	}

	/*---------------------------------------------------------------
	IsConnected

	summary:�ڑ�����Ă��邩
	param  :�Ȃ�(void)
	return :�ڑ�(true)�A���ڑ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsConnected()
	{
		return true;	// ��ɐڑ�����Ă���
	}

	/*---------------------------------------------------------------
	IsLeft

	summary:������������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsLeft()
	{
		// ������
		return this->key_state_.A;
	}

	/*---------------------------------------------------------------
	IsLeftDown

	summary:������������(�g���K�[)
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsLeftDown()
	{
		// ������
		return this->key_tracker_.IsKeyPressed(Keyboard::A);
	}

	/*---------------------------------------------------------------
	IsRight

	summary:�E����������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsRight()
	{
		// ������
		return this->key_state_.D;
	}

	/*---------------------------------------------------------------
	IsRightDown

	summary:�E����������(�g���K�[)
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsRightDown()
	{
		// ������
		return this->key_tracker_.IsKeyPressed(Keyboard::D);
	}

	/*---------------------------------------------------------------
	IsUp

	summary:�����������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsUp()
	{
		return this->key_state_.W;
	}

	/*---------------------------------------------------------------
	IsUpDown

	summary:�����������(�g���K�[)
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsUpDown()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::W);
	}

	/*---------------------------------------------------------------
	IsDown

	summary:������������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsDown()
	{
		return this->key_state_.S;
	}

	/*---------------------------------------------------------------
	IsDownDown

	summary:������������(�g���K�[)
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsDownDown()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::S);
	}

	/*---------------------------------------------------------------
	IsControlLeft

	summary:�R���g���[���p�̍�����������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlLeft()
	{
		return this->key_state_.Left;
	}

	/*---------------------------------------------------------------
	IsControlRight

	summary:�R���g���[���p�̉E����������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlRight()
	{
		return this->key_state_.Right;
	}

	/*---------------------------------------------------------------
	IsControlUp

	summary:�R���g���[���p�̏����������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlUp()
	{
		return this->key_state_.Up;
	}

	/*---------------------------------------------------------------
	IsControlDown

	summary:�R���g���[���p�̉�����������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlDown()
	{
		return this->key_state_.Down;
	}

	/*---------------------------------------------------------------
	IsJump

	summary:�W�����v�{�^������������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsJump()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::Space);
	}

	/*---------------------------------------------------------------
	IsDebug

	summary:�f�o�b�O�{�^������������
	param  :�Ȃ�(void)
	return :������(true)�A�����ĂȂ�(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsDebug()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::C);
	}

	/*---------------------------------------------------------------
	MyKeyboard

	summary:�R���X�g���N�^
	param  :�Ȃ�(void)
	return :���݂��Ȃ�
	---------------------------------------------------------------*/
	MyKeyboard::MyKeyboard()
		:keyboard_(make_unique<Keyboard>())
	{
		this->Update();
	}
}