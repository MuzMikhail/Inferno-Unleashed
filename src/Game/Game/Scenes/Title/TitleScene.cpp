#include "TitleScene.h"
#include "../GamePlay/Objects/Enemy.h"
#include "../GamePlay/GameScene.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>

//#define USE_MOUSE_CAMERA

namespace Sample
{
	namespace GameSample
	{
		BP_CLASS_IMPL( TitleScene, u8"GameSample/TitleScene" );

		// 初期化 // Инициализация
		bool TitleScene::Init()
		{
			// グラフィックデータの読み込み // Загрузка графических данных
			imageHandle			= LoadGraph( "data/Sample/UI/main_menu1.jpg" );
			imgLoad				= LoadGraph( "data/Sample/UI/main_menu1.jpg" );
			imgCursor			= LoadGraph( "data/Sample/UI/cur.png" );
			startButtonImage	= LoadGraph( "data/Sample/UI/butStart.png" );
			optionButtonImage	= LoadGraph( "data/Sample/UI/butOpt.png" );
			creditsButtonImage	= LoadGraph( "data/Sample/UI/butCred.png" );
			exitButtonImage		= LoadGraph( "data/Sample/UI/butExit.png" );
			startButtonPImage	= LoadGraph( "data/Sample/UI/butStartP.png" );
			optionButtonPImage	= LoadGraph( "data/Sample/UI/butOptP.png" );
			creditsButtonPImage = LoadGraph( "data/Sample/UI/butCredP.png" );
			exitButtonPImage	= LoadGraph( "data/Sample/UI/butExitP.png" );
			loadbar				= LoadGraph( "data/Sample/UI/loadbbar.png" );
			loadbarF			= LoadGraph( "data/Sample/UI/loadingFull.png" );
			tutorialImage		= LoadGraph( "data/Sample/UI/tutor.png" );
			tutorialText		= LoadGraph( "data/Sample/UI/tutText.png" );
			imgGameTit			= LoadGraph( "data/Sample/UI/gameTit.png" );

			alphaValue		= 0;	  // アルファ値 // Альфа-значение
			increasingAlpha = true;	  // 増加フラグ // Флаг увеличения

			return true;
		}

		// 更新処理 // Обновление состояния
		void TitleScene::Update( [[maybe_unused]] float delta )
		{
			bool isMouseButtonDown = GetMouseInput() & MOUSE_INPUT_LEFT;
			mouseX				   = GetMouseX();
			mouseY				   = GetMouseY();

			// アルファ値の増減 // Изменение прозрачности
			if( increasingAlpha )
			{
				alphaValue += 2;
				if( alphaValue >= 255 )
				{
					alphaValue		= 255;
					increasingAlpha = false;
				}
			}
			else
			{
				alphaValue -= 2;
				if( alphaValue <= 0 )
				{
					alphaValue		= 0;
					increasingAlpha = true;
				}
			}

			// チュートリアルの表示状態に応じた操作 // Действия в зависимости от отображения туториала
			if( !showTutorial )
			{
				// "Start" ボタンが押されたらチュートリアルを表示する // Отображение туториала при нажатии "Start"
				if( mouseX >= startButtonX && mouseX <= startButtonX + 400 && mouseY >= startButtonY && mouseY <= startButtonY + 90 )
				{
					if( IsMouseOn( MOUSE_INPUT_LEFT ) )
					{
						showTutorial = true;
					}
				}
				// "Exit" ボタンが押されたらアプリケーションを終了する // Завершение приложения при нажатии "Exit"
				else if( mouseX >= exitButtonX && mouseX <= exitButtonX + 400 && mouseY >= exitButtonY && mouseY <= exitButtonY + 90 )
				{
					if( IsMouseOn( MOUSE_INPUT_LEFT ) )
					{
						exit( 0 );
					}
				}
			}
			else
			{
				// チュートリアル中にスペースキーが押されたらゲームシーンに遷移する // Переход к игровой сцене по пробелу
				if( IsKeyOn( KEY_INPUT_SPACE ) )
				{
					Scene::Change( Scene::GetScene<GameScene>() );
					loading = true;
				}
			}

			SetMouseDispFlag( FALSE );

			int windowWidth	 = WINDOW_W;
			int windowHeight = WINDOW_H;

			// マウス位置の境界チェック // Ограничение позиции курсора
			if( mouseX < 0 )
				mouseX = 0;
			if( mouseX >= windowWidth )
				mouseX = windowWidth - 1;
			if( mouseY < 0 )
				mouseY = 0;
			if( mouseY >= windowHeight )
				mouseY = windowHeight - 1;

			SetMousePoint( mouseX, mouseY );
		}

		// 描画処理 // Отображение
		void TitleScene::Draw()
		{
			DrawGraph( 0, 0, imageHandle, TRUE );

			if( loading == false )
			{
				if( !showTutorial )
				{
					// ボタンの描画 // Отображение кнопок
					DrawExtendGraphF( startButtonX, startButtonY, startButtonX + 400, startButtonY + 90, startButtonImage, TRUE );
					//DrawExtendGraphF( optionButtonX, optionButtonY, optionButtonX + 400, optionButtonY + 90, optionButtonImage, TRUE );
					DrawExtendGraphF( creditsButtonX, creditsButtonY, creditsButtonX + 400, creditsButtonY + 90, creditsButtonImage, TRUE );
					DrawExtendGraphF( exitButtonX, exitButtonY, exitButtonX + 400, exitButtonY + 90, exitButtonImage, TRUE );
					DrawGraph( 720, 100, imgGameTit, TRUE );
					// カーソル位置に応じたボタンの色変更 // Изменение цвета кнопок при наведении курсора
					if( mouseX >= startButtonX && mouseX <= startButtonX + 400 && mouseY >= startButtonY && mouseY <= startButtonY + 90 )
					{
						DrawExtendGraphF( startButtonX, startButtonY, startButtonX + 400, startButtonY + 90, startButtonPImage, TRUE );
					}
					//else if( mouseX >= optionButtonX && mouseX <= optionButtonX + 400 && mouseY >= optionButtonY &&
					//		 mouseY <= optionButtonY + 90 )
					//{
					//	DrawExtendGraphF( optionButtonX, optionButtonY, optionButtonX + 400, optionButtonY + 90, optionButtonPImage, TRUE );
					//}
					else if( mouseX >= creditsButtonX && mouseX <= creditsButtonX + 400 && mouseY >= creditsButtonY &&
							 mouseY <= creditsButtonY + 90 )
					{
						DrawExtendGraphF( creditsButtonX,
										  creditsButtonY,
										  creditsButtonX + 400,
										  creditsButtonY + 90,
										  creditsButtonPImage,
										  TRUE );
					}
					else if( mouseX >= exitButtonX && mouseX <= exitButtonX + 400 && mouseY >= exitButtonY && mouseY <= exitButtonY + 90 )
					{
						DrawExtendGraphF( exitButtonX, exitButtonY, exitButtonX + 400, exitButtonY + 90, exitButtonPImage, TRUE );
					}

					// カーソル画像の描画 // Отображение курсора
					DrawExtendGraphF( GetMouseX(), GetMouseY(), GetMouseX() + 50, GetMouseY() + 50, imgCursor, TRUE );
				}
				else
				{
					// チュートリアルの表示 // Отображение туториала
					DrawExtendGraphF( 0, 0, WINDOW_W, WINDOW_H, tutorialImage, TRUE );
					DrawExtendGraphF( WINDOW_W / 2 - 200, 100, WINDOW_W / 2 + 200, 200, tutorialText, TRUE );

					SetFontSize( fontSize + 1 );

					SetFontSize( fontSize );   // フォントサイズを設定 // Устанавливаем размер шрифта

					int textX		= 200;	 // テキストのX座標 // Начальная координата X для текста
					int textY		= 200;	 // テキストのY座標 // Начальная координата Y для текста
					int lineSpacing = 40;	 // 行間隔 // Отступ между строками
					int keyColor  = GetColor( 255, 0, 0 );	 // キーの色 // Цвет для выделения ключевых слов
					int textColor = GetColor( 255, 255, 255 );	 // テキストの色 // Основной цвет текста

					textY += lineSpacing;

					// キーワードを色分けして描画 // Отображение выделенных ключевых слов
					DrawString( textX, textY, "左クリック : ", keyColor );	 // 左クリックの説明 // Описание левой кнопки мыши
					DrawString( textX + 300,
								textY,
								"ファイアボールを発射 (3秒クールダウン)。",
								textColor );   // ファイアボールを発射します // Выпускает файербол (3 секунды перезарядка)
					textY += lineSpacing;

					DrawString( textX, textY, "WASDキー: ", keyColor );	  // WASDキーの説明 // Описание клавиш WASD
					DrawString( textX + 300,
								textY,
								"キャラクターを移動します。",
								textColor );   // キャラクターを移動するためのキー // Клавиши для перемещения персонажа
					textY += lineSpacing;

					DrawString( textX, textY, "マウス: ", keyColor );	// マウスの説明 // Описание управления мышью
					DrawString( textX + 300,
								textY,
								"カメラ視点を操作します。",
								textColor );   // カメラ視点を動かす // Управляет видом камеры
					textY += lineSpacing;

					DrawString( textX, textY, "スペースバー: ", keyColor );	  // スペースバーの説明 // Описание клавиши пробела
					DrawString( textX + 300,
								textY,
								"スケルトンをキャラクターの近くに召喚。",
								textColor );   // スケルトンを近くに呼び出す // Призывает скелета к персонажу
					textY += lineSpacing;

					DrawString( textX, textY, "Fキー: ", keyColor );   // Fキーの説明 // Описание клавиши F
					DrawString( textX + 300,
								textY,
								"スケルトンのアグレッシブモード切替。",
								textColor );   // アグレッシブモードを切り替える // Переключает режим агрессии скелета
					textY += lineSpacing;

					DrawString( textX + 500, textY, "ゲーム目標:", keyColor );	 // ゲーム目標の説明 // Описание цели игры
					textY += lineSpacing;

					DrawString( textX,
								textY,
								"インフェルノの門を開けるためには",
								textColor );   // インフェルノの門を開ける条件 // Чтобы открыть врата Инферно
					textY += lineSpacing;
					DrawString( textX,
								textY,
								"デスの仮面の形をした鍵を見つける必要があります。",
								textColor );   // 鍵を見つける必要があります // Нужно найти ключ в форме маски смерти
					textY += lineSpacing;

					// 続行するためのヒント // Подсказка для продолжения
					SetDrawBlendMode( DX_BLENDMODE_ALPHA, alphaValue );
					DrawString( textX + 300, textY + 100, "PRESS THE SPACE BAR TO CONTINUE", GetColor( 255, 255, 0 ) );
					SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
				}
			}
			else
			{
				DrawExtendGraph( 0, 0, 10, 10, img, TRUE );
				SetFontSize( fontSize + 50 );
				DrawExtendGraphF( exitButtonX, exitButtonY, exitButtonX + 600, exitButtonY + 100, loadbar, TRUE );
				img = imgLoad;
			}
		}

		// 終了処理 // Завершение работы
		void TitleScene::Exit()
		{
			// タイトル終了時に行いたいことは今はない // Нет действий при завершении сцены
		}

		void TitleScene::GUI()
		{
		}

	}	// namespace GameSample
}	// namespace Sample
