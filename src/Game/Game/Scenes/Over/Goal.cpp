#include "Goal.h"
#include "../Title/TitleScene.h"

namespace Sample
{
	namespace GameSample
	{
		BP_CLASS_IMPL( Goal, u8"GameSample/Goal" );

		//! @brief Инициализация сцены победы // 勝利シーンの初期化
		bool Goal::Init()
		{
			// 勝利画面の画像を読み込む // Загрузка изображения победы
			win		= LoadGraph( "data/Sample/UI/win.jpg" );
			winText = LoadGraph( "data/Sample/UI/winT.png" );
			return true;
		}

		//! @brief Обновление сцены // シーンの更新
		void Goal::Update( [[maybe_unused]] float delta )
		{
			// 3秒たったらタイトルへ // Через 3 секунды переход на титульный экран
			if( Scene::GetTime() >= 3.0f )
				Scene::Change( Scene::GetScene<TitleScene>() );
		}

		//! @brief Отображение сцены победы // 勝利シーンの描画
		void Goal::Draw()
		{
			// 背景画像を描画 // Отображение фонового изображения
			DrawGraph( 0, 0, win, TRUE );

			// 勝利テキストの描画 // Отображение текста победы
			DrawExtendGraphF( WINDOW_W / 2 - 200, WINDOW_H / 2 - 100, WINDOW_W / 2 + 200, WINDOW_H / 2 + 100, winText, TRUE );
		}

		//! @brief События при выходе из сцены победы // 勝利シーン終了時のイベント
		void Goal::Exit()
		{
			// タイトル終了時に行いたいことは今はない
			// Нет действий при выходе из сцены победы
		}

		//! @brief GUI отображение // GUIの表示
		void Goal::GUI()
		{
		}

	}	// namespace GameSample
}	// namespace Sample
