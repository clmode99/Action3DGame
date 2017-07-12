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

/* メンバ変数の定義 */
namespace CloverLib
{
	/*---------------------------------------------------------------
	Create

	summary:生成する
	param  :読み込むファイル名(const wchar_t*)、表示するか(bool)
	return :スプライト(unique_ptr<CloverLib::Model3D>)
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
		this->model_ = Model::CreateFromCMO(this->device_.Get(), file_name, *(this->effect_));
	}

	/*---------------------------------------------------------------
	Draw

	summary:描画
	param  :なし(void)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::Draw()
	{
		if (!(this->is_visible_))
			return;

		/* 行列計算 */
		Matrix rotate = Matrix::CreateFromQuaternion(this->rotate_);
		Matrix world = this->scale_ * rotate * this->trans_;

		/* 描画 */
		this->model_->Draw(this->context_.Get(), *(this->states_),
							world, this->camera_->GetView(), this->camera_->GetProjection(),
							this->is_draw_wireframe_);
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
	SetRotateYPR

	summary:回転(ヨー・ピッチ・ロール版)
	param  :ヨー(float)、ピッチ(float)、ロール(float)
	return :なし(void)
	---------------------------------------------------------------*/
	void Model3D::SetRotateYPR(float yaw, float pitch, float roll)
	{
		this->rotate_ = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
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
	SetLighting

	summary:ライトの切替
	param  :オンオフ(bool)
	return :なし(void)

	※参考資料：Obj3D.cpp - Obj3D::DisableLighting()
	---------------------------------------------------------------*/
	void Model3D::SetLighting(bool is_lighting)
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

				Vector3 emissive_color = (is_lighting ? Vector3::Zero : Vector3::One);		// 発光色

				basic_effect->SetEmissiveColor(emissive_color);
				basic_effect->SetLightEnabled(0, is_lighting);
			}
		}
	}

	/*---------------------------------------------------------------
	Model3D

	summary:コンストラクタ
	param  :なし(void)
	return :なし(void)
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
