/*-------------------------------------
3Dモデルクラス(Model3D.h)

date  :2017.06.02
Author:Miyu Hara
-------------------------------------*/
/* 多重インクルード防止 */
#pragma once

/* インクルードファイル */
#include <list>
#include <map>
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
		enum class STATE
		{
			NORMAL,			// 普通
			ADD,			// 加算
			SUBTRACT,		// 減算
		};

	public:
		virtual ~Model3D();						// デストラクタ

		void LoadModelCMO(const wchar_t* file_name);	// CMOモデル読込

		void Draw(STATE state = STATE::NORMAL);		// 描画

		void AddChild(Model3D* child);								// 子を追加

		/* 各種設定・取得 */
		// 拡大縮小
		void SetScale(float scale) { scale_ = DirectX::SimpleMath::Matrix::CreateScale(scale); }

		// 回転
		void SetRotate(float x, float y, float z);						// float版
		void SetRotate(const DirectX::SimpleMath::Matrix& rotate);		// Matrix版

		// 座標
		void SetTranslation(float x, float y, float z);						// float版
		void SetTranslation(const DirectX::SimpleMath::Vector3& trans);		// Vector3版
		const DirectX::SimpleMath::Vector3& GetTranslation();
		void SetOffset(const DirectX::SimpleMath::Vector3& offset);

		// 表示
		void SetVisible(bool is_visible)           { is_visible = is_visible; }					// オブジェクト
		void SetWireframe(bool is_draw_wireframe)  { is_draw_wireframe = is_draw_wireframe; }	// ワイヤーフレーム

		// 見た目の調整
		void SetDiffuseColor(const DirectX::SimpleMath::Vector3& color);	// 拡散反射光の設定
		void SetSpecular(bool flag);										// 鏡面反射光の切替
		void SetLighting(bool flag);										// ライトの切替

	private:
		Model3D();		// コンストラクタ
		
		void Draw(const std::function<void()>& state);		// 描画(private版)

		void CreateState();			// 各ステート作成

		void CalcWorldMatrix();		// ワールド行列計算

		void SetParent(Model3D* parent) { this->parent_ = parent; }	// 親を設定

	private:
		static std::map<const wchar_t*, std::shared_ptr<DirectX::Model>> model_map_;	// モデルマップ

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>&        device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context_;
		std::unique_ptr<DirectX::EffectFactory>      effect_;
		std::unique_ptr<DirectX::CommonStates>       states_;
		std::unique_ptr<Camera>&                     camera_;

		// ステート
		Microsoft::WRL::ComPtr<ID3D11BlendState> add_state_;		// 加算
		Microsoft::WRL::ComPtr<ID3D11BlendState> subtract_state_;	// 減算

		std::shared_ptr<DirectX::Model>     model_;			// モデル

		Model3D* parent_;					// 親
		std::list<Model3D*> children_;		// 子

		DirectX::SimpleMath::Matrix     scale_;		// 拡大縮小率
		DirectX::SimpleMath::Quaternion rotate_;	// 回転
		DirectX::SimpleMath::Matrix     trans_;		// 座標(Matrix版)
		DirectX::SimpleMath::Vector3    trans_vec_;	// 座標(Vector版)
		DirectX::SimpleMath::Matrix     world_;		// ワールド座標

		DirectX::SimpleMath::Vector3 offset_;		// オフセット

		bool is_visible_;			// モデルを表示するか
		bool is_draw_wireframe_;	// ワイヤーフレームを表示するか
	};
}
