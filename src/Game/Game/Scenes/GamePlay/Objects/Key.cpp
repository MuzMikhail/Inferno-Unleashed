#include "Sorc.h"
#include "Key.h"

#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentTargetTracking.h>
#include <System/Component/ComponentSpringArm.h>

#include <cmath>

namespace Sample
{
	namespace GameSample
	{

		KeyPtr Key::Create( const float3& pos, const float3& front )
		{
			// 箱の作成 // Создание объекта
			auto goal = Scene::CreateObject<Key>();
			goal->SetName( "Key" );	  // 名前の設定 // Установление имени объекта

			// vecの方向に向ける // Установление направления по вектору
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			goal->SetMatrix( mat );	  // 行列の設定 // Установление матрицы

			// posの位置に設定 // Установление позиции объекта
			goal->SetTranslate( pos );

			return goal;   // オブジェクトを返す // Возврат объекта
		}

		bool Key::Init()   // override
		{
			__super::Init();

			// モデルコンポーネント(0.08倍) // Добавление компонента модели (масштаб 0.05)
			auto model = AddComponent<ComponentModel>( "data/Sample/Key/source/death_mask.mv1" );

			model->SetScaleAxisXYZ( { 0.05f } );   // スケールの設定 // Установление масштаба модели
			model->SetTranslate( { 0, 6, 0 } );	   // トランスレートの設定 // Установление трансляции

			auto col = AddComponent<ComponentCollisionSphere>();
			col->SetTranslate( { 0, 0, 0 } );	// トランスレートの設定 // Установление трансляции
			col->SetRadius( 5 );				// 半径の設定 // Установление радиуса
			col->SetCollisionGroup( ComponentCollision::CollisionGroup::KEY );	 // 衝突グループの設定 // Установление группы коллизий
			col->UseGravity();	 // 重力の使用 // Использование гравитации

			effect_k_ = LoadEffekseerEffect( "data/Sample/Effects/portal.efkefc" );	  // エフェクトの読み込み // Загрузка эффекта
			effect_play_k_ = PlayEffekseer3DEffect( effect_k_ );   // エフェクトの再生 // Воспроизведение эффекта
			float scale	   = 1.5f;

			if( IsEffekseer3DEffectPlaying( effect_play_k_ ) )
			{
				effect_play_k_ = PlayEffekseer3DEffect( effect_k_ );   // エフェクトの再生 // Воспроизведение эффекта
			}

			SetPosPlayingEffekseer3DEffect( effect_play_k_, position_k_.x, position_k_.y, position_k_.z );
			SetScalePlayingEffekseer3DEffect( effect_play_k_, scale, scale, scale );   // スケールの設定 // Установка масштаба эффекта

			return true;   // 初期化の結果を返す // Возврат результата инициализации
		}

		//! @brief 処理更新 // Обновление обработки
		//! @param delta フレーム時間 // Время кадра
		void Key::Update( [[maybe_unused]] float delta )   // override
		{
			auto mdl = GetComponent<ComponentModel>();
			if( mdl )
			{
				auto   attach_frame_fire = MV1SearchFrame( mdl->GetModel(),
														   "Group16913" );	 // エフェクトのフレームを探す // Поиск фрейма для эффекта
				auto   sword_mat_fire	 = MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_fire );
				float3 effect_pos_f		 = mul( float4( position_k_, 1.0f ), cast( sword_mat_fire ) ).xyz;

				float scale = 1.0f;

				if( IsEffekseer3DEffectPlaying( effect_play_k_ ) )
				{
					effect_play_k_ = PlayEffekseer3DEffect( effect_k_ );   // エフェクトの再生 // Воспроизведение эффекта
				}

				SetPosPlayingEffekseer3DEffect( effect_play_k_, effect_pos_f.x, effect_pos_f.y - 3, effect_pos_f.z );
				SetScalePlayingEffekseer3DEffect( effect_play_k_, scale, scale, scale );   // スケールの設定 // Установка масштаба эффекта
			}
		}

		//! @brief 当たりのコールバック // Колбэк при столкновении
		//! @param hitInfo // Информация о столкновении
		void Key::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )	 // override
		{
			// 当たりで移動させる(これが無ければめり込みます) // Обработка столкновения (без этого объект проваливается)
			__super::OnHit( hitInfo );
			auto owner = hitInfo.hit_collision_->GetOwnerPtr();
			if( owner->GetNameDefault() == "Sorc" )
			{
				if( auto sorc = Scene::GetObjectPtr<Sorc>( "Sorc" ) )
				{
					sorc->maskAct = true;					   // マスクのアクティブ化 // Активация маски
					Scene::ReleaseObject( SharedThis() );	   // オブジェクトの解放 // Освобождение объекта
					StopEffekseer3DEffect( effect_play_k_ );   // エフェクトの停止 // Остановка эффекта
				}
			}
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE とセットで必要 (Editor上でCreateObjectするため) // Требуется с BP_OBJECT_TYPE для создания объекта в редакторе
BP_OBJECT_IMPL( Sample::GameSample::Key, u8"Key" );

// オブジェクトセーブ構造を追加します // Добавление структуры для сохранения объекта
CEREAL_REGISTER_TYPE( Sample::GameSample::Key )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Key )
