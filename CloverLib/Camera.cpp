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
	/*----------------------------------------------------------
	Camera

	summary:�R���X�g���N�^
	param  :�ޗ�(const ViewMaterial&�Aconst ProjectionMatrial)
	return :���݂��Ȃ�
	----------------------------------------------------------*/
	Camera::Camera(const ViewMaterial& view, const ProjectionMaterial& proj)
		:view_material_(view),
		 proj_material_(proj)
	{
		this->Update();
	}

	/*----------------------------------------------------------
	~Camera

	summary:�f�X�g���N�^
	param  :���݂��Ȃ�
	return :���݂��Ȃ�
	----------------------------------------------------------*/
	Camera::~Camera()
	{}

	/*----------------------------------------------------------
	Update

	summary:�X�V
	param  :�Ȃ�(void)
	return :�Ȃ�(void)
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

	summary:Y����]
	param  :�ʒu(const DirectX::SimpleMath&)�A��]�p�x(float)
	return :�Ȃ�(void)

	���Q�l�����F�x�m�������� - 9.�S���Ђ��J����
	----------------------------------------------------------*/
	void Camera::RotateY(const Vector3& pos, float angle_radian)
	{
		Vector3 eye, target;

		target = pos;
		Vector3 camera(0.0f, 0.0f, 30.0f);			// �J�����ƈʒu�Ƃ̋���

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