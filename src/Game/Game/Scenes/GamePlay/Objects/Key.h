#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Key );

		// 鍵オブジェクト // Объект ключа
		class Key : public Object
		{
		public:
			BP_OBJECT_TYPE( Key, Object );

			// 鍵オブジェクトの生成 // Создание объекта ключа
			static KeyPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// 初期化 // Инициализация объекта
			// @return 初期化の成否 // Результат инициализации
			bool Init() override;

			// フレームごとの更新処理 // Обновление каждый кадр
			// @param delta フレーム時間 // Время кадра
			void Update( [[maybe_unused]] float delta ) override;

			// 衝突時のコールバック // Коллбэк при столкновении
			// @param hitInfo 衝突情報 // Информация о столкновении
			void OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo ) override;

		private:
			int	   effect_k_	  = -1;			   // 鍵のエフェクト // Эффект ключа
			int	   effect_play_k_ = -1;			   // 鍵の再生エフェクト // Эффект проигрывания ключа
			float3 position_k_	  = { 0, 0, 0 };   // 鍵の位置 // Позиция ключа
		};
	}	// namespace GameSample
}	// namespace Sample
