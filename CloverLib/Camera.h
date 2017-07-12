/*-------------------------------------
�J�����N���X(Camera.h)

date  :2017.07.10
Author:Miyu Hara
-------------------------------------*/
/* ���d�C���N���[�h�h�~ */
#pragma once

/* �C���N���[�h�t�@�C�� */
#include <d3d11.h>
#include <d3d11_1.h>
#include <SimpleMath.h>

/* �N���X�̒�` */
namespace CloverLib
{
	/* �r���[�s��쐬���ɕK�v�ȍޗ� */
	struct ViewMaterial
	{
		DirectX::SimpleMath::Vector3 eye;			// �J�����̈ʒu
		DirectX::SimpleMath::Vector3 target;		// �ǂ������Ă��邩
		DirectX::SimpleMath::Vector3 up;			// �J�����̏�̌���
	};

	/* �ˉe�s��쐬���ɕK�v�ȍޗ� */
	struct ProjectionMaterial
	{
		float fov_radians;		// ����p
		float aspect;			// �A�X�y�N�g��
		float near_clip;		// near�N���b�v
		float far_clip;			// far�N���b�v
	};

	/* �J���� */
	class Camera
	{
	public:
		Camera(const ViewMaterial& view,const ProjectionMaterial& proj);		// �R���X�g���N�^
		virtual ~Camera();														// �f�X�g���N�^

		void Update();	// �X�V

		void SetRotateY(float angle_radian);

		/* �擾�n */
		const DirectX::SimpleMath::Matrix& GetView()       { return this->view_; }
		const DirectX::SimpleMath::Matrix& GetProjection() { return this->proj_; }

	private:
		DirectX::SimpleMath::Matrix view_;		// �r���[�s��
		DirectX::SimpleMath::Matrix proj_;		// �ˉe�s��

		/* �ޗ� */
		ViewMaterial       view_material;
		ProjectionMaterial proj_material;
	};
}
