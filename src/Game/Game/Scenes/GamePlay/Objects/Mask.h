#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Mask );

		// ƒ}ƒXƒNƒIƒuƒWƒFƒNƒg // „O„q„Œ„u„{„„ „}„p„ƒ„{„y
		class Mask : public Object
		{
		public:
			BP_OBJECT_TYPE( Mask, Object );

			// ƒ}ƒXƒNƒIƒuƒWƒFƒNƒg‚Ì¶¬ // „R„€„x„t„p„~„y„u „€„q„Œ„u„{„„„p „}„p„ƒ„{„y
			static MaskPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// ‰Šú‰» // „I„~„y„ˆ„y„p„|„y„x„p„ˆ„y„‘ „€„q„Œ„u„{„„„p
			// @return ‰Šú‰»‚Ì¬”Û // „Q„u„x„…„|„„„„p„„ „y„~„y„ˆ„y„p„|„y„x„p„ˆ„y„y
			bool Init() override;

			// ƒtƒŒ[ƒ€‚²‚Æ‚ÌXVˆ— // „O„q„~„€„r„|„u„~„y„u „{„p„w„t„„z „{„p„t„‚
			// @param delta ƒtƒŒ[ƒ€ŠÔ // „B„‚„u„}„‘ „{„p„t„‚„p
			void Update( [[maybe_unused]] float delta ) override;

			// Õ“Ë‚ÌƒR[ƒ‹ƒoƒbƒN // „K„€„|„|„q„„{ „„‚„y „ƒ„„„€„|„{„~„€„r„u„~„y„y
			// @param hitInfo Õ“Ëî•ñ // „I„~„†„€„‚„}„p„ˆ„y„‘ „€ „ƒ„„„€„|„{„~„€„r„u„~„y„y
			void OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo ) override;

			// ƒ}ƒXƒN‚ÌˆÊ’u‚Æ•ûŒü‚Ìİ’è // „T„ƒ„„„p„~„€„r„{„p „„€„x„y„ˆ„y„y „y „~„p„„‚„p„r„|„u„~„y„‘ „}„p„ƒ„{„y
			void SetPositionAndDirection( const float3& handPos, const float3& vec );

			// ’x‰„ŠÔ‚Ìİ’è // „T„ƒ„„„p„~„€„r„{„p „r„‚„u„}„u„~„y „x„p„t„u„‚„w„{„y
			void SetDelay( float delay );

		private:
			int	   effect_k_	  = -1;			   // ƒ}ƒXƒN‚ÌƒGƒtƒFƒNƒg // „^„†„†„u„{„„ „}„p„ƒ„{„y
			int	   effect_play_k_ = -1;			   // Ä¶‚³‚ê‚éƒ}ƒXƒN‚ÌƒGƒtƒFƒNƒg // „^„†„†„u„{„„ „„‚„€„y„s„‚„„r„p„~„y„‘ „}„p„ƒ„{„y
			float3 position_k_	  = { 0, 0, 0 };   // ƒ}ƒXƒN‚ÌˆÊ’u // „P„€„x„y„ˆ„y„‘ „}„p„ƒ„{„y
			float3 vec_			  = { 0, 0, 0 };   // ˆÚ“®•ûŒüƒxƒNƒgƒ‹ // „B„u„{„„„€„‚ „~„p„„‚„p„r„|„u„~„y„‘ „t„r„y„w„u„~„y„‘
			float  delay_		  = 0.0f;		   // •\¦’x‰„ // „H„p„t„u„‚„w„{„p „€„„„€„q„‚„p„w„u„~„y„‘
		};
	}	// namespace GameSample
}	// namespace Sample
