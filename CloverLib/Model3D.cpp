/*-------------------------------------
3D���f���N���X(Model3D.cpp)

date  :2017.06.02
Author:Miyu Hara
-------------------------------------*/
/* �C���N���[�h�t�@�C�� */
#include "Model3D.h"
#include "../Game/ActionGame.h"

/* ���O��Ԃ̎g�p */
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/* �����o�ϐ��̒�` */
namespace CloverLib
{
	/*---------------------------------------------------------------
	Create

	summary:��������
	param  :�ǂݍ��ރt�@�C����(const wchar_t*)�A�\�����邩(bool)
	return :�X�v���C�g(unique_ptr<CloverLib::Model3D>)
	---------------------------------------------------------------*/
	unique_ptr<Model3D> Model3D::Create(const wchar_t * file_name, bool is_visible)
	{
		unique_ptr<Model3D> instance(new Model3D());

		instance->LoadModelCMO(file_name);
		instance->SetVisible(is_visible);
		return instance;
	}

	/*---------------------------------------------------------------
	~Model3D

	summary:�f�X�g���N�^
	param  :���݂��Ȃ�
	return :���݂��Ȃ�
	---------------------------------------------------------------*/
	Model3D::~Model3D()
	{}

	/*---------------------------------------------------------------
	Model3D::LoadModelCMO

	summary:CMO���f����ǂݍ���
	param  :�t�@�C����(const wchar_t*)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::LoadModelCMO(const wchar_t* file_name)
	{
		this->model_ = Model::CreateFromCMO(this->device_.Get(), file_name, *(this->effect_));
	}

	/*---------------------------------------------------------------
	Draw

	summary:�`��
	param  :�Ȃ�(void)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::Draw()
	{
		if (!(this->is_visible_))
			return;

		/* �s��v�Z */
		Matrix rotate = Matrix::CreateFromQuaternion(this->rotate_);
		Matrix world = this->scale_ * rotate * this->trans_;

		/* �`�� */
		this->model_->Draw(this->context_.Get(), *(this->states_),
							world, this->camera_->GetView(), this->camera_->GetProjection(),
							this->is_draw_wireframe_);
	}

	/*---------------------------------------------------------------
	SetRotate

	summary:��](float��)
	param  :��]���������p�x(float, float, float)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetRotate(float x, float y, float z)
	{
		Matrix rotate_x = Matrix::CreateRotationX(x);
		Matrix rotate_y = Matrix::CreateRotationY(y);
		Matrix rotate_z = Matrix::CreateRotationZ(z);
		Matrix rotate = rotate_z * rotate_x * rotate_y;

		this->SetRotate(rotate);
	}

	/*---------------------------------------------------------------
	SetRotate

	summary:��](Matrix��)
	param  :��]�s��(DirectX::SimpleMath::Matrix&)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetRotate(const Matrix& rotate)
	{
		this->rotate_ = Quaternion::CreateFromRotationMatrix(rotate);
	}

	/*---------------------------------------------------------------
	SetRotateYPR

	summary:��](���[�E�s�b�`�E���[����)
	param  :���[(float)�A�s�b�`(float)�A���[��(float)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetRotateYPR(float yaw, float pitch, float roll)
	{
		this->rotate_ = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	}

	/*---------------------------------------------------------------
	SetTranslation

	summary:�ړ�(float��)
	param  :�ړ������������W(float, float, float)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetTranslation(float x, float y, float z)
	{
		Vector3 trans = Vector3(x, y, z);
		SetTranslation(trans);
	}

	/*---------------------------------------------------------------
	SetTranslation

	summary:�ړ�(Vector3��)
	param  :�ړ��s��(DirectX::SimpleMath::Vector3&)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetTranslation(const Vector3& trans)
	{
		this->trans_ = Matrix::CreateTranslation(trans);
	}

	/*---------------------------------------------------------------
	SetLighting

	summary:���C�g�̐ؑ�
	param  :�I���I�t(bool)
	return :�Ȃ�(void)

	���Q�l�����FObj3D.cpp - Obj3D::DisableLighting()
	---------------------------------------------------------------*/
	void Model3D::SetLighting(bool is_lighting)
	{
		/* ���b�V�����̃��[�v */
		for (auto& mesh : this->model_->meshes)
		{
			auto model_mesh = mesh.get();
			assert(model_mesh);

			/* ���b�V���p�[�c���̃��[�v */
			for (auto& mesh_part : model_mesh->meshParts)
			{
				auto model_mesh_part = mesh_part.get();
				assert(model_mesh_part);

				auto ieffect      = model_mesh_part->effect;
				auto basic_effect = dynamic_cast<BasicEffect*>(ieffect.get());

				if (!basic_effect)
					continue;

				Vector3 emissive_color = (is_lighting ? Vector3::Zero : Vector3::One);		// �����F

				basic_effect->SetEmissiveColor(emissive_color);
				basic_effect->SetLightEnabled(0, is_lighting);
			}
		}
	}

	/*---------------------------------------------------------------
	Model3D

	summary:�R���X�g���N�^
	param  :�Ȃ�(void)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	Model3D::Model3D()
		:device_(ActionGame::GetInstance()->GetDevice()),
		 context_(ActionGame::GetInstance()->GetContext()),
		 camera_(ActionGame::GetInstance()->GetCamera()),
		 model_(nullptr),
		 is_visible_(true),
		 is_draw_wireframe_(false)
	{
		this->effect_ = make_unique<EffectFactory>(this->device_.Get());
		this->effect_->SetDirectory(L"Resources");
		this->states_ = make_unique<CommonStates>(this->device_.Get());
	}
}
