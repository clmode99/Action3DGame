/*-------------------------------------
3D���f���N���X(Model3D.h)

date  :2017.06.02
Author:Miyu Hara
-------------------------------------*/
/* ���d�C���N���[�h�h�~ */
#pragma once

/* �C���N���[�h�t�@�C�� */
#include <memory>
#include <wrl\client.h>

#include <CommonStates.h>
#include <Effects.h>
#include <Model.h>
#include <SimpleMath.h>

#include "Camera.h"

/* �N���X�̒�` */
namespace CloverLib
{
	class Model3D
	{
	public:
		static std::unique_ptr<Model3D> Create(const wchar_t* file_name, bool is_visible = true);	// ����

	public:
		virtual ~Model3D();						// �f�X�g���N�^

		void LoadModelCMO(const wchar_t* file_name);	// CMO���f���Ǎ�

		void Draw();		// �`��

		/* �e��ݒ�E�擾 */
		// �g��k��
		void SetScale(float scale) { scale_ = DirectX::SimpleMath::Matrix::CreateScale(scale); }

		// ��]
		void SetRotate(float x, float y, float z);						// float��
		void SetRotate(const DirectX::SimpleMath::Matrix& rotate);		// Matrix��
		void SetRotateYPR(float yaw, float pitch, float roll);			// ���[�E�s�b�`�E���[����

		// ���W
		void SetTranslation(float x, float y, float z);						// float��
		void SetTranslation(const DirectX::SimpleMath::Vector3& trans);		// Vector3��

		// �\��
		void SetVisible(bool is_visible)           { is_visible = is_visible; }					// �I�u�W�F�N�g
		void SetWireframe(bool is_draw_wireframe)  { is_draw_wireframe = is_draw_wireframe; }	// ���C���[�t���[��

		// �����ڂ̒���
		void SetLighting(bool flag);		// ���C�g�̐ؑ�
		void SetSpecular(bool flag);		// ���ʔ��ˌ��̐ؑ�

	private:
		Model3D();		// �R���X�g���N�^

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>&        device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context_;
		std::unique_ptr<DirectX::EffectFactory>      effect_;
		std::unique_ptr<DirectX::CommonStates>       states_;
		std::unique_ptr<Camera>&                     camera_;

		std::unique_ptr<DirectX::Model> model_;		// ���f��

		DirectX::SimpleMath::Matrix     scale_;		// �g��k����
		DirectX::SimpleMath::Quaternion rotate_;	// ��]
		DirectX::SimpleMath::Matrix     trans_;		// ���W

		bool is_visible_;			// ���f����\�����邩
		bool is_draw_wireframe_;	// ���C���[�t���[����\�����邩
	};
}
