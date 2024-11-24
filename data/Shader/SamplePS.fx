
//---------------------------------------------------------------------------------
//	ピクセルシェーダー
//---------------------------------------------------------------------------------

//	受け取る各ピクセルの情報
struct PS_INPUT
{
	float4 Color	: COLOR;		//	色

	float3 WNormal	: NORMAL;

	float4 WPos		: TEXCOORD1;

	//	頂点シェーダーから受け取るＵＶ座標
	float2 UV		: TEXCOORD0;
};

//	計算した結果、画面に表示される色
struct PS_OUTPUT
{
	float4 Color	: COLOR;		//	色
};

//	プログラム側から渡された値
float Param : register( c22 );

//	勝手の読み込まれているテクスチャの宣言
sampler Texture : register( s0 );

sampler Texture2 : register( s1 );

//---------------------------------------------------------------------------------
//	各ピクセルの処理
//---------------------------------------------------------------------------------
PS_OUTPUT main( PS_INPUT In )
{
	//	ライトの方向
	float3 LightDir = { 1.0f, -1.0f, 1.0f };
	//	環境光
	float4 Ambient = { 0.3f, 0.3f, 0.3f, 1.0f };

	PS_OUTPUT Out;

	//	ライトの向きベクトル
	float3 L = normalize( -LightDir );
	//	ワールド法線ベクトル
	float3 N = normalize( In.WNormal );

	float4 Color = tex2D( Texture, In.UV );

	if (In.WPos.y < Param) {
		Color = tex2D(Texture2, In.UV);
	}


	//	ライトの方向と法線から強さを求めて明るさ計算
	Out.Color = Color * max( Ambient, dot( N, L ) );

	return Out;
}
