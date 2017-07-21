/*-------------------------------------
カメラクラス(Camera.cpp)

date  :2017.07.10
Author:Miyu Hara
-------------------------------------*/
/* インクルードファイル */
#include "Camera.h"

/* 名前空間の使用 */
using namespace DirectX;
using namespace DirectX::SimpleMath;

/* メンバ関数の定義 */
namespace CloverLib
{
	/*----------------------------------------------------------
	Camera

	summary:コンストラクタ
	param  :材料(const ViewMaterial&、const ProjectionMatrial)
	return :存在しない
	----------------------------------------------------------*/
	Camera::Camera(const ViewMaterial& view, const ProjectionMaterial& proj)
		:view_material_(view),
		 proj_material_(proj)
	{
		this->Update();
	}

	/*----------------------------------------------------------
	~Camera

	summary:デストラクタ
	param  :存在しない
	return :存在しない
	----------------------------------------------------------*/
	Camera::~Camera()
	{}

	/*----------------------------------------------------------
	Update

	summary:更新
	param  :なし(void)
	return :なし(void)
	----------------------------------------------------------*/
	void Camera::Update()
	{
		assert(this->view_material_.eye != this->view_material_.target);

		this->view_ = Matrix::CreateLookAt(this->view_material_.eye, this->view_material_.target, this->view_material_.up);
		this->proj_ = Matrix::CreatePerspectiveFieldOfView(this->proj_material_.fov_radians, this->proj_material_.aspect,
		                                                   this->proj_material_.near_clip, this->proj_material_.far_clip);
	}

	/*----------------------------------------------------------
	RotateY

	summary:Y軸回転
	param  :位置(const DirectX::SimpleMath&)、回転角度(float)
	return :なし(void)

	※参考資料：富士見研究所 - 9.ゴムひもカメラ
	----------------------------------------------------------*/
	void Camera::RotateY(const Vector3& pos, float angle_radian)
	{
		Vector3 eye, target;

		target = pos;
		Vector3 camera(0.0f, 0.0f, 30.0f);			// カメラと位置との距離

		static float s_angle = 0.0f;
		s_angle += angle_radian;
		Matrix rotate = Matrix::CreateRotationY(s_angle);
		camera = Vector3::TransformNormal(camera, rotate);

		eye = target + camera;

		this->view_material_.eye    = eye;
		this->view_material_.target = target;
	}

	void Camera::Move(const Vector3& dir, float speed)
	{
		const Vector3 VELOCITY = dir * speed;
		this->view_material_.eye    += VELOCITY;
		this->view_material_.target += VELOCITY;
	}
}