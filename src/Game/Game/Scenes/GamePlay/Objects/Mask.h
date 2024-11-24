#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Mask );

		// �}�X�N�I�u�W�F�N�g // �O�q���u�{�� �}�p���{�y
		class Mask : public Object
		{
		public:
			BP_OBJECT_TYPE( Mask, Object );

			// �}�X�N�I�u�W�F�N�g�̐��� // �R���x�t�p�~�y�u ���q���u�{���p �}�p���{�y
			static MaskPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// ������ // �I�~�y���y�p�|�y�x�p���y�� ���q���u�{���p
			// @return �������̐��� // �Q�u�x���|�����p�� �y�~�y���y�p�|�y�x�p���y�y
			bool Init() override;

			// �t���[�����Ƃ̍X�V���� // �O�q�~���r�|�u�~�y�u �{�p�w�t���z �{�p�t��
			// @param delta �t���[������ // �B���u�}�� �{�p�t���p
			void Update( [[maybe_unused]] float delta ) override;

			// �Փˎ��̃R�[���o�b�N // �K���|�|�q���{ �����y �������|�{�~���r�u�~�y�y
			// @param hitInfo �Փˏ�� // �I�~�������}�p���y�� �� �������|�{�~���r�u�~�y�y
			void OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo ) override;

			// �}�X�N�̈ʒu�ƕ����̐ݒ� // �T�����p�~���r�{�p �����x�y���y�y �y �~�p�����p�r�|�u�~�y�� �}�p���{�y
			void SetPositionAndDirection( const float3& handPos, const float3& vec );

			// �x�����Ԃ̐ݒ� // �T�����p�~���r�{�p �r���u�}�u�~�y �x�p�t�u���w�{�y
			void SetDelay( float delay );

		private:
			int	   effect_k_	  = -1;			   // �}�X�N�̃G�t�F�N�g // �^�����u�{�� �}�p���{�y
			int	   effect_play_k_ = -1;			   // �Đ������}�X�N�̃G�t�F�N�g // �^�����u�{�� �������y�s�����r�p�~�y�� �}�p���{�y
			float3 position_k_	  = { 0, 0, 0 };   // �}�X�N�̈ʒu // �P���x�y���y�� �}�p���{�y
			float3 vec_			  = { 0, 0, 0 };   // �ړ������x�N�g�� // �B�u�{������ �~�p�����p�r�|�u�~�y�� �t�r�y�w�u�~�y��
			float  delay_		  = 0.0f;		   // �\���x�� // �H�p�t�u���w�{�p �������q���p�w�u�~�y��
		};
	}	// namespace GameSample
}	// namespace Sample
