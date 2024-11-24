#include "Sorc.h"
#include "Mask.h"

#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentTargetTracking.h>
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentAttachModel.h>

#include <cmath>

namespace Sample
{
	namespace GameSample
	{

		MaskPtr Mask::Create( const float3& pos, const float3& front )
		{
			// ” ‚Ìì¬ // „R„€„x„t„p„~„y„u „€„q„Œ„u„{„„„p
			auto goal = Scene::CreateObject<Mask>();
			goal->SetName( "Mask" );   // –¼‘O‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „y„}„u„~„y „€„q„Œ„u„{„„„p

			// vec‚Ì•ûŒü‚ÉŒü‚¯‚é // „T„ƒ„„„p„~„€„r„|„u„~„y„u „~„p„„‚„p„r„|„u„~„y„‘ „„€ „r„u„{„„„€„‚„…
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			goal->SetMatrix( mat );	  // s—ñ‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „}„p„„„‚„y„ˆ„

			// pos‚ÌˆÊ’u‚Éİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „„€„x„y„ˆ„y„y „€„q„Œ„u„{„„„p
			goal->SetTranslate( pos );

			return goal;   // ƒIƒuƒWƒFƒNƒg‚ğ•Ô‚· // „B„€„x„r„‚„p„„ „€„q„Œ„u„{„„„p
		}

		bool Mask::Init()	// override
		{
			__super::Init();

			// ƒ‚ƒfƒ‹ƒRƒ“ƒ|[ƒlƒ“ƒg(0.03”{) // „D„€„q„p„r„|„u„~„y„u „{„€„}„„€„~„u„~„„„p „}„€„t„u„|„y („}„p„ƒ„Š„„„p„q 0.03)
			auto model = AddComponent<ComponentModel>( "data/Sample/Key/source/death_mask.mv1" );

			model->SetScaleAxisXYZ( { 0.03f } );   // ƒXƒP[ƒ‹‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „}„p„ƒ„Š„„„p„q„p „}„€„t„u„|„y
			model->SetTranslate( { 0, 6, 0 } );	   // ƒgƒ‰ƒ“ƒXƒŒ[ƒg‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „„„‚„p„~„ƒ„|„‘„ˆ„y„y


			return true;   // ‰Šú‰»‚ÌŒ‹‰Ê‚ğ•Ô‚· // „B„€„x„r„‚„p„„ „‚„u„x„…„|„„„„p„„„p „y„~„y„ˆ„y„p„|„y„x„p„ˆ„y„y
		}

		//! @brief ˆ—XV // „O„q„~„€„r„|„u„~„y„u „€„q„‚„p„q„€„„„{„y
		//! @param delta ƒtƒŒ[ƒ€ŠÔ // „B„‚„u„}„‘ „{„p„t„‚„p
		void Mask::Update( [[maybe_unused]] float delta )	// override
		{
			auto mdl = GetComponent<ComponentModel>();
		}

		//! @brief “–‚½‚è‚ÌƒR[ƒ‹ƒoƒbƒN // „K„€„|„q„„{ „„‚„y „ƒ„„„€„|„{„~„€„r„u„~„y„y
		//! @param hitInfo // „I„~„†„€„‚„}„p„ˆ„y„‘ „€ „ƒ„„„€„|„{„~„€„r„u„~„y„y
		void Mask::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )	  // override
		{
			// “–‚½‚è‚ÅˆÚ“®‚³‚¹‚é(‚±‚ê‚ª–³‚¯‚ê‚Î‚ß‚è‚İ‚Ü‚·) // „O„q„‚„p„q„€„„„{„p „ƒ„„„€„|„{„~„€„r„u„~„y„‘ („q„u„x „„„„€„s„€ „€„q„Œ„u„{„„ „„‚„€„r„p„|„y„r„p„u„„„ƒ„‘)
			__super::OnHit( hitInfo );
		}

		void Mask::SetPositionAndDirection( const float3& handPos, const float3& vec )
		{
			SetTranslate( handPos + float3( 0, -1, 0 ) );	// ˆÊ’u‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „„€„x„y„ˆ„y„y
			vec_ = normalize( vec );						// ƒxƒNƒgƒ‹‚Ì³‹K‰» // „N„€„‚„}„y„‚„€„r„p„~„y„u „r„u„{„„„€„‚„p
		}

		void Mask::SetDelay( float delay )
		{
			delay_ = delay;	  // ’x‰„‚Ìİ’è // „T„ƒ„„„p„~„€„r„|„u„~„y„u „x„p„t„u„‚„w„{„y
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE ‚ÆƒZƒbƒg‚Å•K—v (Editorã‚ÅCreateObject‚·‚é‚½‚ß) // „S„‚„u„q„…„u„„„ƒ„‘ „ƒ BP_OBJECT_TYPE „t„|„‘ „ƒ„€„x„t„p„~„y„‘ „€„q„Œ„u„{„„„p „r „‚„u„t„p„{„„„€„‚„u
BP_OBJECT_IMPL( Sample::GameSample::Mask, u8"Mask" );

// ƒIƒuƒWƒFƒNƒgƒZ[ƒu\‘¢‚ğ’Ç‰Á‚µ‚Ü‚· // „D„€„q„p„r„|„u„~„y„u „ƒ„„„‚„…„{„„„…„‚„ „t„|„‘ „ƒ„€„‡„‚„p„~„u„~„y„‘ „€„q„Œ„u„{„„„p
CEREAL_REGISTER_TYPE( Sample::GameSample::Mask )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Mask )
