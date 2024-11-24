#include "Shot.h"
#include "Sorc.h"

#include <System/Scene.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentModel.h>
#include <System/Graphics/Animation.h>

namespace Sample
{
	namespace GameSample
	{
		// エディタ上で発生させる // Реализация объекта Shot, созданного в редакторе.
		BP_OBJECT_IMPL( Shot, u8"GameSample/Shot" );

		// ショットオブジェクトを指定の位置と方向で作成する // Создание объекта Shot с заданным положением и направлением
		ShotPtr Shot::Create( const float3& handPos, const float3& vec )
		{
			// 弾の作成 // Создание снаряда
			auto shot = Scene::CreateObject<Shot>();
			shot->SetDelay( 0.5f );	  // ディレイの設定 // Установка задержки

			shot->SetPositionAndDirection( handPos, vec );	 // 位置と方向の設定 // Установка позиции и направления

			return shot;   // オブジェクトを返す // Возврат объекта
		}

		bool Shot::Init()
		{
			Super::Init();

			SetName( "Shot" );	 // 名前の設定 // Установка имени
			auto model = AddComponent<ComponentModel>( "data/Sample/PlayerSorc/fireball.mv1" );
			model->SetScaleAxisXYZ( { 0.01f } );   // スケールの設定 // Установка масштаба
			if( model_ == -1 )
			{
				return false;	// モデルの読み込み失敗時の処理 // Обработка неудачной загрузки модели
			}

			auto col = AddComponent<ComponentCollisionSphere>();
			col->SetRadius( radius_ )						   // 半径の設定 // Установка радиуса
				->SetTranslate( float3( 0.0f, 1.0f, 0.0f ) )   // トランスレートの設定 // Установка трансляции
				->SetHitCollisionGroup(
					(u32)ComponentCollision::CollisionGroup::ENEMY |
					(u32)ComponentCollision::CollisionGroup::GROUND );	 // 衝突グループの設定 // Установка группы коллизий

			effect_fb_ = LoadEffekseerEffect( "data/Sample/Effects/MagicFire5.efkefc" );   // エフェクトの読み込み // Загрузка эффекта
			effect_play_fb_ = PlayEffekseer3DEffect( effect_fb_ );	 // エフェクトの再生 // Воспроизведение эффекта

			float scale = 1.5f;

			if( IsEffekseer3DEffectPlaying( effect_play_fb_ ) )
			{
				effect_play_fb_ = PlayEffekseer3DEffect( effect_fb_ );	 // エフェクトの再生 // Воспроизведение эффекта
			}

			SetPosPlayingEffekseer3DEffect( effect_play_fb_, position_fb_.x, position_fb_.y, position_fb_.z );
			SetScalePlayingEffekseer3DEffect( effect_play_fb_, scale, scale, scale );	// スケールの設定 // Установка масштаба эффекта
		}

		void Shot::Update( float delta )
		{
			if( time_since_creation_ >= delay_ )
			{
				AddTranslate( vec_ * speed_ * delta );	 // 位置の更新 // Обновление позиции
			}
			else
			{
				time_since_creation_ += delta;	 // 時間の更新 // Обновление времени
			}

			MV1SetPosition( model_, cast( GetTranslate() ) );	// モデル位置の設定 // Установка позиции модели
			MV1DrawModel( model_ );								// モデルの描画 // Отрисовка модели
			auto mdl = GetComponent<ComponentModel>();

			if( mdl )
			{
				auto attach_frame_fb = MV1SearchFrame( mdl->GetModel(), "Сфера" );	 // フレームの検索 // Поиск фрейма
				auto   sword_mat_fb	 = MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_fb );
				float3 effect_pos_fb = mul( float4( position_fb_, 1.0f ), cast( sword_mat_fb ) ).xyz;

				if( IsEffekseer3DEffectPlaying( effect_play_fb_ ) )
				{
					effect_play_fb_ = PlayEffekseer3DEffect( effect_fb_ );	 // エフェクトの再生 // Воспроизведение эффекта
				}

				SetPosPlayingEffekseer3DEffect( effect_play_fb_,
												effect_pos_fb.x,
												effect_pos_fb.y,
												effect_pos_fb.z );	 // エフェクト位置の設定 // Установка позиции эффекта
			}
		}

		void Shot::Draw()
		{
		}

		//! @brief 当たりのコールバック // Колбэк при столкновении
		//! @param hitInfo // Информация о столкновении
		void Shot::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )
		{
			int	 en_hp = 3;	  // 敵のHP // Здоровье врага
			auto owner = hitInfo.hit_collision_->GetOwnerPtr();
			if( owner->GetNameDefault() == "Enemy" )
			{
				Scene::ReleaseObject( SharedThis() );	// オブジェクトの解放 // Освобождение объекта
				en_hp--;								// 敵のHPを減少させる // Уменьшение здоровья врага
				StopEffekseer3DEffect( effect_play_fb_ );	// エフェクトの停止 // Остановка эффекта
				if( en_hp >= 4 )
				{
					// owner->SetDestroyed( true );
				}
			}
			if( owner->GetNameDefault() == "Ground" )
			{
				Scene::ReleaseObject( SharedThis() );		// オブジェクトの解放 // Освобождение объекта
				StopEffekseer3DEffect( effect_play_fb_ );	// エフェクトの停止 // Остановка эффекта
			}
		}

		void Shot::SetPositionAndDirection( const float3& handPos, const float3& vec )
		{
			SetTranslate( handPos + float3( 0, -1, 0 ) );	// 位置の設定 // Установка позиции
			vec_ = normalize( vec );						// ベクトルの正規化 // Нормализация вектора
		}

		void Shot::SetDelay( float delay )
		{
			delay_ = delay;	  // ディレイの設定 // Установка задержки
		}

	}	// namespace GameSample
}	// namespace Sample
