/*-------------------------------------
カメラクラス(Camera.h)

date  :2017.07.10
Author:Miyu Hara
-------------------------------------*/
/* 多重インクルード防止 */
#pragma once

/* インクルードファイル */
#include <d3d11.h>
#include <d3d11_1.h>
#include <SimpleMath.h>

/* クラスの定義 */
namespace CloverLib
{
	/* ビュー行列作成時に必要な材料 */
	struct ViewMaterial
	{
		DirectX::SimpleMath::Vector3 eye;			// カメラの位置
		DirectX::SimpleMath::Vector3 target;		// どこを見ているか
		DirectX::SimpleMath::Vector3 up;			// カメラの上の向き
	};

	/* 射影行列作成時に必要な材料 */
	struct ProjectionMaterial
	{
		float fov_radians;		// 視野角
		float aspect;			// アスペクト比
		float near_clip;		// nearクリップ
		float far_clip;			// farクリップ
	};

	/* カメラ */
	class Camera
	{
	public:
		Camera(const ViewMaterial& view,const ProjectionMaterial& proj);		// コンストラクタ
		virtual ~Camera();														// デストラクタ

		void Update();	// 更新

		void RotateY(const DirectX::SimpleMath::Vector3& pos, float angle_radian);		// Y軸に回転

		void Move(const DirectX::SimpleMath::Vector3& dir, float speed);		// 移動

		/* 設定・取得系 */
		void SetEye(const DirectX::SimpleMath::Vector3& eye)       { this->view_material_.eye = eye; }
		void SetTarget(const DirectX::SimpleMath::Vector3& target) { this->view_material_.target = target; }

		const DirectX::SimpleMath::Matrix& GetView()       { return this->view_; }
		const DirectX::SimpleMath::Matrix& GetProjection() { return this->proj_; }

	private:
		DirectX::SimpleMath::Matrix view_;		// ビュー行列
		DirectX::SimpleMath::Matrix proj_;		// 射影行列

		/* 材料 */
		ViewMaterial       view_material_;
		ProjectionMaterial proj_material_;
	};
}

