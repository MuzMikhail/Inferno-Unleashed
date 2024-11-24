#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Portal );

		// ポータルオブジェクト // Объект портала
		class Portal : public Object
		{
		public:
			BP_OBJECT_TYPE( Portal, Object );

			// ポータルオブジェクトの生成 // Создание объекта портала
			static PortalPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// 初期化 // Инициализация
			// @return 初期化の成否 // Успешность инициализации
			bool Init() override;

			// フレームごとの更新処理 // Обновление каждый кадр
			// @param delta フレーム時間 // Время кадра
			void Update( [[maybe_unused]] float delta ) override;

			// 衝突時のコールバック // Коллбэк при столкновении
			// @param hitInfo 衝突情報 // Информация о столкновении
			void OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo ) override;

		private:
			int effect_p_	   = -1;   // ポータルのエフェクト // Эффект портала
			int effect_play_p_ = -1;   // 再生されるポータルのエフェクト // Эффект проигрывания портала
			float3 position_ = { 0, 0, 0 };	  // ポータルの位置 // Позиция портала
		};
	}	// namespace GameSample
}	// namespace Sample
