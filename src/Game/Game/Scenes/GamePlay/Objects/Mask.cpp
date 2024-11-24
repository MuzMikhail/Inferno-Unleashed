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
			// ���̍쐬 // �R���x�t�p�~�y�u ���q���u�{���p
			auto goal = Scene::CreateObject<Mask>();
			goal->SetName( "Mask" );   // ���O�̐ݒ� // �T�����p�~���r�|�u�~�y�u �y�}�u�~�y ���q���u�{���p

			// vec�̕����Ɍ����� // �T�����p�~���r�|�u�~�y�u �~�p�����p�r�|�u�~�y�� ���� �r�u�{��������
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			goal->SetMatrix( mat );	  // �s��̐ݒ� // �T�����p�~���r�|�u�~�y�u �}�p�����y����

			// pos�̈ʒu�ɐݒ� // �T�����p�~���r�|�u�~�y�u �����x�y���y�y ���q���u�{���p
			goal->SetTranslate( pos );

			return goal;   // �I�u�W�F�N�g��Ԃ� // �B���x�r���p�� ���q���u�{���p
		}

		bool Mask::Init()	// override
		{
			__super::Init();

			// ���f���R���|�[�l���g(0.03�{) // �D���q�p�r�|�u�~�y�u �{���}�����~�u�~���p �}���t�u�|�y (�}�p�������p�q 0.03)
			auto model = AddComponent<ComponentModel>( "data/Sample/Key/source/death_mask.mv1" );

			model->SetScaleAxisXYZ( { 0.03f } );   // �X�P�[���̐ݒ� // �T�����p�~���r�|�u�~�y�u �}�p�������p�q�p �}���t�u�|�y
			model->SetTranslate( { 0, 6, 0 } );	   // �g�����X���[�g�̐ݒ� // �T�����p�~���r�|�u�~�y�u �����p�~���|�����y�y


			return true;   // �������̌��ʂ�Ԃ� // �B���x�r���p�� ���u�x���|�����p���p �y�~�y���y�p�|�y�x�p���y�y
		}

		//! @brief �����X�V // �O�q�~���r�|�u�~�y�u ���q���p�q�����{�y
		//! @param delta �t���[������ // �B���u�}�� �{�p�t���p
		void Mask::Update( [[maybe_unused]] float delta )	// override
		{
			auto mdl = GetComponent<ComponentModel>();
		}

		//! @brief ������̃R�[���o�b�N // �K���|�q���{ �����y �������|�{�~���r�u�~�y�y
		//! @param hitInfo // �I�~�������}�p���y�� �� �������|�{�~���r�u�~�y�y
		void Mask::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )	  // override
		{
			// ������ňړ�������(���ꂪ������΂߂荞�݂܂�) // �O�q���p�q�����{�p �������|�{�~���r�u�~�y�� (�q�u�x �������s�� ���q���u�{�� �������r�p�|�y�r�p�u������)
			__super::OnHit( hitInfo );
		}

		void Mask::SetPositionAndDirection( const float3& handPos, const float3& vec )
		{
			SetTranslate( handPos + float3( 0, -1, 0 ) );	// �ʒu�̐ݒ� // �T�����p�~���r�|�u�~�y�u �����x�y���y�y
			vec_ = normalize( vec );						// �x�N�g���̐��K�� // �N�����}�y�����r�p�~�y�u �r�u�{�������p
		}

		void Mask::SetDelay( float delay )
		{
			delay_ = delay;	  // �x���̐ݒ� // �T�����p�~���r�|�u�~�y�u �x�p�t�u���w�{�y
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE �ƃZ�b�g�ŕK�v (Editor���CreateObject���邽��) // �S���u�q���u������ �� BP_OBJECT_TYPE �t�|�� �����x�t�p�~�y�� ���q���u�{���p �r ���u�t�p�{�������u
BP_OBJECT_IMPL( Sample::GameSample::Mask, u8"Mask" );

// �I�u�W�F�N�g�Z�[�u�\����ǉ����܂� // �D���q�p�r�|�u�~�y�u ���������{�������� �t�|�� ���������p�~�u�~�y�� ���q���u�{���p
CEREAL_REGISTER_TYPE( Sample::GameSample::Mask )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Mask )
