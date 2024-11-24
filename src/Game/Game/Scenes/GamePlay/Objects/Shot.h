#pragma once

#include <System/Scene.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionModel.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Shot );

		// 発射オブジェクトのクラス // Класс объекта выстрела
		class Shot : public Object
		{
		public:
			BP_OBJECT_TYPE( Shot, Object );

			// 発生用関数 // Функция создания
			// pos: 発生場所, vec: 方向 // pos: Позиция создания, vec: Направление
			static ShotPtr Create( const float3& handPos, const float3& vec );

			bool Init() override;	// 初期化 // Инициализация

			void Update( float delta ) override;   // 更新 // Обновление

			void Draw() override;	// 描画 // Отображение

			// 衝突時のコールバック // Коллбэк при столкновении
			void OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo ) override;

			void SetPositionAndDirection( const float3& handPos,
										  const float3& vec );	 // 位置と方向の設定 // Установка позиции и направления
			void SetDelay( float delay );						 // 遅延の設定 // Установка задержки

		private:
			float3 vec_					= { 0, 0, 0 };	 // 移動方向 // Направление движения
			float3 speed_				= 100.0f;		 // 移動速度 // Скорость движения
			float  radius_				= 1.0f;			 // 衝突の半径 // Радиус столкновения
			float  time_since_creation_ = 0.0f;			 // 作成からの時間 // Время с момента создания
			float  delay_				= -0.0f;		 // 遅延時間 // Время задержки

			int	   effect_fb_	   = -1;			// エフェクトのID // ID эффекта
			int	   effect_play_fb_ = -1;			// 再生中のエフェクトID // ID проигрываемого эффекта
			float3 position_fb_	   = { 0, 0, 0 };	// エフェクトの位置 // Позиция эффекта
			int	   model_;							// モデルのID // ID модели
		};

	}	// namespace GameSample
}	// namespace Sample
