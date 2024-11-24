#include "Enemy.h"
#include "Skelet.h"
#include "Sorc.h"

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
		SkeletPtr Skelet::Create( const float3& pos, const float3& front )
		{
			// オブジェクトの作成 // Создание объекта
			auto skel = Scene::CreateObject<Skelet>();
			skel->SetName( "Skelet" );	 // 名前の設定 // Установление имени объекта

			// frontの方向に向ける // Установление направления по вектору front
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			skel->SetMatrix( mat );	  // 行列の設定 // Установление матрицы

			// posの位置に設定 // Установление позиции объекта
			skel->SetTranslate( pos );

			return skel;   // オブジェクトを返す // Возврат объекта
		}

		bool Skelet::Init()	  // override
		{
			__super::Init();

			// モデルコンポーネント(0.08倍) // Добавление компонента модели (масштаб 0.08)
			auto model = AddComponent<ComponentModel>( "data/Sample/Skelet/Skelet.mv1" );

			model->SetScaleAxisXYZ( { 0.08f } );   // スケールの設定 // Установление масштаба модели

			// アニメーションの設定 // Настройка анимаций
			model->SetAnimation( { { "idle", "data/Sample/Skelet/Anim/Idle.mv1", 0, 1.0f },
								   { "run", "data/Sample/Skelet/Anim/run.mv1", 0, 1.0f },
								   { "attack", "data/Sample/Skelet/Anim/attack3.mv1", 0, 1.0f },
								   { "walk2", "data/Sample/Player/Anim/Walk2.mv1", 1, 1.0f },
								   { "dance5", "data/Sample/Player/Anim/Dance5.mv1", 0, 1.0f } } );

			auto col = AddComponent<ComponentCollisionCapsule>();
			col->SetTranslate( { 0, 0, 0 } );	// トランスレートの設定 // Установление трансляции
			col->SetRadius( 2.5 );				// 半径の設定 // Установление радиуса
			col->SetHeight( 15 );				// 高さの設定 // Установление высоты
			col->SetCollisionGroup( ComponentCollision::CollisionGroup::SKELET );	// 衝突グループの設定 // Установление группы коллизий
			col->UseGravity();	 // 重力の使用 // Использование гравитации

			return true;   // 初期化の結果を返す // Возврат результата инициализации
		}

		ObjectPtr TrackingNearEnemy( ObjectPtr& obj )
		{
			float minDistance = FLT_MAX;   // 最小距離の初期化 // Инициализация минимального расстояния
			ObjectPtr nearestEnemy = nullptr;

			// シーン内の全ての敵オブジェクトを取得 // Получение всех объектов врагов в сцене
			auto enemies = Scene::GetObjectsPtr<Enemy>();

			for( const auto& enemy : enemies )
			{
				if( enemy != obj )	 // 自分自身を除外する // Исключаем самого себя
				{
					float distance = length( enemy->GetTranslate() - obj->GetTranslate() );	  // 距離を計算 // Вычисляем расстояние

					if( distance < minDistance )
					{
						minDistance = distance;	  // 最小距離の更新 // Обновляем минимальное расстояние
						nearestEnemy = enemy;	  // 最近の敵の更新 // Обновляем ближайшего врага
					}
				}
			}

			return nearestEnemy;   // 最近の敵を返す // Возврат ближайшего врага
		}

		//! @brief 処理更新 // Обновление обработки
		//! @param delta フレーム時間 // Время кадра
		void Skelet::Update( [[maybe_unused]] float delta )	  // override
		{
			auto near_enemy = TrackingNearEnemy( SharedThis() );   // 近い敵の追跡 // Отслеживание ближайшего врага
			auto mdl = GetComponent<ComponentModel>();

			// スケルトンがY=0のレベル以下に降りたことを確認 // Проверка, что скелет опустился ниже уровня Y = 0
			if( GetTranslate().y < 0.0f || IsKeyOn( KEY_INPUT_SPACE ) )
			{
				// スケルトンがマップの外に落ちた場合、ソルクの近くに召喚 // Призыв скелета к сорку, если он упал за пределы карты
				SummonNearSorc();
			}

			// マップ上にいる場合のスケルトンの通常の動作 // Обычное поведение скелета, если он на карте
			// アクティブとパッシブモードの切り替えロジック // Логика активного и пассивного режима
			if( IsKeyOn( KEY_INPUT_F ) )
			{
				aggressiveMode = !aggressiveMode;
			}

			// Остальной код для перемещения или атаки

			//if( IsKeyOn( KEY_INPUT_SPACE ) )   // スペースキーが押されたら // Если нажата клавиша пробела
			//{
			//	aggressiveMode = !aggressiveMode;	// 攻撃モードの切り替え // Переключение режима агрессии
			//}

			if( aggressiveMode )   // 攻撃モードの場合 // Если режим агрессии включён
			{
				if( auto target = GetComponent<ComponentTargetTracking>() )
				{
					target->SetTargetObjectPtr( near_enemy );	// ターゲットを設定する // Устанавливаем цель
					target->SetFrontVector( -mdl->GetMatrix().axisZ() );   // フロントベクトルの設定 // Устанавливаем фронтальный вектор
				}

				float3 target	= GetTranslate();
				target			= near_enemy->GetTranslate();
				auto   pos		= GetTranslate();
				float3 move		= ( target - pos );
				float  distance = length( move );

				if( distance <= 10 )   // 攻撃範囲内の場合 // Если враг в пределах досягаемости атаки
				{
					move = { 0, 0, 0 };
					timeSinceLastAttack += delta;	// 攻撃タイマーの更新 // Обновляем таймер атаки
					if( timeSinceLastAttack >= ATTACK_COOLDOWN )   // 攻撃クールダウンのチェック // Проверка кулдауна атаки
					{
						Attack();					  // 攻撃を行う // Выполняем атаку
						timeSinceLastAttack = 0.0f;	  // 攻撃タイマーをリセット // Сбрасываем таймер атаки
					}

					if( mdl && mdl->GetPlayAnimationName() != "attack" )
						mdl->PlayAnimation( "attack", true );	// 攻撃アニメーションの再生 // Воспроизводим анимацию атаки
				}
				else if( distance > 10 || distance <= 150 )	  // 追いかける範囲内の場合 // Если враг в пределах преследования
				{
					move = normalize( move );	// ベクトルの正規化 // Нормализуем вектор

					float x = -move.x;
					float z = -move.z;
					float theta = atan2( x, z ) * RadToDeg - rot_y_;   // 回転角度の計算 // Рассчитываем угол поворота

					SetRotationAxisXYZ( { 0, theta, 0 } );	 // 回転の設定 // Устанавливаем вращение

					move *= speed_ * ( delta * 20.0f );	  // 移動量の計算 // Рассчитываем перемещение
					if( length( move ).x > 0 )
					{
						if( mdl && mdl->GetPlayAnimationName() != "run" )
							mdl->PlayAnimation( "run", true );	 // ランアニメーションの再生 // Воспроизводим анимацию бега
					}
				}

				AddTranslate( move );	// 位置の更新 // Обновляем позицию
			}
			else   // 非攻撃モードの場合 // Если режим агрессии выключен
			{
				float3 follow = GetTranslate();

				if( auto sorc = Scene::GetObjectPtr<Sorc>( "Sorc" ) )
					follow = sorc->GetTranslate();
				auto pos_fol = GetTranslate();

				float3 move_fol		 = ( follow - pos_fol );
				float  distance_skel = length( move_fol );
				if( distance_skel <= 10 )	// 追従範囲内の場合 // Если в пределах расстояния следования
				{
					move_fol = { 0, 0, 0 };
					if( mdl && mdl->GetPlayAnimationName() != "idle" )
						mdl->PlayAnimation( "idle", true );	  // アイドルアニメーションの再生 // Воспроизводим анимацию ожидания
				}
				else if( distance_skel > 10 )
				{
					move_fol = normalize( move_fol );	// ベクトルの正規化 // Нормализуем вектор

					float x = -move_fol.x;
					float z = -move_fol.z;
					float theta = atan2( x, z ) * RadToDeg - rot_y_;   // 回転角度の計算 // Рассчитываем угол поворота

					SetRotationAxisXYZ( { 0, theta, 0 } );	 // 回転の設定 // Устанавливаем вращение

					move_fol *= speed_ * ( delta * 40.0f );	  // 移動量の計算 // Рассчитываем перемещение
					if( length( move_fol ).x > 0 )
					{
						if( mdl && mdl->GetPlayAnimationName() != "run" )
							mdl->PlayAnimation( "run", true );	 // ランアニメーションの再生 // Воспроизводим анимацию бега
					}
				}
				AddTranslate( move_fol );	// 位置の更新 // Обновляем позицию
			}
		}

		void Skelet::Attack()
		{
			static int attackCount = 0;	  // 攻撃回数を記録するカウンター // Счётчик атак для определения первой атаки
			float	   attackCooldown = ( attackCount == 0 ) ?
					 1.0f :
					 1.5f;	 // 最初の攻撃は1秒、以降は1.5秒 // Первая атака через 1 секунду, последующие через 1.5

			float		 currentTime = GetCurrentTime();   // 現在の時間を取得する // Получаем текущее время
			static float lastAttackTime = 0;   // 最後の攻撃時間を記録する // Время последней атаки

			if( currentTime - lastAttackTime >= attackCooldown )
			{
				if( auto enem = std::dynamic_pointer_cast<Enemy>( TrackingNearEnemy( SharedThis() ) ) )
				{
					enem->TakeDamage( 1 );	 // 敵にダメージを与える // Наносим урон врагу
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
		void Skelet::SummonNearSorc()
		{
			if( auto sorc = Scene::GetObjectPtr<Sorc>( "Sorc" ) )
			{
				SetTranslate( sorc->GetTranslate() + float3( 2.0f, 0.0f, 2.0f ) );	 // Sorcの近くに召喚 // Призыв рядом с Sorc
				aggressiveMode = false;	  // すぐにパッシブモードに切り替える // Сразу переводим в пассивный режим
			}
		}
		void Skelet::LateDraw()	  // override
		{
			if( aggressiveMode )   // 攻撃モードの場合 // Если режим агрессии включён
			{
				DrawFormatString( WINDOW_W / 2 - 100,
								  50,
								  GetColor( 255, 255, 255 ),
								  "Aggressive Mode" );	 // 攻撃モード表示 // Отображаем "Режим агрессии"
			}
			else
			{
				DrawFormatString( WINDOW_W / 2 - 100,
								  50,
								  GetColor( 255, 255, 255 ),
								  "Passive Mode" );	  // 非攻撃モード表示 // Отображаем "Пассивный режим"
			}
		}
		void Skelet::GUI()	 // override
		{
			__super::GUI();

			// GUI描画 // Отрисовка GUI
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
		//! @param hitInfo
		void Skelet::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )	// override
		{
			// 当たりで移動させる(これが無ければめり込みます) // Вызов родительской функции OnHit для обработки столкновения
			__super::OnHit( hitInfo );
		}

		//! @brief スピードを設定する // Устанавливает скорость
		//! @param s スピード // Скорость
		void Skelet::SetSpeed( float s )
		{
			speed_ = s;
		}

		//! @brief スピードを取得する // Получает скорость
		//! @return 現在のスピード // Текущая скорость
		float Skelet::GetSpeed()
		{
			return speed_;
		}

		//! @brief 変数として扱う関数 // Возвращает переменную скорости
		//! @detail Set/Get系と比べデバックはしにくい // Труднее отлаживать по сравнению с методами Set/Get
		//! @return 現在のスピードの変数(speed_) // Переменная текущей скорости (speed_)
		float& Skelet::Speed()
		{
			return speed_;
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE とセットで必要 (Editor上でCreateObjectするため) // Требуется в паре с BP_OBJECT_TYPE для создания объектов в редакторе
BP_OBJECT_IMPL( Sample::GameSample::Skelet, u8"Skelet" );

// オブジェクトセーブ構造を追加します // Добавление структуры для сохранения объектов
CEREAL_REGISTER_TYPE( Sample::GameSample::Skelet )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Skelet )	 // 多態関係を登録する // Регистрация полиморфных отношений
