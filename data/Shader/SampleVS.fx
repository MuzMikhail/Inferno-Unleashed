
//---------------------------------------------------------------------------------
//	���_�V�F�[�_�[
//---------------------------------------------------------------------------------

//	�󂯎��e���_�̏��
struct VS_INPUT
{
	float4 LPos		: POSITION;		//	���[�J�����W
	float3 LNormal	: NORMAL;		//	���[�J���@��
	float4 Color	: COLOR;		//	�F�i���f���̐F�j

	//	���f���f�[�^������炦��t�u���W
	float2 UV		: TEXCOORD0;
};

//	�s�N�Z���V�F�[�_�[�ɓn�����
struct VS_OUTPUT
{
	float4 SPos		: POSITION;		//	��ʍ��W
	float4 Color	: COLOR;		//	�F

	float3 WNormal	: NORMAL;

	float4 WPos		: TEXCOORD1;

	//	�t�u���W���s�N�Z���V�F�[�_�[�Ŏg�������̂�
	//	�s�N�Z���V�F�[�_�[�ɓn�����̂ɂ�����ǉ����܂�
	float2 UV		: TEXCOORD0;
};

float4x4 World		: register( c94 );	//	���[���h�ϊ��s��
float4x4 View		: register( c6 );	//	�r���[�ϊ��s��
float4x4 Projection	: register( c2 );	//	�v���W�F�N�V�����ϊ��s��

//---------------------------------------------------------------------------------
//	�e���_�̏���
//---------------------------------------------------------------------------------
VS_OUTPUT main( VS_INPUT In )
{
	//	���C�g�̕���
	float3 LightDir = { 1.0f, -1.0f, 1.0f };
	//	����
	float4 Ambient = { 0.3f, 0.3f, 0.3f, 1.0f };

	VS_OUTPUT Out = (VS_OUTPUT)0;

	//	���[�J�����W�����[���h���W�ɕϊ�
	float4 WPos = mul( In.LPos, World );
	//	���[���h���W���J�������W�ɕϊ�
	float4 CPos = mul( WPos, View );
	//	�J�������W����ʍ��W�ɕϊ�
	Out.SPos = mul( CPos, Projection );

	//	���[���h�@�����v�Z
	float3 N = normalize( mul( In.LNormal, (float3x3)World ) );

	//	�F��n��
	Out.Color = In.Color;
	//	���[���h�@����n��
	Out.WNormal = N;
	//	���[���h���W��n��
	Out.WPos = WPos;

	//	�t�u���W���s�N�Z���V�F�[�_�[�ɓn���܂�
	Out.UV = In.UV;

	return Out;
}
