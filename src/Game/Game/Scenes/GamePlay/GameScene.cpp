#include "GameScene.h"
#include "Objects/Sorc.h"
#include "Objects/Enemy.h"
#include "Objects/Skelet.h"
#include "Objects/Camera.h"
#include "Objects/Portal.h"
#include "Objects/Key.h"
#include "Objects/Mask.h"
#include "Objects/Deathbody.h"

#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentAttachModel.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentTargetTracking.h>
#include <System/Component/ComponentSpringArm.h>
#include <System/Graphics/Animation.h>

#include <cmath>

namespace Sample
{
	namespace GameSample
	{
		// クラスの初期化 (BP_CLASS_IMPL)
		// Инициализация класса (BP_CLASS_IMPL)
		BP_CLASS_IMPL( GameScene, u8"GameSample/GameScene" );

// STAGE01を使用するときはこちらを有効にする
// Включить при использовании STAGE01
#define USE_MOUSE_CAMERA

		// ゲームシーンの初期化処理
		// Инициализация игровой сцены
		bool GameScene::Init()
		{
			{
				// 地面オブジェクトの作成
				// Создание объекта земли
				auto obj = Scene::CreateObject<Object>()->SetName( "Ground" );
				obj->AddComponent<ComponentModel>( "data/Sample/SwordBout/Stage/Stage01.mv1" );
				if( auto cmpmdl = obj->AddComponent<ComponentCollisionModel>() )
					cmpmdl->AttachToModel( true );	 // モデルにコリジョンを付ける // Присоединение модели к коллизии
			}

			//----------------------------------------------------------------------------------
			// プレイヤーキャラクターとオブジェクトの生成
			// Создание персонажа игрока и объектов
			//----------------------------------------------------------------------------------
			auto s	   = Sorc::Create( { 0, 100, 0 } );
			auto skel  = Skelet::Create( { 0, 100, 0 } );
			auto goal  = Portal::Create( { 0, 100, 0 } );
			auto item  = Key::Create( { 0, 100, 0 } );
			auto dbody = Deathbody::Create( { 0, 0, 0 } );

			// ステージオブジェクトの位置設定
			// Установка позиций объектов на сцене
			int randomIndex = 1;
			switch( randomIndex )
			{
			case 1:
				s->SetTranslate( { 0, 100, 0 } );
				skel->SetTranslate( { 5, 100, 0 } );
				goal->SetTranslate( { 513, 22, -65 } );
				item->SetTranslate( { 180, 80, -262 } );
				break;
			case 2:
				s->SetTranslate( { 462, 10, 260 } );
				skel->SetTranslate( { 465, 10, 260 } );
				goal->SetTranslate( { 475, 10, 260 } );
				item->SetTranslate( { 485, 10, 260 } );
				break;
			case 3:
				s->SetTranslate( { 513, 15, -175 } );
				skel->SetTranslate( { 515, 15, -175 } );
				goal->SetTranslate( { 525, 15, -175 } );
				item->SetTranslate( { 535, 15, -175 } );
				break;
			default:
				// エラーハンドリングやデフォルト動作
				// Обработка ошибок или действие по умолчанию
				break;
			}

			//----------------------------------------------------------------------------------
			// 敵キャラクターの生成
			// Создание врагов
			//----------------------------------------------------------------------------------
			auto enemy = Enemy::Create( { GetRand( 1000 ) - 500, 100, -GetRand( 500 ) } );
			enemy->SetSpeed( 0.1f );   // 敵のスピード設定 // Установка скорости врага

			// 敵にカメラを設定する
			// Создание камеры для врага
			Camera::Create( enemy )->SetName( "EnemyCamera" );

			// 複数の敵キャラクターをランダムな位置に生成する
			// Создание множества врагов в случайных позициях
			for( int i = 0; i < 20; i++ )
			{
				auto enemy = Scene::CreateObject<Enemy>();
				enemy->SetName( "Enemy" );
				enemy->SetTranslate( { GetRand( 383 - 33 ) + 33, 0, GetRand( 721 - 564 ) + 564 } );
				enemy->SetSpeed( 0.1f );
			}
			for( int i = 0; i < 10; i++ )
			{
				auto enemy = Scene::CreateObject<Enemy>();
				enemy->SetName( "Enemy" );
				enemy->SetTranslate( { GetRand( 371 - 207 ) + 207, 85, GetRand( -113 - ( -230 ) ) + ( -230 ) } );
				enemy->SetSpeed( 0.1f );
			}
			for( int i = 0; i < 10; i++ )
			{
				auto enemy = Scene::CreateObject<Enemy>();
				enemy->SetName( "Enemy" );
				enemy->SetTranslate( { GetRand( 623 - 410 ) + 410, 80, GetRand( 202 - 72 ) + 72 } );
				enemy->SetSpeed( 0.1f );
			}
			for( int i = 0; i < 10; i++ )
			{
				auto enemy = Scene::CreateObject<Enemy>();
				enemy->SetName( "Enemy" );
				enemy->SetTranslate( { GetRand( 303 - 186 ) + 186, 40, GetRand( 369 - 72 ) + 72 } );
				enemy->SetSpeed( 0.1f );
			}

			// プレイヤーキャメラの生成
			// Создание камеры для игрока
			Camera::Create( s )->SetName( "PlayerCamera" );

			return true;
		}

		// 更新処理
		// Обновление сцены
		void GameScene::Update( [[maybe_unused]] float delta )
		{
			if( IsKeyOn( KEY_INPUT_1 ) )
				Scene::SetCurrentCamera( "PlayerCamera" );
		}

		// 描画処理
		// Отрисовка сцены
		void GameScene::Draw()
		{
			// "Title"という文字を描画する (テスト用)
			// Отображение текста "Title" (для теста)
			// DrawFormatString( 100, 50, GetColor( 255, 255, 255 ), "Title" );
		}

		// シーン終了処理
		// Завершение сцены
		void GameScene::Exit()
		{
			// 現在、終了時に特別な処理は不要
			// Пока нет действий при завершении сцены
		}

		// GUI描画処理
		// Обработка GUI
		void GameScene::GUI()
		{
		}

	}	// namespace GameSample
}	// namespace Sample
