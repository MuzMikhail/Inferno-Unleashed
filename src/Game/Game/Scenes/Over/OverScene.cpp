#include "OverScene.h"
#include "../Title/TitleScene.h"

namespace Sample
{
	namespace GameSample
	{
		BP_CLASS_IMPL( OverScene, u8"GameSample/OverScene" );

		//! @brief Инициализация сцены Game Over // ゲームオーバーシーンの初期化
		bool OverScene::Init()
		{
			// ゲームオーバー画面の画像を読み込む // Загрузка изображения для экрана поражения
			deathimg = LoadGraph( "data/Sample/UI/death.png" );

			return true;
		}

		//! @brief Обновление сцены Game Over // ゲームオーバーシーンの更新
		void OverScene::Update( [[maybe_unused]] float delta )
		{
			// 3秒たったらタイトルへ // Через 3 секунды переход на титульный экран
			if( Scene::GetTime() >= 3.0f )
				Scene::Change( Scene::GetScene<TitleScene>() );
		}

		//! @brief Отображение сцены Game Over // ゲームオーバーシーンの描画
		void OverScene::Draw()
		{
			// 背景画像を描画 // Отображение фонового изображения
			DrawGraph( 0, 0, deathimg, TRUE );

			// ゲームオーバーのテキスト表示 // Вывод текста "Game Over"
			DrawFormatString( 100, 50, GetColor( 255, 0, 0 ), "GameOver" );
		}

		//! @brief События при выходе из сцены Game Over // ゲームオーバーシーン終了時のイベント
		void OverScene::Exit()
		{
			// タイトル終了時に行いたいことは今はない
			// Нет действий при выходе из сцены Game Over
		}

		//! @brief GUI отображение // GUIの表示
		void OverScene::GUI()
		{
		}

	}	// namespace GameSample
}	// namespace Sample
