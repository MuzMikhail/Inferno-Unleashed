#include "Enemy.h"
#include "Sorc.h"
#include "Deathbody.h"

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
		//! @brief オブジェクト(obj)に近い敵を返す // Возвращение ближайшего к объекту (obj) врага
		//! @param obj
		//! @return 近い敵 // Ближайший враг
		ObjectPtr TrackingNearEnemy( ObjectPtr obj )
		{
			// シーンにいる敵をすべて取得する // Получение всех врагов в сцене
			auto enemies = Scene::GetObjectsPtr<Enemy>();

			// 一番近い敵 // Самый близкий враг
			ObjectPtr near_enemy  = nullptr;
			float	  near_length = 200.0f;

			float3 obj_pos = obj->GetTranslate();

			for( auto enemy : enemies )
			{
				if( near_enemy == nullptr )
				{
					near_enemy = enemy;
					continue;
				}

				float3 enemy_pos = enemy->GetTranslate();

				float len = length( enemy_pos - obj_pos );
				if( len < near_length )
				{
					near_enemy	= enemy;   // 一番近い敵を更新する // Обновляем ближайшего врага
					near_length = len;	   // 距離を更新する // Обновляем расстояние
				}
			}

			return near_enemy;	 // 近い敵を返す // Возвращаем ближайшего врага
		}

		EnemyPtr Enemy::Create( const float3& pos, const float3& front )
		{
			// 敵の作成 // Создание врага
			auto enemy = Scene::CreateObject<Enemy>();
			enemy->SetName( "Enemy" );

			// frontの方向に向ける // Ориентация в направлении front
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			enemy->SetMatrix( mat );

			// posの位置に設定 // Установка позиции врага
			enemy->SetTranslate( pos );

			return enemy;	// 作成した敵を返す // Возвращаем созданного врага
		}

		//! @brief 初期化 // Инициализация
		//! @return 初期化できたかどうか // Удалось ли инициализировать
		bool Enemy::Init()	 // override
		{
			__super::Init();

			// モデルコンポーネント(0.08倍) // Добавление компонента модели (масштаб 0.08)
			auto model = AddComponent<ComponentModel>( "data/Sample/Enemy/char/Zomb.mv1" );

			model->SetScaleAxisXYZ( { 0.08f } );   // スケールを設定する // Установка масштаба

			// アニメーションを設定する // Настройка анимаций
			model->SetAnimation( { { "idle", "data/Sample/Enemy/char/Anim/zombi_idle.mv1", 0, 1.0f },
								   { "walk", "data/Sample/Enemy/char/Anim/zombi_run.mv1", 0, 1.0f },
								   { "death", "data/Sample/Enemy/char/Anim/zombi_death.mv1", 0, 1.0f },
								   { "attack", "data/Sample/Enemy/char/Anim/zombi_attack.mv1", 0, 1.0f },
								   { "dance5", "data/Sample/Player/Anim/Dance5.mv1", 0, 1.0f } } );

			// 衝突カプセルの追加 // Добавление капсулы столкновения
			auto col = AddComponent<ComponentCollisionCapsule>();
			col->SetTranslate( { 0, 0, 0 } );	// 位置を設定する // Установка позиции
			col->SetRadius( 2.5 );				// 半径を設定する // Установка радиуса
			col->SetHeight( 15 );				// 高さを設定する // Установка высоты
			col->SetCollisionGroup(
				ComponentCollision::CollisionGroup::ENEMY );   // 衝突グループを設定する // Установка группы столкновений
			col->UseGravity();								   // 重力を有効にする // Включение гравитации

			// ヒットエフェクトの設定 // Настройка эффекта попадания
			effect_hit_		 = LoadEffekseerEffect( "data/Sample/Effects/shot.efkefc" );
			effect_play_hit_ = PlayEffekseer3DEffect( effect_hit_ );
			float scale		 = 1.5f;   // スケールを設定する // Установка масштаба

			// エフェクトが再生中か確認して再生する // Проверка, воспроизводится ли эффект, и его воспроизведение
			if( IsEffekseer3DEffectPlaying( effect_play_hit_ ) )
			{
				effect_play_hit_ = PlayEffekseer3DEffect( effect_hit_ );
			}

			// エフェクトの位置とスケールを設定する // Установка позиции и масштаба эффекта
			SetPosPlayingEffekseer3DEffect( effect_play_hit_, position_h_.x, position_h_.y, position_h_.z );
			SetScalePlayingEffekseer3DEffect( effect_play_hit_,
											  scale,
											  scale,
											  scale );	 // エフェクトのスケールを設定する // Установка масштаба эффекта

			return true;   // 初期化が成功した場合trueを返す // Возвращаем true, если инициализация прошла успешно
		}


		//bool Enemy::CheckCollision( float3 position, float3 direction, float distance )
		bool Enemy::CheckCollision( float3 position, float3 direction, float distance, float& outSlopeAngle )
		{
			auto colliders = Scene::GetObjectsPtr<ComponentCollision>();

			for( auto collider : colliders )
			{
				if( collider->GetOwnerPtr().get() == this )
					continue;

				// 簡単な衝突判定を行う // Примитивная проверка столкновения путем изменения позиции объекта в данном направлении
				float3 newPos = position + direction * distance;
				if( collider->IsColliding( newPos ) )
				{
					// 衝突した面の法線を取得する // Получение нормали поверхности при столкновении
					float3 surfaceNormal = collider->GetSurfaceNormal( newPos );
					outSlopeAngle		 = acos( dot( surfaceNormal, float3( 0, 1, 0 ) ) ) *
						RadToDeg;	// 傾斜角を計算する // Вычисление угла наклона
					return true;   // 衝突があった場合trueを返す // Возвращаем true, если произошло столкновение
				}
			}

			outSlopeAngle = 0.0f;	// 衝突がなかった場合、傾斜角を0に設定する // Устанавливаем угол наклона в 0, если столкновения не было
			return false;	// 衝突がなかった場合falseを返す // Возвращаем false, если столкновения не было
		}

		float3 Enemy::GetRepulsionVector( float3 position, float3 direction )
		{
			auto colliders = Scene::GetObjectsPtr<ComponentCollision>();

			for( auto collider : colliders )
			{
				if( collider->GetOwnerPtr().get() == this )
					continue;

				float slopeAngle;
				if( CheckCollision( position, direction, 5.0f, slopeAngle ) )
				{
					// 傾斜角が90度に近い場合、方向を反転する // Если угол наклона близок к 90 градусам, инвертируем направление
					if( slopeAngle > 80.0f && slopeAngle < 100.0f )
					{
						return -direction;	 // 反転した方向を返す // Возвращаем инвертированное направление
					}
				}
			}

			return direction;	// 衝突がない場合、元の方向を返す // Возвращаем исходное направление, если столкновения нет
		}

		void Enemy::CorrectPositionIfColliding()
		{
			auto colliders = Scene::GetObjectsPtr<ComponentCollision>();
			auto pos	   = GetTranslate();

			for( auto collider : colliders )
			{
				if( collider->GetOwnerPtr().get() == this )
					continue;

				float slopeAngle;
				if( CheckCollision( pos, float3( 0, -1, 0 ), 1.0f, slopeAngle ) )
				{
					// 衝突が発生した場合、オブジェクトを上に移動する // Если объект оказался внутри коллизии, сдвигаем его вверх
					float3 correction = float3( 0.0f, 1.0f, 0.0f );
					AddTranslate( correction );	  // 位置を修正する // Исправляем позицию
				}
			}
		}
		bool Enemy::FindAlternativePath( float3& move, float3 position, float3 direction )
		{
			float stepAngle = 45.0f;   // 代替経路を探すための回転角度 // Угол поворота для поиска альтернативного пути
			int	  maxSteps	= 360 / stepAngle;	 // 最大ステップ数 // Максимальное количество шагов

			for( int i = 1; i <= maxSteps; ++i )
			{
				float angle = i * stepAngle;
				// 新しい方向を計算する // Вычисляем новое направление
				float3 newDirection = float3( direction.x * cos( angle * DegToRad ) - direction.z * sin( angle * DegToRad ),
											  direction.y,
											  direction.x * sin( angle * DegToRad ) + direction.z * cos( angle * DegToRad ) );

				float slopeAngle;
				if( !CheckCollision( position, newDirection, 5.0f, slopeAngle ) )
				{
					move = normalize( newDirection ) *
						length( move );	  // 新しい有効な方向を設定する // Устанавливаем новое допустимое направление
					return true;   // 新しい経路が見つかった場合trueを返す // Возвращаем true, если нашли новый путь
				}
			}

			return false;	// 代替経路が見つからなかった場合falseを返す // Возвращаем false, если не удалось найти альтернативный путь
		}

		//! @brief 処理更新 // Обновление обработки
		//! @param delta フレーム時間 // Время кадра
		void Enemy::Update( [[maybe_unused]] float delta )	 // override
		{

			auto mdl = GetComponent<ComponentModel>();

			float3 target = GetTranslate();

			// Sorcオブジェクトの位置を取得する // Получаем позицию объекта Sorc
			if( auto sorc = Scene::GetObjectPtr<Sorc>( "Sorc" ) )
				target = sorc->GetTranslate();

			auto pos = GetTranslate();

			float3 move		= ( target - pos );	  // 移動ベクトルを計算する // Вычисляем вектор движения
			float  distance = length( move );	  // 距離を計算する // Вычисляем расстояние

			if( distance > 150 )
			{
				move = { 0, 0, 0 };	  // 遠すぎる場合、移動を停止する // Останавливаем движение, если слишком далеко
				if( mdl && mdl->GetPlayAnimationName() != "idle" )
					mdl->PlayAnimation( "idle", true );	  // アイドルアニメーションを再生する // Воспроизводим анимацию ожидания
			}
			else if( distance <= 10 )
			{
				move = { 0, 0, 0 };	  // 近すぎる場合、移動を停止する // Останавливаем движение, если слишком близко
				timeSinceLastAttack += delta;
				// 攻撃タイマーを確認する // Проверка таймера для атаки
				if( timeSinceLastAttack >= ATTACK_COOLDOWN )
				{
					Attack();					  // 攻撃を実行する // Выполняем атаку
					timeSinceLastAttack = 0.0f;	  // タイマーをリセットする // Сбрасываем таймер
				}
				if( mdl && mdl->GetPlayAnimationName() != "attack" )
					mdl->PlayAnimation( "attack", true );	// 攻撃アニメーションを再生する // Воспроизводим анимацию атаки
			}
			else if( distance > 10 && distance <= 150 )
			{
				move = normalize( move );	// 移動ベクトルを正規化する // Нормализуем вектор движения

				float slopeAngle;
				if( CheckCollision( pos, move, 5.0f, slopeAngle ) )
				{
					// 衝突した場合、代替経路を探す // При столкновении ищем альтернативный путь
					if( !FindAlternativePath( move, pos, move ) )
					{
						move = { 0, 0, 0 };	  // 経路が見つからなければ移動を停止する // Останавливаем движение, если путь не найден
					}
				}

				// 回転を計算する // Вычисляем вращение
				float x		= -move.x;
				float z		= -move.z;
				float theta = atan2( x, z ) * RadToDeg - rot_y_;

				SetRotationAxisXYZ( { 0, theta, 0 } );	 // 回転を設定する // Устанавливаем вращение

				move *= speed_ * ( delta * 200.0f );   // 移動速度を計算する // Вычисляем скорость движения
				if( length( move ).x > 0 )
				{
					if( mdl && mdl->GetPlayAnimationName() != "walk" )
						mdl->PlayAnimation( "walk", true );	  // 歩行アニメーションを再生する // Воспроизводим анимацию ходьбы
				}
			}

			AddTranslate( move );	// 位置を更新する // Обновляем позицию

			CorrectPositionIfColliding();	// 衝突した場合の位置を修正する // Исправляем позицию при столкновении
		}
		void Enemy::Attack()
		{
			static int attackCount = 0;	  // 攻撃回数を記録するカウンター // Счётчик атак для определения первой атаки
			float	   attackCooldown = ( attackCount == 0 ) ?
					 1.0f :
					 1.5f;	 // 最初の攻撃は1秒、以降は1.5秒 // Первая атака через 1 секунду, последующие через 1.5

			float currentTime = GetCurrentTime();	// 現在の時間を取得する // Получаем текущее время
			//	static float lastAttackTime = 0;   // 最後の攻撃時間を記録する // Время последней атаки

			if( currentTime - lastAttackTime >= attackCooldown )
			{
				// Sorcオブジェクトが存在するか確認する // Проверяем, есть ли объект Sorc
				if( auto sorc = Scene::GetObjectPtr<Sorc>( "Sorc" ) )
				{
					sorc->TakeDamage( 2 );	 // Sorcにダメージを与える // Наносим урон Sorc
				}

				// 攻撃アニメーションを再生する // Воспроизводим анимацию атаки
				auto mdl = GetComponent<ComponentModel>();
				if( mdl )
				{
					mdl->PlayAnimation( "attack", false );
				}

				lastAttackTime = currentTime;	// 最後の攻撃時間を更新する // Обновляем время последней атаки
				attackCount++;					// 攻撃回数を増やす // Увеличиваем счётчик атак
			}
		}

		//! @brief GUI処理 // Обработка GUI
		void Enemy::GUI()	// override
		{
			__super::GUI();

			// GUIの描画 // Отрисовка GUI
			ImGui::Begin( GetName().data() );
			{
				ImGui::Separator();

				ImGui::DragFloat( u8"速度",
								  &speed_,
								  0.01f,
								  0.01f,
								  10.0f,
								  "%2.2f" );   // 速度を調整するスライダー // Слайдер для регулировки скорости
			}
			ImGui::End();
		}

		//! @brief 当たりのコールバック // Колбэк при столкновении
		void Enemy::OnHit( const ComponentCollision::HitInfo& hitInfo )	  // override
		{
			// 親クラスのOnHitを呼び出して衝突を処理する // Вызов родительской функции OnHit для обработки столкновения
			__super::OnHit( hitInfo );
			auto mdl = GetComponent<ComponentModel>();

			auto owner = hitInfo.hit_collision_->GetOwnerPtr();
			if( owner->GetNameDefault() == "Shot" )
			{
				if( mdl )
				{
					auto   attach_frame_hit = MV1SearchFrame( mdl->GetModel(), "mixamorig:Spine2" );
					auto   sword_mat		= MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_hit );
					float3 effect_pos		= mul( float4( position_h_, 1.0f ), cast( sword_mat ) ).xyz;

					float scale = 1.0f;

					if( IsEffekseer3DEffectPlaying( effect_play_hit_ ) )
					{
						effect_play_hit_ = PlayEffekseer3DEffect( effect_hit_ );
					}

					SetPosPlayingEffekseer3DEffect( effect_play_hit_,
													effect_pos.x,
													effect_pos.y,
													effect_pos.z );	  // エフェクトの位置を設定する // Устанавливаем позицию эффекта
					SetScalePlayingEffekseer3DEffect( effect_play_hit_,
													  scale,
													  scale,
													  scale );	 // エフェクトのスケールを設定する // Устанавливаем масштаб эффекта
				}

				TakeDamage( 1 );   // ダメージを与える // Наносим урон
			}
		}

		void Enemy::TakeDamage( int damage )
		{
			hitCount_ += damage;   // ダメージ分ヒット数を増やす // Увеличиваем счётчик попаданий на количество урона

			// 死亡判定を行う // Проверяем, достигло ли количество попаданий порога смерти
			if( hitCount_ >= 3 )
			{
				OnDeath();
			}
		}

		void Enemy::OnDeath()
		{
			auto mdl = GetComponent<ComponentModel>();
			if( mdl )
			{
				mdl->PlayAnimation( "death", false );	// 死亡アニメーションを再生する // Воспроизводим анимацию смерти
			}

			// Deathbodyオブジェクトを作成する // Создаём объект Deathbody
			float3 vec				   = mdl ? mdl->GetMatrix().axisZ().xyz : float3( 0, 0, 0 );
			auto   attach_frame_bd	   = MV1SearchFrame( mdl->GetModel(), "Ch25" );
			auto   bone_mat			   = MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_bd );
			float3 bd_pos			   = mul( float4( position_h_, 1.0f ), cast( bone_mat ) ).xyz;
			auto   enemyRotationMatrix = this->GetMatrix();

			auto db = Scene::CreateObject<Deathbody>();
			db->SetPositionAndDirection( bd_pos, vec );
			db->SetMatrix( enemyRotationMatrix );

			// 移動を停止し、非アクティブにする // Останавливаем движение и деактивируем
			speed_ = 0.0f;
			Scene::ReleaseObject( SharedThis() );

			RemoveComponent<ComponentCollisionCapsule>();	// コリジョンカプセルを削除する // Удаляем компонент капсулы столкновения
			RemoveComponent<ComponentModel>();	 // モデルコンポーネントを削除する // Удаляем компонент модели
		}

		//! @brief スピードを設定する // Устанавливает скорость
		//! @param s スピード // Скорость
		void Enemy::SetSpeed( float s )
		{
			speed_ = s;
		}

		//! @brief スピードを取得する // Получает скорость
		//! @return 現在のスピード // Текущая скорость
		float Enemy::GetSpeed()
		{
			return speed_;
		}

		//! @brief 変数として扱う関数 // Возвращает переменную скорости
		//! @detail Set/Get系と比べデバックはしにくい // Труднее отлаживать по сравнению с методами Set/Get
		//! @return 現在のスピードの変数(speed_) // Переменная текущей скорости (speed_)
		float& Enemy::Speed()
		{
			return speed_;
		}


	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE とセットで必要 (Editor上でCreateObjectするため) // Требуется в паре с BP_OBJECT_TYPE для создания объектов в редакторе
BP_OBJECT_IMPL( Sample::GameSample::Enemy, u8"Enemy" );

// オブジェクトセーブ構造を追加します // Добавление структуры для сохранения объектов
CEREAL_REGISTER_TYPE( Sample::GameSample::Enemy )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Enemy )	// 多態関係を登録する // Регистрация полиморфных отношений
