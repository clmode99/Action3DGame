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
#include "../CloverLib/Model3D.h"
#include "../CloverLib/MyKeyboard.h"

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
	ActionGame();				// �R���X�g���N�^

	bool InitCamera(int width, int height);		// �J����������

private:
	std::unique_ptr<CloverLib::Camera>         camera_;		// �J����
	std::unique_ptr<CloverLib::InputInterface> input_;		// ���͑��u

	std::unique_ptr<CloverLib::Model3D> skydome_;
	std::unique_ptr<CloverLib::Model3D> grass_;
	// player
	// enemy
};

