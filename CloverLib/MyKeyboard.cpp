/*----------------------------------------------
キーボード(MyKeyboard.cpp)

date  :2017.07.11
Author:Miyu Hara
----------------------------------------------*/
/* インクルードファイル */
#include "MyKeyboard.h"

/* 名前空間の使用 */
using namespace DirectX;
using namespace std;

/* メンバ関数の定義 */
namespace CloverLib
{
	/*---------------------------------------------------------------
	GetInstance

	summary:インスタンス取得
	param  :なし(void)
	return :インスタンス(std::unique_ptr<InputInterface>&)
	---------------------------------------------------------------*/
	shared_ptr<InputInterface>& MyKeyboard::GetInstance()
	{
		static shared_ptr<InputInterface> s_instance(new MyKeyboard());
		return s_instance;
	}

	/*---------------------------------------------------------------
	~MyKeyboard

	summary:デストラクタ
	param  :存在しない
	return :存在しない
	---------------------------------------------------------------*/
	MyKeyboard::~MyKeyboard()
	{}

	/*---------------------------------------------------------------
	Update

	summary:更新
	param  :なし(void)
	return :なし(vid)
	---------------------------------------------------------------*/
	void MyKeyboard::Update()
	{
		this->key_state_ = this->keyboard_->GetState();
		this->key_tracker_.Update(this->key_state_);
	}

	/*---------------------------------------------------------------
	IsConnected

	summary:接続されているか
	param  :なし(void)
	return :接続(true)、未接続(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsConnected()
	{
		return true;	// 常に接続されている
	}

	/*---------------------------------------------------------------
	IsLeft

	summary:左を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsLeft()
	{
		// 仮処理
		return this->key_state_.A;
	}

	/*---------------------------------------------------------------
	IsLeftDown

	summary:左を押したか(トリガー)
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsLeftDown()
	{
		// 仮処理
		return this->key_tracker_.IsKeyPressed(Keyboard::A);
	}

	/*---------------------------------------------------------------
	IsRight

	summary:右を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsRight()
	{
		// 仮処理
		return this->key_state_.D;
	}

	/*---------------------------------------------------------------
	IsRightDown

	summary:右を押したか(トリガー)
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsRightDown()
	{
		// 仮処理
		return this->key_tracker_.IsKeyPressed(Keyboard::D);
	}

	/*---------------------------------------------------------------
	IsUp

	summary:上を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsUp()
	{
		return this->key_state_.W;
	}

	/*---------------------------------------------------------------
	IsUpDown

	summary:上を押したか(トリガー)
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsUpDown()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::W);
	}

	/*---------------------------------------------------------------
	IsDown

	summary:下を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsDown()
	{
		return this->key_state_.S;
	}

	/*---------------------------------------------------------------
	IsDownDown

	summary:下を押したか(トリガー)
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsDownDown()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::S);
	}

	/*---------------------------------------------------------------
	IsControlLeft

	summary:コントロール用の左を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlLeft()
	{
		return this->key_state_.Left;
	}

	/*---------------------------------------------------------------
	IsControlRight

	summary:コントロール用の右を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlRight()
	{
		return this->key_state_.Right;
	}

	/*---------------------------------------------------------------
	IsControlUp

	summary:コントロール用の上を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlUp()
	{
		return this->key_state_.Up;
	}

	/*---------------------------------------------------------------
	IsControlDown

	summary:コントロール用の下を押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsControlDown()
	{
		return this->key_state_.Down;
	}

	/*---------------------------------------------------------------
	IsJump

	summary:ジャンプボタンを押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsJump()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::Space);
	}

	/*---------------------------------------------------------------
	IsDebug

	summary:デバッグボタンを押したか
	param  :なし(void)
	return :押した(true)、押してない(false)
	---------------------------------------------------------------*/
	bool MyKeyboard::IsDebug()
	{
		return this->key_tracker_.IsKeyPressed(Keyboard::C);
	}

	/*---------------------------------------------------------------
	MyKeyboard

	summary:コンストラクタ
	param  :なし(void)
	return :存在しない
	---------------------------------------------------------------*/
	MyKeyboard::MyKeyboard()
		:keyboard_(make_unique<Keyboard>())
	{
		this->Update();
	}
}