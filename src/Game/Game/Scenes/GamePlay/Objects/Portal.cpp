﻿#include "Sorc.h"
#include "Portal.h"
#include "../../Over/Goal.h"

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

		PortalPtr Portal::Create( const float3& pos, const float3& front )
		{
			// 箱の作成 // Создание объекта
			auto goal = Scene::CreateObject<Portal>();
			goal->SetName( "Portal" );	 // 名前の設定 // Установление имени объекта

			// vecの方向に向ける // Установление направления по вектору
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			goal->SetMatrix( mat );	  // 行列の設定 // Установление матрицы

			// posの位置に設定 // Установление позиции объекта
			goal->SetTranslate( pos );

			return goal;   // オブジェクトを返す // Возврат объекта
		}

		bool Portal::Init()	  // override
		{
			__super::Init();

			// モデルコンポーネント(0.08倍) // Добавление компонента модели (масштаб 0.1)
			auto model = AddComponent<ComponentModel>( "data/Sample/Portal/DragonJawPortal2.mv1" );

			model->SetScaleAxisXYZ( { 0.1f } );	  // スケールの設定 // Установление масштаба модели
			model->SetTranslate( { 0, 6, 0 } );	  // トランスレートの設定 // Установление трансляции

			auto col = AddComponent<ComponentCollisionCapsule>();
			col->SetTranslate( { 0, 0, 0 } );	// トランスレートの設定 // Установление трансляции
			col->SetRadius( 5 );				// 半径の設定 // Установление радиуса
			col->SetHeight( 15 );				// 高さの設定 // Установление высоты
			col->SetCollisionGroup( ComponentCollision::CollisionGroup::PORTAL );	// 衝突グループの設定 // Установление группы коллизий
			//col->UseGravity();	 // 重力の使用 // Использование гравитации
			col->SetMass( -1.0f );	 // 質量の設定 (負の値で固定) // Установка массы (отрицательное значение делает объект неподвижным)
			effect_p_ = LoadEffekseerEffect( "data/Sample/Effects/portal.efkefc" );	  // エフェクトの読み込み // Загрузка эффекта
			effect_play_p_ = PlayEffekseer3DEffect( effect_p_ );   // エフェクトの再生 // Воспроизведение эффекта
			float scale	   = 1.5f;

			if( IsEffekseer3DEffectPlaying( effect_play_p_ ) )
			{
				effect_play_p_ = PlayEffekseer3DEffect( effect_p_ );   // エフェクトの再生 // Воспроизведение эффекта
			}

			SetPosPlayingEffekseer3DEffect( effect_play_p_, position_.x, position_.y, position_.z );
			SetScalePlayingEffekseer3DEffect( effect_play_p_, scale, scale, scale );   // スケールの設定 // Установка масштаба эффекта

			return true;   // 初期化の結果を返す // Возврат результата инициализации
		}

		//! @brief 処理更新 // Обновление обработки
		//! @param delta フレーム時間 // Время кадра
		void Portal::Update( [[maybe_unused]] float delta )	  // override
		{
			auto mdl = GetComponent<ComponentModel>();
			if( mdl )
			{
				auto attach_frame_fire = MV1SearchFrame( mdl->GetModel(), "mesh01" );	// フレームの検索 // Поиск фрейма
				auto sword_mat_fire	   = MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_fire );
				float3 effect_pos_f	   = mul( float4( position_, 1.0f ), cast( sword_mat_fire ) ).xyz;

				float scale = 1.5f;
				auto  sorc	= Scene::GetObjectPtr<Sorc>( "Sorc" );
				if( sorc->maskAct == true )	  // Sorcがマスクを持っている場合 // Если Sorc активировал маску
				{
					if( IsEffekseer3DEffectPlaying( effect_play_p_ ) )
					{
						effect_play_p_ = PlayEffekseer3DEffect( effect_p_ );   // エフェクトの再生 // Воспроизведение эффекта
					}

					SetPosPlayingEffekseer3DEffect( effect_play_p_, effect_pos_f.x, effect_pos_f.y, effect_pos_f.z );
					SetScalePlayingEffekseer3DEffect( effect_play_p_,
													  scale,
													  scale,
													  scale );	 // スケールの設定 // Установка масштаба эффекта
				}
			}
		}

		//! @brief 当たりのコールバック // Колбэк при столкновении
		//! @param hitInfo // Информация о столкновении
		void Portal::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )	// override
		{
			// 当たりで移動させる(これが無ければめり込みます) // Обработка столкновения (без этого объект проваливается)
			__super::OnHit( hitInfo );
			auto owner = hitInfo.hit_collision_->GetOwnerPtr();
			if( auto sorc = Scene::GetObjectPtr<Sorc>( "Sorc" ) )
			{
				if( owner->GetNameDefault() == "Sorc" )
				{
					if( sorc->maskAct == true )	  // Sorcがマスクを持っている場合 // Если Sorc активировал маску
					{
						StopEffekseer3DEffect( effect_play_p_ );   // エフェクトの停止 // Остановка эффекта
					}
					//Scene::Change( Scene::GetScene<Goal>() );
				}
			}
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE とセットで必要 (Editor上でCreateObjectするため) // Требуется с BP_OBJECT_TYPE для создания объекта в редакторе
BP_OBJECT_IMPL( Sample::GameSample::Portal, u8"Portal" );

// オブジェクトセーブ構造を追加します // Добавление структуры для сохранения объекта
CEREAL_REGISTER_TYPE( Sample::GameSample::Portal )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Portal )