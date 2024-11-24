#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Deathbody );

		// •`‰æ‚Í‚ ‚è‚Ü‚¹‚ñBObject•W€‚É‚Äˆ—‚³‚ê‚Ü‚· // „U„…„~„{„ˆ„y„‘ Draw() „€„„„ƒ„…„„„ƒ„„„r„…„u„„. „O„q„‚„p„q„€„„„{„p „r„„„€„|„~„‘„u„„„ƒ„‘ „ƒ„y„ƒ„„„u„}„€„z Object „„€ „…„}„€„|„‰„p„~„y„.
		class Deathbody : public Object
		{
		public:
			BP_OBJECT_TYPE( Deathbody, Object );

			// DeathbodyƒIƒuƒWƒFƒNƒg‚ğì¬‚·‚éÃ“Iƒƒ\ƒbƒh // „R„„„p„„„y„‰„u„ƒ„{„y„z „}„u„„„€„t „t„|„‘ „ƒ„€„x„t„p„~„y„‘ „€„q„Œ„u„{„„„p Deathbody
			static DeathbodyPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// ‰Šú‰» // „I„~„y„ˆ„y„p„|„y„x„p„ˆ„y„‘ „€„q„Œ„u„{„„„p
			// ‰Šú‰»‚Å‚«‚½‚©‚Ç‚¤‚©‚ğ•Ô‚· // „B„€„x„r„‚„p„‹„p„u„„ „…„ƒ„„u„Š„~„€„ƒ„„„ „y„~„y„ˆ„y„p„|„y„x„p„ˆ„y„y
			bool Init() override;

			// ˆ—XV // „O„q„~„€„r„|„u„~„y„u „€„q„‚„p„q„€„„„{„y
			// @param delta ƒtƒŒ[ƒ€ŠÔ // „B„‚„u„}„‘ „{„p„t„‚„p
			void Update( [[maybe_unused]] float delta ) override;

			// ƒIƒuƒWƒFƒNƒg‚ÌˆÊ’u‚Æ•ûŒü‚ğİ’è // „T„ƒ„„„p„~„€„r„{„p „„€„x„y„ˆ„y„y „y „~„p„„‚„p„r„|„u„~„y„‘ „€„q„Œ„u„{„„„p
			// @param handPos è‚ÌˆÊ’u // „P„€„x„y„ˆ„y„‘ „‚„…„{„y
			// @param vec •ûŒüƒxƒNƒgƒ‹ // „B„u„{„„„€„‚ „~„p„„‚„p„r„|„u„~„y„‘
			void SetPositionAndDirection( const float3& handPos, const float3& vec );

		private:
			float3 vec_ = { 0, 0, 0 };	 // ƒIƒuƒWƒFƒNƒg‚Ì•ûŒüƒxƒNƒgƒ‹ // „B„u„{„„„€„‚ „~„p„„‚„p„r„|„u„~„y„‘ „€„q„Œ„u„{„„„p

		public:
		};
	}	// namespace GameSample
}	// namespace Sample
