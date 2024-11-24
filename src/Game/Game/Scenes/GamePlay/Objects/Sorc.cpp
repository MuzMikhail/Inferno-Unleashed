#include "Sorc.h"
#include "Enemy.h"
#include "Shot.h"
#include "Camera.h"
#include "Mask.h"
#include "../../Over/OverScene.h"
#include "../../Over/Goal.h"

#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentAttachModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentTargetTracking.h>
#include <System/Component/ComponentSpringArm.h>

namespace Sample
{
	namespace GameSample
	{
		//! @brief Editor上でのCreateObject用の設定 // Настройки для создания объектов в редакторе
		//! @detail BP_OBJECT_TYPEとセットで用意する // Подготавливается в паре с BP_OBJECT_TYPE для корректного отображения в редакторе
		BP_OBJECT_IMPL( Sorc, "GameSample/Sorc" );

		// Sorcオブジェクトを作成する関数 // Функция для создания объекта Sorc
		SorcPtr Sorc::Create( const float3& pos, const float3& front )
		{
			// Sorcオブジェクトをシーンに作成する // Создаём объект Sorc в сцене
			auto sorc = Scene::CreateObject<Sorc>();
			sorc->SetName( "Sorc" );   // オブジェクトの名前を設定 // Устанавливаем имя объекта

			// frontの方向に向ける // Поворачиваем объект в направлении вектора front
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			sorc->SetMatrix( mat );	  // 計算した行列をSorcに適用 // Применяем рассчитанную матрицу к объекту Sorc

			// posの位置にオブジェクトを配置 // Размещаем объект в позиции pos
			sorc->SetTranslate( pos );

			return sorc;   // Sorcオブジェクトを返す // Возвращаем объект Sorc
		}

		// Sorcオブジェクトの初期化 // Инициализация объекта Sorc
		bool Sorc::Init()	// override
		{
			__super::Init();   // 親クラスのInitを呼び出す // Вызываем инициализацию родительского класса

			// モデルコンポーネントを追加 (スケールを0.07に設定) // Добавляем компонент модели и устанавливаем масштаб 0.07
			auto model = AddComponent<ComponentModel>( "data/Sample/PlayerSorc/Sorc.mv1" );
			model->SetScaleAxisXYZ( { 0.07f } );   // モデルのスケールを設定 // Устанавливаем масштаб модели

			// モデルのアニメーションを設定 // Устанавливаем анимации модели
			model->SetAnimation( {
				{ "idle", "data/Sample/PlayerSorc/Anim/iatest4.mv1", 0, 1.0f },	  // 待機アニメーション // Анимация ожидания
				{ "attack", "data/Sample/PlayerSorc/Anim/sorc_attack1.mv1", 0, 1.0f },	 // 攻撃アニメーション // Анимация атаки
				{ "run", "data/Sample/PlayerSorc/Anim/sorc_run.mv1", 0, 1.0f },	  // 走るアニメーション // Анимация бега
				{ "death", "data/Sample/PlayerSorc/Anim/sorc_death.mv1", 0, 1.0f },	  // 死亡アニメーション // Анимация смерти
				{ "sp_run",
				  "data/Sample/PlayerSorc/Anim/sorc_speed_run.mv1",
				  0,
				  1.0f },	// 高速走行アニメーション // Анимация быстрого бега
				{ "magic_sh",
				  "data/Sample/PlayerSorc/Anim/sorc_magic_shield.mv1",
				  0,
				  1.0f }   // 魔法シールドアニメーション // Анимация магического щита
			} );

			// コリジョンコンポーネント (カプセル型) を追加 // Добавляем компонент коллизии (капсула)
			auto col = AddComponent<ComponentCollisionCapsule>();
			col->SetTranslate( { 0, 0, 0 } );	// カプセルのトランスレートを設定 // Устанавливаем смещение капсулы
			col->SetRadius( 2.5 );	 // カプセルの半径を設定 // Устанавливаем радиус капсулы
			col->SetHeight( 13 );	 // カプセルの高さを設定 // Устанавливаем высоту капсулы
			col->UseGravity();	 // 重力の影響を有効にする // Включаем влияние гравитации на капсулу

			// カメラコンポーネントを追加 // Добавляем компонент камеры
			auto camera = AddComponent<ComponentCamera>();
			camera->SetPositionAndTarget( { 0, 35, 60 },
										  { 0, 20, 0 } );	// カメラの位置とターゲットを設定 // Устанавливаем положение и цель камеры

			// モデルの特定のフレームを検索してエフェクトを設定 // Ищем определённые кадры в модели для настройки эффектов
			auto   attach		  = MV1SearchFrame( model->GetModel(), "Nightshade" );
			auto   sword_mat	  = MV1GetFrameLocalWorldMatrix( model->GetModel(), attach );
			auto   sword_mat_fire = MV1GetFrameLocalWorldMatrix( model->GetModel(), attach );
			float3 effect_pos =
				mul( float4( position_, 1.0f ), cast( sword_mat ) ).xyz;   // エフェクトの位置を計算 // Вычисляем позицию эффекта

			// ターゲットトラッキングコンポーネントを追加 // Добавляем компонент отслеживания цели
			auto target = AddComponent<ComponentTargetTracking>();
			target->SetTrackingNode( "mixamorig:Neck" );   // トラッキングするノードを設定 // Устанавливаем узел для отслеживания
			target->SetFrontVector( { 0, 0, -1 } );	  // フロントベクトルを設定 // Устанавливаем фронтальный вектор

			// トラッキングの左右と上下の制限を設定 // Настраиваем ограничения отслеживания по горизонтали и вертикали
			target->SetTrackingLimitLeftRight( { 70, 70 } );   // 左右の制限 // Ограничения по горизонтали
			target->SetTrackingLimitUpDown( { 10, 10 } );	   // 上下の制限 // Ограничения по вертикали

			// エフェクトのロードと初期化 // Загружаем и инициализируем эффекты
			effect_sw_		= LoadEffekseerEffect( "data/Sample/Effects/MagicArea.efkefc" );
			effect_play_sw_ = PlayEffekseer3DEffect( effect_sw_ );
			effect_f_		= LoadEffekseerEffect( "data/Sample/Effects/MagicFire4.efkefc" );
			effect_play_f_	= PlayEffekseer3DEffect( effect_f_ );
			float scale		= 1.5f;	  // エフェクトのスケールを設定 // Устанавливаем масштаб эффекта

			// エフェクトの位置とスケールを設定 // Устанавливаем позицию и масштаб эффектов
			SetPosPlayingEffekseer3DEffect( effect_play_sw_, position_.x, position_.y, position_.z );

			if( IsEffekseer3DEffectPlaying( effect_play_f_ ) )
			{
				effect_play_f_ = PlayEffekseer3DEffect( effect_f_ );
			}

			SetPosPlayingEffekseer3DEffect( effect_play_f_, position_.x, position_.y, position_.z );
			SetScalePlayingEffekseer3DEffect( effect_play_f_, scale, scale, scale );   // スケールを設定 // Устанавливаем масштаб

			maskAct = false;   // マスクアクションを無効にする // Отключаем действие маски

			return true;   // 初期化が成功した場合にtrueを返す // Возвращаем true, если инициализация прошла успешно
		}

		// Sorcオブジェクトの更新処理 // Обновление объекта Sorc
		void Sorc::Update( [[maybe_unused]] float delta )	// override
		{
			auto mdl = GetComponent<ComponentModel>();	 // モデルコンポーネントを取得 // Получаем компонент модели
			auto near_enemy = TrackingNearEnemy( SharedThis() );   // 近い敵を追跡 // Отслеживание ближайшего врага

			// ゲーム内の時間を計算する // Рассчитываем игровое время
			seconds++;
			if( seconds >= 60 )
			{
				seconds = 0;
				minutes++;
				if( minutes >= 60 )
				{
					minutes = 0;
					hours++;
				}
			}

			float3 move = float3( 0, 0, 0 );   // 移動量を初期化 // Инициализируем перемещение

			// キーボード入力でキャラクターを回転させる // Поворачиваем персонажа в зависимости от ввода с клавиатуры
			if( IsKeyRepeat( KEY_INPUT_A ) )
			{
				rot_y_ += -1.0f;   // 左に回転 // Вращение влево
			}
			if( IsKeyRepeat( KEY_INPUT_D ) )
			{
				rot_y_ += 1.0f;	  // 右に回転 // Вращение вправо
			}
			SetRotationAxisXYZ( { 0, rot_y_, 0 } );	  // 回転を設定 // Устанавливаем вращение

			auto mat = GetMatrix();	  // 現在の行列を取得 // Получаем текущую матрицу

			// カメラの方向に基づいてキャラクターを動かす // Двигаем персонажа в направлении камеры
			auto   cam = Scene::GetObjectPtr<Camera>( "PlayerCamera" );
			float3 v   = GetTranslate() - cam->GetTranslate();
			mat		   = HelperLib::Math::CreateMatrixByFrontVector(
				   -v );   // カメラに向かって行列を設定 // Устанавливаем матрицу в направлении камеры

			// キーボード入力で移動方向を決定する // Определяем направление движения по вводу с клавиатуры
			if( IsKeyRepeat( KEY_INPUT_W ) )
			{
				float3 vec = mat.axisZ();
				move	   = -normalize( float3( vec.x, 0, vec.z ) );	// 前進 // Движение вперёд
			}
			if( IsKeyRepeat( KEY_INPUT_D ) )
			{
				float3 vec = mat.axisX();
				move -= normalize( float3( vec.x, 0, vec.z ) );	  // 右に移動 // Движение вправо
			}
			if( IsKeyRepeat( KEY_INPUT_S ) )
			{
				float3 vec = mat.axisZ();
				move	   = normalize( float3( vec.x, 0, vec.z ) );   // 後退 // Движение назад
			}
			if( IsKeyRepeat( KEY_INPUT_A ) )
			{
				float3 vec = mat.axisX();
				move += normalize( float3( vec.x, 0, vec.z ) );	  // 左に移動 // Движение влево
			}

#if 1	// Animation \
	// キャラクターが移動しているときのアニメーション処理 // Анимация при движении персонажа
			if( mdl )
				if( length( move ).x > 0 && mdl->GetPlayAnimationName() != "attack" )
				{
					move = normalize( move );	// 移動ベクトルを正規化 // Нормализуем вектор движения

					float x = -move.x;
					float z = -move.z;
					float theta = atan2( x, z ) * RadToDeg - rot_y_;   // 回転角度を計算 // Рассчитываем угол поворота
					mdl->SetRotationAxisXYZ( { 0, theta, 0 } );	  // モデルの回転を設定 // Устанавливаем вращение модели

					if( mdl->GetPlayAnimationName() != "run" && mdl->GetPlayAnimationName() != "attack" )
						mdl->PlayAnimation( "run", true );	 // 通常走行アニメーション再生 // Запуск обычной анимации бега
					move *= speed_ * ( delta * 45.f );	 // 通常速度で移動 // Движение на обычной скорости


					// Shiftキーが押されたらダッシュする // Бег при нажатии клавиши Shift
					// // 加速とスタミナの試用版は現在開発中です。// Пробная версия ускорения и стамины находится в доработке.
					//
					//if( IsKeyRepeat( KEY_INPUT_LSHIFT ) )
					//{
					//	stamina_point_ -= 3;   // スタミナを消費 // Расходуем выносливость
					//	if( stamina_point_ > 0 )
					//	{
					//		move *= speed_ * ( delta * 60.f );	 // 高速移動 // Быстрое движение
					//		if( mdl->GetPlayAnimationName() != "sp_run" && mdl->GetPlayAnimationName() != "attack" )
					//			mdl->PlayAnimation( "sp_run", true );	// ダッシュアニメーション再生 // Запуск анимации бега
					//	}
					//	else
					//	{
					//if( mdl->GetPlayAnimationName() != "run" && mdl->GetPlayAnimationName() != "attack" )
					//	mdl->PlayAnimation( "run", true );	 // 通常走行アニメーション再生 // Запуск обычной анимации бега
					//move *= speed_ * ( delta * 30.f );	 // 通常速度で移動 // Движение на обычной скорости
					//	}
					//	if( stamina_point_ <= 0 )
					//	{
					//		stamina_point_ = 0;	  // スタミナがゼロになったらリセット // Если выносливость на нуле, сбрасываем её
					//	}
					//}
					//else if( stamina_point_ >= 100 )
					//{
					//	stamina_point_ = 100;	// スタミナが最大の場合 // Если выносливость максимальная
					//	if( mdl->GetPlayAnimationName() != "run" && mdl->GetPlayAnimationName() != "attack" )
					//		mdl->PlayAnimation( "run", true );	 // 通常走行アニメーション再生 // Запуск обычной анимации бега
					//}
					//else if( stamina_point_ <= 0 )
					//{
					//	stamina_point_ = 0;	  // スタミナがゼロの場合 // Если выносливость на нуле
					//}
					//else
					////{
					//	if( mdl->GetPlayAnimationName() != "run" && mdl->GetPlayAnimationName() != "attack" )
					//		mdl->PlayAnimation( "run", true );	 // 通常走行アニメーション再生 // Запуск обычной анимации бега
					//	move *= speed_ * ( delta * 60.f );	 // 通常速度で移動 // Движение на обычной скорости
					//	stamina_point_ += 3;   // スタミナを回復 // Восстанавливаем выносливость
					//	if( stamina_point_ <= 0 )
					//	{
					//		stamina_point_ = 0;	  // スタミナがゼロの場合 // Если выносливость на нуле
					//		move *= speed_ * ( delta * 60.f );	 // 通常速度で移動 // Движение на обычной скорости
					//	}
					//	else if( stamina_point_ >= 100 )
					//	{
					//		stamina_point_ = 100;	// スタミナが最大の場合 // Если выносливость максимальная
					//		if( mdl->GetPlayAnimationName() != "run" && mdl->GetPlayAnimationName() != "attack" )
					//			mdl->PlayAnimation( "run", true );	 // 通常走行アニメーション再生 // Запуск обычной анимации бега
					//		move *= speed_ * ( delta * 60.f );	 // 通常速度で移動 // Движение на обычной скорости
					//	}
					//}
				}
				else
				{
					// スペースキーが押されたら魔法シールドを使う // Используем магический щит при нажатии пробела
					if( IsKeyOn( KEY_INPUT_SPACE ) )
					{
						if( stamina_point_ > 0 )
						{
							if( mdl->GetPlayAnimationName() != "magic_sh" && mdl->GetPlayAnimationName() != "sp_run" &&
								mdl->GetPlayAnimationName() != "attack" && mdl->GetPlayAnimationName() != "run" )
							{
								mdl->PlayAnimation( "magic_sh" );	// 魔法シールドアニメーション再生 // Запуск анимации магического щита
							}
						}
						else
						{
							if( mdl->GetPlayAnimationName() != "idle" && mdl->GetPlayAnimationName() != "attack" &&
								mdl->GetPlayAnimationName() != "magic_sh" )
								mdl->PlayAnimation( "idle", true );	  // 待機アニメーション再生 // Запуск анимации ожидания
						}
					}
					else
					{
						// スペースキーが押されていない場合 // Если пробел не нажат
						if( mdl->GetPlayAnimationName() != "idle" && mdl->GetPlayAnimationName() != "attack" &&
							mdl->GetPlayAnimationName() != "magic_sh" )
							mdl->PlayAnimation( "idle", true );	  // 待機アニメーション再生 // Запуск анимации ожидания
					}
				}
#endif

			if( mdl )
			{
				// エフェクトの位置を更新する // Обновляем позиции эффектов
				auto   attach_frame_shield = MV1SearchFrame( mdl->GetModel(), "Nightshade" );
				auto   attach_frame_fire   = MV1SearchFrame( mdl->GetModel(), "mixamorig:RightHandMiddle1" );
				auto   sword_mat		   = MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_shield );
				auto   sword_mat_fire	   = MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_fire );
				float3 effect_pos		   = mul( float4( position_, 1.0f ), cast( sword_mat ) ).xyz;
				float3 effect_pos_f		   = mul( float4( position_, 1.0f ), cast( sword_mat_fire ) ).xyz;

				float scale = 7.0f;	  // エフェクトのスケール // Масштаб эффектов

				// スペースキーが押されたら魔法エフェクトを再生する // При нажатии пробела запускаем магический эффект
				if( IsKeyRepeat( KEY_INPUT_SPACE ) )
				{
					if( IsEffekseer3DEffectPlaying( effect_play_sw_ ) )
					{
						effect_play_sw_ = PlayEffekseer3DEffect( effect_sw_ );	 // エフェクト再生 // Запуск эффекта
					}

					SetPosPlayingEffekseer3DEffect( effect_play_sw_, effect_pos.x, effect_pos.y, effect_pos.z );
					SetScalePlayingEffekseer3DEffect( effect_play_sw_,
													  scale,
													  scale,
													  scale );	 // エフェクトのスケール設定 // Устанавливаем масштаб эффекта
				}

				if( IsEffekseer3DEffectPlaying( effect_play_f_ ) )
				{
					effect_play_f_ = PlayEffekseer3DEffect( effect_f_ );   // エフェクト再生 // Запуск эффекта
				}

				SetPosPlayingEffekseer3DEffect( effect_play_f_, effect_pos_f.x, effect_pos_f.y, effect_pos_f.z );
			}


			// 攻撃アニメーション以外の場合に移動処理を行う // Обновляем позицию, если не проигрывается анимация атаки
			if( mdl->GetPlayAnimationName() != "attack" )
			{
				AddTranslate( move );	// 位置を更新 // Обновляем позицию
			}

			// マウス左クリックでショットを発射する // Выстрел при нажатии левой кнопки мыши
			if( IsMouseOn( MOUSE_INPUT_LEFT ) )
			{
				float3 vec = { 0, 0, 1 };	// 初期ベクトル // Начальный вектор

				if( mdl )
					vec = mul( -float4( mdl->GetMatrix().axisZ(), 0 ), GetMatrix() )
							  .xyz;	  // 発射方向を計算 // Рассчитываем направление выстрела

				// ショットの作成と発射 // Создание и запуск выстрела
				if( mdl->GetPlayAnimationName() != "attack" && mdl->GetPlayAnimationName() != "sp_run" &&
					mdl->GetPlayAnimationName() != "run" )
				{
					auto   attach_frame_shot = MV1SearchFrame( mdl->GetModel(), "mixamorig:RightHandMiddle1" );
					auto   bone_mat			 = MV1GetFrameLocalWorldMatrix( mdl->GetModel(), attach_frame_shot );
					float3 shot_pos			 = mul( float4( position_, 1.0f ), cast( bone_mat ) ).xyz;

					mdl->PlayAnimation( "attack" );	  // 攻撃アニメーション再生 // Запуск анимации атаки
					auto shot = Scene::CreateObject<Shot>();
					shot->SetDelay( 0.5f );	  // ショットの遅延設定 // Устанавливаем задержку выстрела
					shot->SetPositionAndDirection( shot_pos,
												   vec );	// ショットの位置と方向を設定 // Устанавливаем позицию и направление выстрела
				}
			}

			float3 vec_m = { 0, 0, 1 };	  // 初期ベクトル // Начальный вектор

			if( mdl )
				vec_m = mul( -float4( mdl->GetMatrix().axisZ(), 0 ), GetMatrix() )
							.xyz;	// モデルの方向ベクトルを計算 // Рассчитываем вектор направления модели
		}

		// 基本描画の後に処理します
		// 基本描画の後に処理します // Выполняем после основного рендера
		void Sorc::LateDraw()	// override
		{
			gauge_.Draw();	 // ゲージを描画 // Отображаем индикатор здоровья

			static bool game_started = false;	// ゲームが始まったかどうかを示す変数 // Переменная, указывающая, началась ли игра
			static int countdown = 3;	// ゲーム開始前のカウントダウン // Обратный отсчет перед началом игры
			static int timer = 0;	// タイマー // Таймер

			//static bool agressive_mode = false;	  // 攻撃モードかどうか // Режим агрессии
			int fontSize = 38;		   // フォントサイズ // Размер шрифта
			SetFontSize( fontSize );   // フォントサイズを設定 // Устанавливаем размер шрифта

			// ゲーム開始前のカウントダウン処理 // Обратный отсчет перед началом игры
			if( !game_started )
			{
				if( countdown > 0 )
				{
					//DrawFormatString( WINDOW_W / 2 - 20, 70, GetColor( 255, 255, 255 ), "%d", countdown );
					countdown--;   // カウントダウンを減らす // Уменьшаем обратный отсчет
				}
				else
				{
					game_started = true;   // ゲーム開始フラグを立てる // Игра началась
					timer		 = 0;	   // タイマーをリセット // Сбрасываем таймер
				}
			}
			else
			{
				// ゲームが始まったらタイマーを進める // Запускаем таймер, когда игра началась
				timer++;
			}

			// タイマーの時間を計算して表示 // Рассчитываем и отображаем время таймера
			int minutes		 = timer / 3600;
			int seconds		 = ( timer % 3600 ) / 60;
			int milliseconds = timer % 60;
			DrawFormatString( WINDOW_W / 2 - 100, 20, GetColor( 255, 255, 255 ), "Timer: %02d:%02d:%02d", minutes, seconds, milliseconds );

			if( maskAct )	// マスクがアクティブな場合の描画処理 // Отображаем, если маска активна
			{
				DrawExtendGraph( 50, 50, 150, 200, maskImg, TRUE );	  // マスクのグラフィックを描画 // Рисуем изображение маски
			}

			// スペースキーで攻撃モードを切り替え // Переключаем режим агрессии при нажатии пробела
			//if( IsKeyOn( KEY_INPUT_А ) )
			//{
			//	agressive_mode = !agressive_mode;	// 攻撃モードを切り替える // Переключаем режим агрессии
			//}
			//if( agressive_mode )
			//{
			//	DrawFormatString( WINDOW_W / 2 - 100,
			//					  50,
			//					  GetColor( 255, 255, 255 ),
			//					  "Aggressive Mode" );	 // 攻撃モード表示 // Отображаем "Режим агрессии"
			//}
			//else
			//{
			//	DrawFormatString( WINDOW_W / 2 - 100,
			//					  50,
			//					  GetColor( 255, 255, 255 ),
			//					  "Passive Mode" );	  // 非攻撃モード表示 // Отображаем "Пассивный режим"
			//}
		}

		// GUIの描画処理 // Отображение GUI
		void Sorc::GUI()   // override
		{
			__super::GUI();	  // 基本的なGUI処理を呼び出す // Вызываем базовое отображение GUI

			// ImGuiによるカスタムGUI // Настраиваем интерфейс с помощью ImGui
			ImGui::Begin( GetName().data() );
			{
				ImGui::Separator();	  // セパレーターを描画 // Отображаем разделитель
				ImGui::DragFloat( u8"速度",
								  &speed_,
								  0.01f,
								  0.01f,
								  10.0f,
								  "%2.2f" );   // 速度調整用のスライダー // Слайдер для настройки скорости
				ImGui::DragFloat( u8"HitPoint",
								  &gauge_.Value(),
								  1,
								  0,
								  100,
								  "%3.0f" );   // ヒットポイント調整用のスライダー // Слайдер для настройки здоровья
			}
			ImGui::End();
		}

		// 衝突時の処理 // Обработка при столкновении
		void Sorc::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )	  // override
		{
			auto owner = hitInfo.hit_collision_
							 ->GetOwnerPtr();	// 衝突相手のオーナーを取得 // Получаем владельца объекта, с которым произошло столкновение

			// ポータルに当たった場合 // Если столкнулись с порталом
			if( owner->GetNameDefault() == "Portal" )
			{
				if( maskAct == true )
				{
					Scene::Change( Scene::GetScene<Goal>() );	// ゴールシーンに移動 // Переход на сцену цели
					StopEffekseer3DEffect( effect_play_sw_ );	// エフェクトを停止 // Останавливаем эффекты
					StopEffekseer3DEffect( effect_play_f_ );
				}
			}

			__super::OnHit( hitInfo );	 // 親クラスのOnHitを呼び出す // Вызываем родительскую функцию OnHit
		}

		// ダメージを受けた際の処理 // Обработка при получении урона
		void Sorc::TakeDamage( int damage )
		{
			if( gauge_.Value() > 0 )
			{
				gauge_ -= damage;	// ダメージ分だけヒットポイントを減らす // Уменьшаем здоровье на величину урона
			}

			// ヒットポイントが0以下の場合 // Если здоровье стало меньше или равно нулю
			if( gauge_ <= 0 )
			{
				gauge_ = 0;	  // ヒットポイントを0に設定 // Устанавливаем здоровье на 0
				StopEffekseer3DEffect( effect_play_sw_ );	// エフェクトを停止 // Останавливаем эффекты
				StopEffekseer3DEffect( effect_play_f_ );

				Scene::Change( Scene::GetScene<OverScene>() );	 // ゲームオーバーシーンに移動 // Переход на сцену поражения
			}
		}

		// 速度を設定する // Устанавливаем скорость
		void Sorc::SetSpeed( float s )
		{
			speed_ = s;
		}

		// 速度を取得する // Получаем скорость
		float Sorc::GetSpeed()
		{
			return speed_;
		}

		// 速度変数への参照を返す // Возвращаем ссылку на переменную скорости
		float& Sorc::Speed()
		{
			return speed_;
		}

	}	// namespace GameSample
}	// namespace Sample

// Cereal用の型登録 // Регистрация типа для Cereal
CEREAL_REGISTER_TYPE( Sample::GameSample::Sorc )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Sorc )
