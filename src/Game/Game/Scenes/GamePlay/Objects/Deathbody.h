#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Deathbody );

		// 描画はありません。Object標準にて処理されます // Функция Draw() отсутствует. Обработка выполняется системой Object по умолчанию.
		class Deathbody : public Object
		{
		public:
			BP_OBJECT_TYPE( Deathbody, Object );

			// Deathbodyオブジェクトを作成する静的メソッド // Статический метод для создания объекта Deathbody
			static DeathbodyPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// 初期化 // Инициализация объекта
			// 初期化できたかどうかを返す // Возвращает успешность инициализации
			bool Init() override;

			// 処理更新 // Обновление обработки
			// @param delta フレーム時間 // Время кадра
			void Update( [[maybe_unused]] float delta ) override;

			// オブジェクトの位置と方向を設定 // Установка позиции и направления объекта
			// @param handPos 手の位置 // Позиция руки
			// @param vec 方向ベクトル // Вектор направления
			void SetPositionAndDirection( const float3& handPos, const float3& vec );

		private:
			float3 vec_ = { 0, 0, 0 };	 // オブジェクトの方向ベクトル // Вектор направления объекта

		public:
		};
	}	// namespace GameSample
}	// namespace Sample
