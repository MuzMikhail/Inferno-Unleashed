
//---------------------------------------------------------------------------------
//	�s�N�Z���V�F�[�_�[
//---------------------------------------------------------------------------------

//	�󂯎��e�s�N�Z���̏��
struct PS_INPUT
{
	float4 Color	: COLOR;		//	�F

	float3 WNormal	: NORMAL;

	float4 WPos		: TEXCOORD1;

	//	���_�V�F�[�_�[����󂯎��t�u���W
	float2 UV		: TEXCOORD0;
};

//	�v�Z�������ʁA��ʂɕ\�������F
struct PS_OUTPUT
{
	float4 Color	: COLOR;		//	�F
};

//	�v���O����������n���ꂽ�l
float Param : register( c22 );

//	����̓ǂݍ��܂�Ă���e�N�X�`���̐錾
sampler Texture : register( s0 );

sampler Texture2 : register( s1 );

//---------------------------------------------------------------------------------
//	�e�s�N�Z���̏���
//---------------------------------------------------------------------------------
PS_OUTPUT main( PS_INPUT In )
{
	//	���C�g�̕���
	float3 LightDir = { 1.0f, -1.0f, 1.0f };
	//	����
	float4 Ambient = { 0.3f, 0.3f, 0.3f, 1.0f };

	PS_OUTPUT Out;

	//	���C�g�̌����x�N�g��
	float3 L = normalize( -LightDir );
	//	���[���h�@���x�N�g��
	float3 N = normalize( In.WNormal );

	float4 Color = tex2D( Texture, In.UV );

	if (In.WPos.y < Param) {
		Color = tex2D(Texture2, In.UV);
	}


	//	���C�g�̕����Ɩ@�����狭�������߂Ė��邳�v�Z
	Out.Color = Color * max( Ambient, dot( N, L ) );

	return Out;
}
