#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Deathbody );

		// �`��͂���܂���BObject�W���ɂď�������܂� // �U���~�{���y�� Draw() ���������������r���u��. �O�q���p�q�����{�p �r�������|�~���u������ ���y�����u�}���z Object ���� ���}���|���p�~�y��.
		class Deathbody : public Object
		{
		public:
			BP_OBJECT_TYPE( Deathbody, Object );

			// Deathbody�I�u�W�F�N�g���쐬����ÓI���\�b�h // �R���p���y���u���{�y�z �}�u�����t �t�|�� �����x�t�p�~�y�� ���q���u�{���p Deathbody
			static DeathbodyPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// ������ // �I�~�y���y�p�|�y�x�p���y�� ���q���u�{���p
			// �������ł������ǂ�����Ԃ� // �B���x�r���p���p�u�� �������u���~�������� �y�~�y���y�p�|�y�x�p���y�y
			bool Init() override;

			// �����X�V // �O�q�~���r�|�u�~�y�u ���q���p�q�����{�y
			// @param delta �t���[������ // �B���u�}�� �{�p�t���p
			void Update( [[maybe_unused]] float delta ) override;

			// �I�u�W�F�N�g�̈ʒu�ƕ�����ݒ� // �T�����p�~���r�{�p �����x�y���y�y �y �~�p�����p�r�|�u�~�y�� ���q���u�{���p
			// @param handPos ��̈ʒu // �P���x�y���y�� �����{�y
			// @param vec �����x�N�g�� // �B�u�{������ �~�p�����p�r�|�u�~�y��
			void SetPositionAndDirection( const float3& handPos, const float3& vec );

		private:
			float3 vec_ = { 0, 0, 0 };	 // �I�u�W�F�N�g�̕����x�N�g�� // �B�u�{������ �~�p�����p�r�|�u�~�y�� ���q���u�{���p

		public:
		};
	}	// namespace GameSample
}	// namespace Sample
