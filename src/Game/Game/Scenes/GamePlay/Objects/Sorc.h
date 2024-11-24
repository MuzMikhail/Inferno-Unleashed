#pragma once

#include <System/Scene.h>
#include "../Gauge.h"

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Sorc );

		//! @brief プレイヤー Sorc
		//! @detail Draw()は存在しません。Object標準にて描画されます
		class Sorc : public Object
		{
		public:
			BP_OBJECT_TYPE( Sorc, Object );

			//! @brief 生成関数
			static SorcPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			//! @name システムオーバーライド系
			// @{

			bool Init() override;

			void Update( [[maybe_unused]] float delta ) override;

			// 基本描画の後に処理します
			void LateDraw() override;

			void GUI() override;

			void OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo ) override;

			// @}
			//! @name 内部アクセス用
			// @{

			//! @brief スピードを設定する
			//! @param s スピード
			void SetSpeed( float s );

			//! @brief スピードを取得する
			//! @return 現在のスピード
			float GetSpeed();
			void  TakeDamage( int damage );

			//! @brief 変数として扱う関数
			//! @detail Set/Get系と比べデバックはしにくい
			//! @return 現在のスピードの変数(speed_)
			float& Speed();

			// @}
		public:
			bool maskAct = false;

		private:
			float speed_		 = 1.0f;
			float rot_y_		 = 0.0f;
			float rot_x_		 = 0.0f;
			int	  hit_point_	 = 100;
			int	  stamina_point_ = 100;

			int	   effect_sw_	   = -1;
			int	   effect_play_sw_ = -1;
			int	   effect_f_	   = -1;
			int	   effect_play_f_  = -1;
			float3 position_	   = { 0, 0, 0 };
			float3 position_mask_  = { 0, 0, 0 };
			int	   minutes		   = 0;
			int	   seconds		   = 0;
			int	   hours		   = 0;
			int	   maskImg		   = LoadGraph( "data/Sample/Key/mask.png" );


			HP::Gauge gauge_;
		};

	}	// namespace GameSample
}	// namespace Sample
