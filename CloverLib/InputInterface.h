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

		virtual bool IsLeft() = 0;
		virtual bool IsLeftDown() = 0;
		virtual bool IsRight() = 0;
		virtual bool IsRightDown() = 0;
	};
}