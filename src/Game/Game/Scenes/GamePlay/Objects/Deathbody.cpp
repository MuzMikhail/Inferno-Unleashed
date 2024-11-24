#include "Precompile.h"
#include "Deathbody.h"
#include "Enemy.h"
#include "Sorc.h"

#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentTargetTracking.h>
#include <System/Component/ComponentSpringArm.h>

#include <cmath>

namespace Sample
{
	namespace GameSample
	{

		DeathbodyPtr Deathbody::Create( const float3& pos, const float3& front )
		{
			// ” ‚Ìì¬ // „R„€„x„t„p„~„y„u „€„q„Œ„u„{„„„p
			auto dbody = Scene::CreateObject<Deathbody>();
			dbody->SetName( "Deathbody" );	 // –¼‘O‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „y„}„u„~„y „€„q„Œ„u„{„„„p

			// vec‚Ì•ûŒü‚ÉŒü‚¯‚é // „T„ƒ„„„p„~„€„r„|„u„~„y„u „~„p„„‚„p„r„|„u„~„y„‘ „„€ „r„u„{„„„€„‚„…
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			dbody->SetMatrix( mat );   // s—ñ‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „}„p„„„‚„y„ˆ„

			// pos‚ÌˆÊ’u‚Éİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „„€„x„y„ˆ„y„y „€„q„Œ„u„{„„„p
			dbody->SetTranslate( pos );

			return dbody;	// ƒIƒuƒWƒFƒNƒg‚ğ•Ô‚· // „B„€„x„r„‚„p„„ „€„q„Œ„u„{„„„p
		}

		//! @brief ‰Šú‰» // „I„~„y„ˆ„y„p„|„y„x„p„ˆ„y„‘
		//! @return ‰Šú‰»‚Å‚«‚½‚©‚Ç‚¤‚© // „B„€„x„r„‚„p„‹„p„u„„ „‚„u„x„…„|„„„„p„„ „y„~„y„ˆ„y„p„|„y„x„p„ˆ„y„y
		bool Deathbody::Init()	 // override
		{
			__super::Init();

			// ƒ‚ƒfƒ‹ƒRƒ“ƒ|[ƒlƒ“ƒg(0.08”{) // „D„€„q„p„r„|„u„~„y„u „{„€„}„„€„~„u„~„„„p „}„€„t„u„|„y („}„p„ƒ„Š„„„p„q 0.08)
			auto model = AddComponent<ComponentModel>( "data/Sample/Enemy/char/Zomb.mv1" );

			model->SetScaleAxisXYZ( { 0.08f } );   // ƒXƒP[ƒ‹‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „}„p„ƒ„Š„„„p„q„p „}„€„t„u„|„y

			model->SetAnimation( { { "death",
									 "data/Sample/Enemy/char/Anim/zombi_death.mv1",
									 0,
									 1.0f } } );   // ƒAƒjƒ[ƒVƒ‡ƒ“‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „p„~„y„}„p„ˆ„y„y

			if( model )
				model->PlayAnimation( "death", false );	  // ƒAƒjƒ[ƒVƒ‡ƒ“Ä¶ // „H„p„„…„ƒ„{ „p„~„y„}„p„ˆ„y„y „ƒ„}„u„‚„„„y

			return true;   // ‰Šú‰»‚ÌŒ‹‰Ê‚ğ•Ô‚· // „B„€„x„r„‚„p„„ „‚„u„x„…„|„„„„p„„„p „y„~„y„ˆ„y„p„|„y„x„p„ˆ„y„y
		}

		//! @brief ˆ—XV // „O„q„~„€„r„|„u„~„y„u „€„q„‚„p„q„€„„„{„y
		//! @param delta ƒtƒŒ[ƒ€ŠÔ // „B„‚„u„}„‘ „{„p„t„‚„p
		void Deathbody::Update( [[maybe_unused]] float delta )	 // override
		{
		}

		void Deathbody::SetPositionAndDirection( const float3& handPos, const float3& vec )
		{
			SetTranslate( handPos + float3( 0, 0, 0 ) );   // ˆÊ’u‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „„€„x„y„ˆ„y„y
			vec_ = normalize( vec );					   // ƒxƒNƒgƒ‹‚Ì³‹K‰» // „N„€„‚„}„y„‚„€„r„p„~„y„u „r„u„{„„„€„‚„p
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE ‚ÆƒZƒbƒg‚Å•K—v (Editorã‚ÅCreateObject‚·‚é‚½‚ß) // „S„‚„u„q„…„u„„„ƒ„‘ „ƒ BP_OBJECT_TYPE „t„|„‘ „ƒ„€„x„t„p„~„y„‘ „€„q„Œ„u„{„„„p „r „‚„u„t„p„{„„„€„‚„u
BP_OBJECT_IMPL( Sample::GameSample::Deathbody, u8"Deathbody" );

// ƒIƒuƒWƒFƒNƒgƒZ[ƒu\‘¢‚ğ’Ç‰Á‚µ‚Ü‚· // „D„€„q„p„r„|„u„~„y„u „ƒ„„„‚„…„{„„„…„‚„ „t„|„‘ „ƒ„€„‡„‚„p„~„u„~„y„‘ „€„q„Œ„u„{„„„p
CEREAL_REGISTER_TYPE( Sample::GameSample::Deathbody )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Deathbody )
