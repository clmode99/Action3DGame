/*-------------------------------------
�A�N�V�����Q�[���{��(ActionGame.h)

date  :2017.07.06
Author:Miyu Hara
-------------------------------------*/
/* ���d�C���N���[�h�h�~ */
#pragma once

/* �C���N���[�h�t�@�C�� */
#include "../GameFramework/Game.h"
#include "../CloverLib/Camera.h"
#include "../CloverLib/MyKeyboard.h"
#include "../CloverLib/Model3D.h"
#include "../Game/Player.h"

/* �N���X�̒�` */
class ActionGame : public Game
{
public:
	static std::unique_ptr<ActionGame>& GetInstance();		// �C���X�^���X�擾

public:
	virtual ~ActionGame();		// �f�X�g���N�^
	
	void Initialize(HWND window, int width, int height);	// ������
	void Update(DX::StepTimer const& timer);				// �X�V
	void Render();											// �`��
	
	/* �擾�n */
	std::unique_ptr<CloverLib::Camera>& GetCamera() { return this->camera_; }

private:
	ActionGame();	// �R���X�g���N�^

	bool InitCamera(int width, int height);		// �J����������
	void FixCameraHeight();						// �J�����̍������Œ肷��
	
	void Turn(float angle_radian);					// ����
	void Move(float player_spd,float camera_spd);	// �ړ�

private:
	std::unique_ptr<CloverLib::Camera>         camera_;		// �J����
	std::unique_ptr<CloverLib::InputInterface> input_;		// ���͑��u

	std::unique_ptr<CloverLib::Model3D> skydome_;	// �V��
	std::unique_ptr<CloverLib::Model3D> ground_;	// �n��

	std::unique_ptr<Player> player_;	// �v���C���[
	// enemy
};

