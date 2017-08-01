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

/* �ÓI�����o�ϐ��̒�` */
map<const wchar_t*, shared_ptr<Model>> CloverLib::Model3D::model_map_;

/* �����o�ϐ��̒�` */
namespace CloverLib
{
	/*---------------------------------------------------------------
	Create

	summary:��������
	param  :�ǂݍ��ރt�@�C����(const wchar_t*)�A�\�����邩(bool)
	return :���f��(unique_ptr<CloverLib::Model3D>)
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
		/* ���ɓ����ꏊ�ɂ��铯���t�@�C���͎g���� */
		auto data = this->model_map_.find(file_name);
		
		if (data != this->model_map_.end())			// �����t�@�C������������
			this->model_ = data->second;
		else
		{
			this->model_ = Model::CreateFromCMO(this->device_.Get(), file_name, *(this->effect_));
			this->model_map_.insert(std::make_pair(file_name, this->model_));
		}
	}

	/*---------------------------------------------------------------
	Draw

	summary:�`��
	param  :�Ȃ�(void)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::Draw(STATE state)
	{
		this->CalcWorldMatrix();

		if (!(this->is_visible_))
			return;

		function<void()> state_func;

		switch (state)
		{
		/* ���� */
		case STATE::NORMAL:
			this->Draw(state_func);

			/* �q�̕`�� */
			for (auto& v : this->children_)
				v->Draw();
			break;

		/* ���Z */
		case STATE::ADD:
			state_func = [&] {this->context_->OMSetBlendState(this->add_state_.Get(), nullptr, 0xffffff); };

			this->Draw(state_func);

			/* �q�̕`�� */
			for (auto& v : this->children_)
				v->Draw(STATE::ADD);
			break;

		/* ���Z */
		case STATE::SUBTRACT:
			state_func = [&] {this->context_->OMSetBlendState(this->subtract_state_.Get(), nullptr, 0xffffff); };

			this->Draw(state_func);

			/* �q�̕`�� */
			for (auto& v : this->children_)
				v->Draw(STATE::SUBTRACT);
			break;
		}
	}

	/*---------------------------------------------------------------
	AddChild

	summary:�q��ǉ�
	param  :�q(Model3D*)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::AddChild(Model3D* child)
	{
		this->children_.push_back(child);
		child->SetParent(this);
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
	GetTranslation

	summary:�ړ��ʂ̎擾
	param  :�Ȃ�(void)
	return :�ړ���(const DirectX::SimpleMath::Vector3&)
	---------------------------------------------------------------*/
	const Vector3& Model3D::GetTranslation()
	{
		/* �s�񂩂�x�N�g���̒l�����o�� */
		this->trans_vec_.x = this->trans_._41;
		this->trans_vec_.y = this->trans_._42;
		this->trans_vec_.z = this->trans_._43;

		return this->trans_vec_;
	}

	/*---------------------------------------------------------------
	SetOffset

	summary:�I�t�Z�b�g�ݒ�
	param  :�I�t�Z�b�g(const DirectX::SimpleMath::Vector3&)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetOffset(const Vector3& offset)
	{
		this->offset_ = offset;
		this->SetTranslation(this->trans_vec_ + this->offset_);
	}

	/*---------------------------------------------------------------
	SetDiffuseColor

	summary:�g�U���ˌ��̐ݒ�
	param  :�F(RGB:0.0f�`1.0f)(const DirectX::SimpleMath::Vector3&)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetDiffuseColor(const Vector3& color)
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

				auto ieffect = model_mesh_part->effect;
				auto basic_effect = dynamic_cast<BasicEffect*>(ieffect.get());

				if (!basic_effect)
					continue;

				basic_effect->SetDiffuseColor(color);
			}
		}

		/* �q�̐ݒ� */
		for (auto& v : this->children_)
			v->SetDiffuseColor(color);
	}

	/*---------------------------------------------------------------
	SetSpecular

	summary:���ʔ��ˌ��̐ؑ�
	param  :�I���I�t(bool)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::SetSpecular(bool flag)
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

				auto ieffect = model_mesh_part->effect;
				auto basic_effect = dynamic_cast<BasicEffect*>(ieffect.get());

				if (!basic_effect)
					continue;

				Vector3 specular_color = (flag ? Vector3::One : Vector3::Zero);		// ���ʔ��ˌ�

				basic_effect->SetSpecularColor(specular_color);
			}
		}

		/* �q�̐ݒ� */
		for (auto& v : this->children_)
			v->SetSpecular(flag);
	}

	/*---------------------------------------------------------------
	SetLighting

	summary:���C�g�̐ؑ�
	param  :�I���I�t(bool)
	return :�Ȃ�(void)

	���Q�l�����FObj3D.cpp - Obj3D::DisableLighting()
	---------------------------------------------------------------*/
	void Model3D::SetLighting(bool flag)
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

				Vector3 emissive_color = (flag ? Vector3::Zero : Vector3::One);		// �����F

				basic_effect->SetEmissiveColor(emissive_color);
				basic_effect->SetLightEnabled(0, flag);
			}
		}

		/* �q�̐ݒ� */
		for (auto& v : this->children_)
			v->SetLighting(flag);
	}

	/*---------------------------------------------------------------
	Model3D

	summary:�R���X�g���N�^
	param  :�Ȃ�(void)
	return :���݂��Ȃ�
	---------------------------------------------------------------*/
	Model3D::Model3D()
		:device_(ActionGame::GetInstance()->GetDevice()),
		 context_(ActionGame::GetInstance()->GetContext()),
		 camera_(ActionGame::GetInstance()->GetCamera()),
		 model_(nullptr),
		 parent_(nullptr),
		 is_visible_(true),
		 is_draw_wireframe_(false)
	{
		this->effect_ = make_unique<EffectFactory>(this->device_.Get());
		this->effect_->SetDirectory(L"Resources");
		this->states_ = make_unique<CommonStates>(this->device_.Get());

		this->CreateState();
	}

	/*---------------------------------------------------------------
	Draw

	summary:�`��(private��)
	param  :�X�e�[�g(const std::function<void()>&)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::Draw(const function<void()>& state)
	{
		this->model_->Draw(this->context_.Get(), *(this->states_),
			this->world_, this->camera_->GetView(), this->camera_->GetProjection(),
			this->is_draw_wireframe_, state);
	}

	/*---------------------------------------------------------------
	CreateState

	summary:�e�X�e�[�g�쐬
	param  :�Ȃ�(void)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::CreateState()
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

		/* ���Z */
		desc.AlphaToCoverageEnable       = false;
		desc.IndependentBlendEnable      = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend  = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOp   = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		this->device_->CreateBlendState(&desc, this->add_state_.GetAddressOf());

		/* ���Z */
		desc.AlphaToCoverageEnable       = false;
		desc.IndependentBlendEnable      = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend  = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOp   = D3D11_BLEND_OP_REV_SUBTRACT;
		desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_REV_SUBTRACT;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		this->device_->CreateBlendState(&desc, this->subtract_state_.GetAddressOf());
	}

	/*---------------------------------------------------------------
	CalcWorldMatrix

	summary:���[���h�s��v�Z
	param  :�Ȃ�(void)
	return :�Ȃ�(void)
	---------------------------------------------------------------*/
	void Model3D::CalcWorldMatrix()
	{
		Matrix rotate = Matrix::CreateFromQuaternion(this->rotate_);
		this->world_ = this->scale_ * rotate * this->trans_;

		if (this->parent_)		// �e������Ȃ�
			this->world_ *= this->parent_->world_;
	}
}
