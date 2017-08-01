/*----------------------------------------------
キーボード(MyKeyboard.h)

date  :2017.07.11
Author:Miyu Hara
----------------------------------------------*/
/* 多重インクルード防止 */
#pragma once

/* インクルードファイル */
#include "InputInterface.h"
#include <d3d11.h>
#include <d3d11_1.h>
#include <Keyboard.h>

/* クラスの定義 */
namespace CloverLib
{
	class MyKeyboard : public InputInterface
	{
	public:
		static std::shared_ptr<InputInterface>& GetInstance();

	public:
		virtual ~MyKeyboard();

		void Update()      override;
		bool IsConnected() override;

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

		bool IsJump() override;

		bool IsDebug() override;

	private:
		MyKeyboard();

	private:
		std::unique_ptr<DirectX::Keyboard> keyboard_;
		DirectX::Keyboard::State                key_state_;
		DirectX::Keyboard::KeyboardStateTracker key_tracker_;
	};
}