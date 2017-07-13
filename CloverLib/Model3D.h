/*-------------------------------------
3Dモデルクラス(Model3D.h)

date  :2017.06.02
Author:Miyu Hara
-------------------------------------*/
/* 多重インクルード防止 */
#pragma once

/* インクルードファイル */
#include <memory>
#include <wrl\client.h>

#include <CommonStates.h>
#include <Effects.h>
#include <Model.h>
#include <SimpleMath.h>

#include "Camera.h"

/* クラスの定義 */
namespace CloverLib
{
	class Model3D
	{
	public:
		static std::unique_ptr<Model3D> Create(const wchar_t* file_name, bool is_visible = true);	// 生成

	public:
		virtual ~Model3D();						// デストラクタ

		void LoadModelCMO(const wchar_t* file_name);	// CMOモデル読込

		void Draw();		// 描画

		/* 各種設定・取得 */
		// 拡大縮小
		void SetScale(float scale) { scale_ = DirectX::SimpleMath::Matrix::CreateScale(scale); }

		// 回転
		void SetRotate(float x, float y, float z);						// float版
		void SetRotate(const DirectX::SimpleMath::Matrix& rotate);		// Matrix版
		void SetRotateYPR(float yaw, float pitch, float roll);			// ヨー・ピッチ・ロール版

		// 座標
		void SetTranslation(float x, float y, float z);						// float版
		void SetTranslation(const DirectX::SimpleMath::Vector3& trans);		// Vector3版

		// 表示
		void SetVisible(bool is_visible)           { is_visible = is_visible; }					// オブジェクト
		void SetWireframe(bool is_draw_wireframe)  { is_draw_wireframe = is_draw_wireframe; }	// ワイヤーフレーム

		// 見た目の調整
		void SetLighting(bool flag);		// ライトの切替
		void SetSpecular(bool flag);		// 鏡面反射光の切替

	private:
		Model3D();		// コンストラクタ

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>&        device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context_;
		std::unique_ptr<DirectX::EffectFactory>      effect_;
		std::unique_ptr<DirectX::CommonStates>       states_;
		std::unique_ptr<Camera>&                     camera_;

		std::unique_ptr<DirectX::Model> model_;		// モデル

		DirectX::SimpleMath::Matrix     scale_;		// 拡大縮小率
		DirectX::SimpleMath::Quaternion rotate_;	// 回転
		DirectX::SimpleMath::Matrix     trans_;		// 座標

		bool is_visible_;			// モデルを表示するか
		bool is_draw_wireframe_;	// ワイヤーフレームを表示するか
	};
}
