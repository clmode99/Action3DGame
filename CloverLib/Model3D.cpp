/*-------------------------------------
3Dモデルクラス(Model3D.cpp)

date  :2017.06.02
Author:Miyu Hara
-------------------------------------*/
/* インクルードファイル */
#include "Model3D.h"
#include "../Game/ActionGame.h"

/* 名前空間の使用 */
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/* 静的メンバ変数の定義 */
map<const wchar_t*, shared_ptr<Model>> CloverLib::Model3D::model_map_;

/* メンバ変数の定義 */
namespace CloverLib
{
	/*---------------------------------------------------------------
	Create

	summary:生成する
	param  :読み込むファイル名(const wchar_t*)、表示するか(bool)
	return :モデル(unique_ptr<CloverLib::Model3D>)
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

	summary:デストラクタ
	param  :存在しない
	return :存在しない
	---------------------------------------------------------------*/
	Model3D::~Model3D()
	{}

	/*---------------------------------------------------------------
	Model3D::LoadModelCMO

	summary:CMOモデルを読み込む
	param  :ファイル名(const wchar_t*)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::LoadModelCMO(const wchar_t* file_name)
	{
		/* 既に同じ場所にある同じファイルは使い回す */
		auto data = this->model_map_.find(file_name);
		
		if (data != this->model_map_.end())			// 同じファイルが見つかった
			this->model_ = data->second;
		else
		{
			this->model_ = Model::CreateFromCMO(this->device_.Get(), file_name, *(this->effect_));
			this->model_map_.insert(std::make_pair(file_name, this->model_));
		}
	}

	/*---------------------------------------------------------------
	Draw

	summary:描画
	param  :なし(void)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::Draw(STATE state)
	{
		this->CalcWorldMatrix();

		if (!(this->is_visible_))
			return;

		function<void()> state_func;

		switch (state)
		{
		/* 普通 */
		case STATE::NORMAL:
			this->Draw(state_func);

			/* 子の描画 */
			for (auto& v : this->children_)
				v->Draw();
			break;

		/* 加算 */
		case STATE::ADD:
			state_func = [&] {this->context_->OMSetBlendState(this->add_state_.Get(), nullptr, 0xffffff); };

			this->Draw(state_func);

			/* 子の描画 */
			for (auto& v : this->children_)
				v->Draw(STATE::ADD);
			break;

		/* 減算 */
		case STATE::SUBTRACT:
			state_func = [&] {this->context_->OMSetBlendState(this->subtract_state_.Get(), nullptr, 0xffffff); };

			this->Draw(state_func);

			/* 子の描画 */
			for (auto& v : this->children_)
				v->Draw(STATE::SUBTRACT);
			break;
		}
	}

	/*---------------------------------------------------------------
	AddChild

	summary:子を追加
	param  :子(Model3D*)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::AddChild(Model3D* child)
	{
		this->children_.push_back(child);
		child->SetParent(this);
	}

	/*---------------------------------------------------------------
	SetRotate

	summary:回転(float版)
	param  :回転させたい角度(float, float, float)
	return :なし(void)
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

	summary:回転(Matrix版)
	param  :回転行列(DirectX::SimpleMath::Matrix&)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::SetRotate(const Matrix& rotate)
	{
		this->rotate_ = Quaternion::CreateFromRotationMatrix(rotate);
	}

	/*---------------------------------------------------------------
	SetTranslation

	summary:移動(float版)
	param  :移動させたい座標(float, float, float)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::SetTranslation(float x, float y, float z)
	{
		Vector3 trans = Vector3(x, y, z);
		SetTranslation(trans);
	}

	/*---------------------------------------------------------------
	SetTranslation

	summary:移動(Vector3版)
	param  :移動行列(DirectX::SimpleMath::Vector3&)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::SetTranslation(const Vector3& trans)
	{
		this->trans_ = Matrix::CreateTranslation(trans);
	}

	/*---------------------------------------------------------------
	GetTranslation

	summary:移動量の取得
	param  :なし(void)
	return :移動量(const DirectX::SimpleMath::Vector3&)
	---------------------------------------------------------------*/
	const Vector3& Model3D::GetTranslation()
	{
		/* 行列からベクトルの値を取り出す */
		this->trans_vec_.x = this->trans_._41;
		this->trans_vec_.y = this->trans_._42;
		this->trans_vec_.z = this->trans_._43;

		return this->trans_vec_;
	}

	/*---------------------------------------------------------------
	SetOffset

	summary:オフセット設定
	param  :オフセット(const DirectX::SimpleMath::Vector3&)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::SetOffset(const Vector3& offset)
	{
		this->offset_ = offset;
		this->SetTranslation(this->trans_vec_ + this->offset_);
	}

	/*---------------------------------------------------------------
	SetDiffuseColor

	summary:拡散反射光の設定
	param  :色(RGB:0.0f～1.0f)(const DirectX::SimpleMath::Vector3&)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::SetDiffuseColor(const Vector3& color)
	{
		/* メッシュ分のループ */
		for (auto& mesh : this->model_->meshes)
		{
			auto model_mesh = mesh.get();
			assert(model_mesh);

			/* メッシュパーツ分のループ */
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

		/* 子の設定 */
		for (auto& v : this->children_)
			v->SetDiffuseColor(color);
	}

	/*---------------------------------------------------------------
	SetSpecular

	summary:鏡面反射光の切替
	param  :オンオフ(bool)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::SetSpecular(bool flag)
	{
		/* メッシュ分のループ */
		for (auto& mesh : this->model_->meshes)
		{
			auto model_mesh = mesh.get();
			assert(model_mesh);

			/* メッシュパーツ分のループ */
			for (auto& mesh_part : model_mesh->meshParts)
			{
				auto model_mesh_part = mesh_part.get();
				assert(model_mesh_part);

				auto ieffect = model_mesh_part->effect;
				auto basic_effect = dynamic_cast<BasicEffect*>(ieffect.get());

				if (!basic_effect)
					continue;

				Vector3 specular_color = (flag ? Vector3::One : Vector3::Zero);		// 鏡面反射光

				basic_effect->SetSpecularColor(specular_color);
			}
		}

		/* 子の設定 */
		for (auto& v : this->children_)
			v->SetSpecular(flag);
	}

	/*---------------------------------------------------------------
	SetLighting

	summary:ライトの切替
	param  :オンオフ(bool)
	return :なし(void)

	※参考資料：Obj3D.cpp - Obj3D::DisableLighting()
	---------------------------------------------------------------*/
	void Model3D::SetLighting(bool flag)
	{
		/* メッシュ分のループ */
		for (auto& mesh : this->model_->meshes)
		{
			auto model_mesh = mesh.get();
			assert(model_mesh);

			/* メッシュパーツ分のループ */
			for (auto& mesh_part : model_mesh->meshParts)
			{
				auto model_mesh_part = mesh_part.get();
				assert(model_mesh_part);

				auto ieffect      = model_mesh_part->effect;
				auto basic_effect = dynamic_cast<BasicEffect*>(ieffect.get());

				if (!basic_effect)
					continue;

				Vector3 emissive_color = (flag ? Vector3::Zero : Vector3::One);		// 発光色

				basic_effect->SetEmissiveColor(emissive_color);
				basic_effect->SetLightEnabled(0, flag);
			}
		}

		/* 子の設定 */
		for (auto& v : this->children_)
			v->SetLighting(flag);
	}

	/*---------------------------------------------------------------
	Model3D

	summary:コンストラクタ
	param  :なし(void)
	return :存在しない
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

	summary:描画(private版)
	param  :ステート(const std::function<void()>&)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::Draw(const function<void()>& state)
	{
		this->model_->Draw(this->context_.Get(), *(this->states_),
			this->world_, this->camera_->GetView(), this->camera_->GetProjection(),
			this->is_draw_wireframe_, state);
	}

	/*---------------------------------------------------------------
	CreateState

	summary:各ステート作成
	param  :なし(void)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::CreateState()
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

		/* 加算 */
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

		/* 減算 */
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

	summary:ワールド行列計算
	param  :なし(void)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::CalcWorldMatrix()
	{
		Matrix rotate = Matrix::CreateFromQuaternion(this->rotate_);
		this->world_ = this->scale_ * rotate * this->trans_;

		if (this->parent_)		// 親がいるなら
			this->world_ *= this->parent_->world_;
	}
}
