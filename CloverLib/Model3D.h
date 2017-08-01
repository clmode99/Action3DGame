/*-------------------------------------
3D���f���N���X(Model3D.h)

date  :2017.06.02
Author:Miyu Hara
-------------------------------------*/
/* ���d�C���N���[�h�h�~ */
#pragma once

/* �C���N���[�h�t�@�C�� */
#include <list>
#include <map>
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
		enum class STATE
		{
			NORMAL,			// ����
			ADD,			// ���Z
			SUBTRACT,		// ���Z
		};

	public:
		virtual ~Model3D();						// �f�X�g���N�^

		void LoadModelCMO(const wchar_t* file_name);	// CMO���f���Ǎ�

		void Draw(STATE state = STATE::NORMAL);		// �`��

		void AddChild(Model3D* child);								// �q��ǉ�

		/* �e��ݒ�E�擾 */
		// �g��k��
		void SetScale(float scale) { scale_ = DirectX::SimpleMath::Matrix::CreateScale(scale); }

		// ��]
		void SetRotate(float x, float y, float z);						// float��
		void SetRotate(const DirectX::SimpleMath::Matrix& rotate);		// Matrix��

		// ���W
		void SetTranslation(float x, float y, float z);						// float��
		void SetTranslation(const DirectX::SimpleMath::Vector3& trans);		// Vector3��
		const DirectX::SimpleMath::Vector3& GetTranslation();
		void SetOffset(const DirectX::SimpleMath::Vector3& offset);

		// �\��
		void SetVisible(bool is_visible)           { is_visible = is_visible; }					// �I�u�W�F�N�g
		void SetWireframe(bool is_draw_wireframe)  { is_draw_wireframe = is_draw_wireframe; }	// ���C���[�t���[��

		// �����ڂ̒���
		void SetDiffuseColor(const DirectX::SimpleMath::Vector3& color);	// �g�U���ˌ��̐ݒ�
		void SetSpecular(bool flag);										// ���ʔ��ˌ��̐ؑ�
		void SetLighting(bool flag);										// ���C�g�̐ؑ�

	private:
		Model3D();		// �R���X�g���N�^
		
		void Draw(const std::function<void()>& state);		// �`��(private��)

		void CreateState();			// �e�X�e�[�g�쐬

		void CalcWorldMatrix();		// ���[���h�s��v�Z

		void SetParent(Model3D* parent) { this->parent_ = parent; }	// �e��ݒ�

	private:
		static std::map<const wchar_t*, std::shared_ptr<DirectX::Model>> model_map_;	// ���f���}�b�v

	private:
		Microsoft::WRL::ComPtr<ID3D11Device>&        device_;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& context_;
		std::unique_ptr<DirectX::EffectFactory>      effect_;
		std::unique_ptr<DirectX::CommonStates>       states_;
		std::unique_ptr<Camera>&                     camera_;

		// �X�e�[�g
		Microsoft::WRL::ComPtr<ID3D11BlendState> add_state_;		// ���Z
		Microsoft::WRL::ComPtr<ID3D11BlendState> subtract_state_;	// ���Z

		std::shared_ptr<DirectX::Model>     model_;			// ���f��

		Model3D* parent_;					// �e
		std::list<Model3D*> children_;		// �q

		DirectX::SimpleMath::Matrix     scale_;		// �g��k����
		DirectX::SimpleMath::Quaternion rotate_;	// ��]
		DirectX::SimpleMath::Matrix     trans_;		// ���W(Matrix��)
		DirectX::SimpleMath::Vector3    trans_vec_;	// ���W(Vector��)
		DirectX::SimpleMath::Matrix     world_;		// ���[���h���W

		DirectX::SimpleMath::Vector3 offset_;		// �I�t�Z�b�g

		bool is_visible_;			// ���f����\�����邩
		bool is_draw_wireframe_;	// ���C���[�t���[����\�����邩
	};
}
