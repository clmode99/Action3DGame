/*-------------------------------------
アクションゲーム本体(ActionGame.h)

date  :2017.07.06
Author:Miyu Hara
-------------------------------------*/
/* 多重インクルード防止 */
#pragma once

/* インクルードファイル */
#include <list>
#include <SpriteBatch.h>

#include "../GameFramework/Game.h"
#include "../CloverLib/Camera.h"
#include "../CloverLib/MyKeyboard.h"
#include "../CloverLib/MyGamePad.h"
#include "../CloverLib/Model3D.h"
#include "../Game/Player.h"
#include "../Game/Enemy.h"

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
	std::unique_ptr<DirectX::SpriteBatch>& GetSpriteBatch() { return this->sprite_batch_; }
	std::unique_ptr<CloverLib::Camera>&    GetCamera()      { return this->camera_; }

private:
	ActionGame();	// コンストラクタ

	bool InitCamera(int width, int height);		// カメラ初期化
	void FixCameraHeight();						// カメラの高さを固定する
	
	void CreateEnemy(const DirectX::SimpleMath::Vector3& pos, float scale = 1.0f, const DirectX::SimpleMath::Vector3& color = DirectX::SimpleMath::Vector3::One);

	void Input();			// 入力処理
	void Collision();		// 当たり判定

	void DrawDebugFont();	// デバッグモード時の描画

	void Turn(float angle_radian);					// 旋回
	void Move(float player_spd,float camera_spd);	// 移動
	void ChangeDebugMode();							// デバッグモード切替

private:
	std::unique_ptr<DirectX::SpriteBatch> sprite_batch_;

	std::unique_ptr<CloverLib::Camera>          camera_;	// カメラ
	std::shared_ptr<CloverLib::InputInterface>& input_;		// 入力装置

	std::unique_ptr<CloverLib::Model3D> skydome_;	// 天球
	std::unique_ptr<CloverLib::Model3D> ground_;	// 地面

	std::unique_ptr<Player>            player_;		// プレイヤー
	std::list<std::unique_ptr<Enemy>>  enemy_;		// 敵

	bool is_debug_mode_;		// デバッグモード？
};