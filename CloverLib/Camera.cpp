/*-------------------------------------
�J�����N���X(Camera.cpp)

date  :2017.07.10
Author:Miyu Hara
-------------------------------------*/
/* �C���N���[�h�t�@�C�� */
#include "Camera.h"

/* ���O��Ԃ̎g�p */
using namespace DirectX;
using namespace DirectX::SimpleMath;

/* �����o�֐��̒�` */
namespace CloverLib
{
	Camera::Camera(const ViewMaterial& view, const ProjectionMaterial& proj)
		:view_material(view),
		 proj_material(proj)
	{
		this->Update();
	}

	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		this->view_ = Matrix::CreateLookAt(this->view_material.eye, this->view_material.target, this->view_material.up);
		this->proj_ = Matrix::CreatePerspectiveFieldOfView(this->proj_material.fov_radians, this->proj_material.aspect,
		                                                   this->proj_material.near_clip, this->proj_material.far_clip);
	}

	void Camera::SetRotateY(float angle_radian)
	{
		Matrix rotate = Matrix::CreateRotationY(angle_radian);
		this->view_material.target = Vector3::Transform(this->view_material.target, rotate);
	}
}