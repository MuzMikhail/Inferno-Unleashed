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
			// ���̍쐬 // �R���x�t�p�~�y�u ���q���u�{���p
			auto dbody = Scene::CreateObject<Deathbody>();
			dbody->SetName( "Deathbody" );	 // ���O�̐ݒ� // �T�����p�~���r�|�u�~�y�u �y�}�u�~�y ���q���u�{���p

			// vec�̕����Ɍ����� // �T�����p�~���r�|�u�~�y�u �~�p�����p�r�|�u�~�y�� ���� �r�u�{��������
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			dbody->SetMatrix( mat );   // �s��̐ݒ� // �T�����p�~���r�|�u�~�y�u �}�p�����y����

			// pos�̈ʒu�ɐݒ� // �T�����p�~���r�|�u�~�y�u �����x�y���y�y ���q���u�{���p
			dbody->SetTranslate( pos );

			return dbody;	// �I�u�W�F�N�g��Ԃ� // �B���x�r���p�� ���q���u�{���p
		}

		//! @brief ������ // �I�~�y���y�p�|�y�x�p���y��
		//! @return �������ł������ǂ��� // �B���x�r���p���p�u�� ���u�x���|�����p�� �y�~�y���y�p�|�y�x�p���y�y
		bool Deathbody::Init()	 // override
		{
			__super::Init();

			// ���f���R���|�[�l���g(0.08�{) // �D���q�p�r�|�u�~�y�u �{���}�����~�u�~���p �}���t�u�|�y (�}�p�������p�q 0.08)
			auto model = AddComponent<ComponentModel>( "data/Sample/Enemy/char/Zomb.mv1" );

			model->SetScaleAxisXYZ( { 0.08f } );   // �X�P�[���̐ݒ� // �T�����p�~���r�|�u�~�y�u �}�p�������p�q�p �}���t�u�|�y

			model->SetAnimation( { { "death",
									 "data/Sample/Enemy/char/Anim/zombi_death.mv1",
									 0,
									 1.0f } } );   // �A�j���[�V�����̐ݒ� // �T�����p�~���r�|�u�~�y�u �p�~�y�}�p���y�y

			if( model )
				model->PlayAnimation( "death", false );	  // �A�j���[�V�����Đ� // �H�p�������{ �p�~�y�}�p���y�y ���}�u�����y

			return true;   // �������̌��ʂ�Ԃ� // �B���x�r���p�� ���u�x���|�����p���p �y�~�y���y�p�|�y�x�p���y�y
		}

		//! @brief �����X�V // �O�q�~���r�|�u�~�y�u ���q���p�q�����{�y
		//! @param delta �t���[������ // �B���u�}�� �{�p�t���p
		void Deathbody::Update( [[maybe_unused]] float delta )	 // override
		{
		}

		void Deathbody::SetPositionAndDirection( const float3& handPos, const float3& vec )
		{
			SetTranslate( handPos + float3( 0, 0, 0 ) );   // �ʒu�̐ݒ� // �T�����p�~���r�|�u�~�y�u �����x�y���y�y
			vec_ = normalize( vec );					   // �x�N�g���̐��K�� // �N�����}�y�����r�p�~�y�u �r�u�{�������p
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE �ƃZ�b�g�ŕK�v (Editor���CreateObject���邽��) // �S���u�q���u������ �� BP_OBJECT_TYPE �t�|�� �����x�t�p�~�y�� ���q���u�{���p �r ���u�t�p�{�������u
BP_OBJECT_IMPL( Sample::GameSample::Deathbody, u8"Deathbody" );

// �I�u�W�F�N�g�Z�[�u�\����ǉ����܂� // �D���q�p�r�|�u�~�y�u ���������{�������� �t�|�� ���������p�~�u�~�y�� ���q���u�{���p
CEREAL_REGISTER_TYPE( Sample::GameSample::Deathbody )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Deathbody )
