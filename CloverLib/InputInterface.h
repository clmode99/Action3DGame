/*----------------------------------------------
入力系の抽象インターフェイス(InputInterface.h)

date  :2017.07.11
Author:Miyu Hara
----------------------------------------------*/
/* 多重インクルード防止 */
#pragma once

/* クラスの定義 */
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