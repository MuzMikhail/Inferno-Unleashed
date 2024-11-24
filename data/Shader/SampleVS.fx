
//---------------------------------------------------------------------------------
//	頂点シェーダー
//---------------------------------------------------------------------------------

//	受け取る各頂点の情報
struct VS_INPUT
{
	float4 LPos		: POSITION;		//	ローカル座標
	float3 LNormal	: NORMAL;		//	ローカル法線
	float4 Color	: COLOR;		//	色（モデルの色）

	//	モデルデータからもらえるＵＶ座標
	float2 UV		: TEXCOORD0;
};

//	ピクセルシェーダーに渡す情報
struct VS_OUTPUT
{
	float4 SPos		: POSITION;		//	画面座標
	float4 Color	: COLOR;		//	色

	float3 WNormal	: NORMAL;

	float4 WPos		: TEXCOORD1;

	//	ＵＶ座標をピクセルシェーダーで使いたいので
	//	ピクセルシェーダーに渡すものにこれも追加します
	float2 UV		: TEXCOORD0;
};

float4x4 World		: register( c94 );	//	ワールド変換行列
float4x4 View		: register( c6 );	//	ビュー変換行列
float4x4 Projection	: register( c2 );	//	プロジェクション変換行列

//---------------------------------------------------------------------------------
//	各頂点の処理
//---------------------------------------------------------------------------------
VS_OUTPUT main( VS_INPUT In )
{
	//	ライトの方向
	float3 LightDir = { 1.0f, -1.0f, 1.0f };
	//	環境光
	float4 Ambient = { 0.3f, 0.3f, 0.3f, 1.0f };

	VS_OUTPUT Out = (VS_OUTPUT)0;

	//	ローカル座標をワールド座標に変換
	float4 WPos = mul( In.LPos, World );
	//	ワールド座標をカメラ座標に変換
	float4 CPos = mul( WPos, View );
	//	カメラ座標を画面座標に変換
	Out.SPos = mul( CPos, Projection );

	//	ワールド法線を計算
	float3 N = normalize( mul( In.LNormal, (float3x3)World ) );

	//	色を渡す
	Out.Color = In.Color;
	//	ワールド法線を渡す
	Out.WNormal = N;
	//	ワールド座標を渡す
	Out.WPos = WPos;

	//	ＵＶ座標もピクセルシェーダーに渡します
	Out.UV = In.UV;

	return Out;
}
