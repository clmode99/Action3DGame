/*----------------------------------------------
�L�[�{�[�h(MyKeyboard.h)

date  :2017.07.11
Author:Miyu Hara
----------------------------------------------*/
/* ���d�C���N���[�h�h�~ */
#pragma once

/* �C���N���[�h�t�@�C�� */
#include "InputInterface.h"
#include <d3d11.h>
#include <d3d11_1.h>
#include <Keyboard.h>

/* �N���X�̒�` */
namespace CloverLib
{
	class MyKeyboard : public InputInterface
	{
	public:
		MyKeyboard();
		virtual ~MyKeyboard();

		void Update()     override;

		bool IsLeft()      override;
		bool IsLeftDown()  override;
		bool IsRight()     override;
		bool IsRightDown() override;
		bool IsUp()	      override;
		bool IsUpDown()   override;
		bool IsDown()     override;
		bool IsDownDown() override;

		bool IsControlLeft()  override;
		bool IsControlRight() override;
		bool IsControlUp()   override;
		bool IsControlDown() override;

	private:
		std::unique_ptr<DirectX::Keyboard> keyboard_;
		DirectX::Keyboard::State                key_state_;
		DirectX::Keyboard::KeyboardStateTracker key_tracker_;
	};
}