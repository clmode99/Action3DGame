/*----------------------------------------------
���͌n�̒��ۃC���^�[�t�F�C�X(InputInterface.h)

date  :2017.07.11
Author:Miyu Hara
----------------------------------------------*/
/* ���d�C���N���[�h�h�~ */
#pragma once

/* �N���X�̒�` */
namespace CloverLib
{
	class InputInterface
	{
	public:
		virtual void Update() = 0;

		virtual bool IsLeft()     = 0;
		virtual bool IsLeftDown() = 0;
		virtual bool IsRight()     = 0;
		virtual bool IsRightDown() = 0;
		virtual bool IsUp()       = 0;
		virtual bool IsUpDown()   = 0;
		virtual bool IsDown()     = 0;
		virtual bool IsDownDown() = 0;

		virtual bool IsControlLeft()  = 0;
		virtual bool IsControlRight() = 0;
		virtual bool IsControlUp()   = 0;
		virtual bool IsControlDown() = 0;

		virtual bool IsJump() = 0;
	};
}