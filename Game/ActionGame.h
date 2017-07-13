/*-------------------------------------
アクションゲーム本体(ActionGame.h)

date  :2017.07.06
Author:Miyu Hara
-------------------------------------*/
/* 多重インクルード防止 */
#pragma once

/* インクルードファイル */
#include "../GameFramework/Game.h"
#include "../CloverLib/Camera.h"
#include "../CloverLib/MyKeyboard.h"
#include "../CloverLib/Model3D.h"
#include "../Game/Player.h"

/* クラスの定義 */
class ActionGame : public Game
{
public:
	static std::unique_ptr<ActionGame>& GetInstance();		// インスタンス取得

public:
	virtual ~ActionGame();		// デストラクタ
	
	void Initialize(HWND window, int width, int height);	// 初期化
	void Update(DX::StepTimer const& timer);				// 更新
	void Render();											// 描画
	
	/* 取得系 */
	std::unique_ptr<CloverLib::Camera>& GetCamera() { return this->camera_; }

private:
	ActionGame();				// コンストラクタ

	bool InitCamera(int width, int height);		// カメラ初期化
	
	void Turn(float angle_radian);
	void Move(float player_spd,float camera_spd);

private:
	std::unique_ptr<CloverLib::Camera>         camera_;		// カメラ
	std::unique_ptr<CloverLib::InputInterface> input_;		// 入力装置

	std::unique_ptr<CloverLib::Model3D> skydome_;
	std::unique_ptr<CloverLib::Model3D> grass_;
	std::unique_ptr<Player> player_;
	// enemy
};

