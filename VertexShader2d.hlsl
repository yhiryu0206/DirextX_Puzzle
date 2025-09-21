/*------------------------------
* 頂点シェーダー[VertexShader.hlsl]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/

//行列を使いたいので 4x4
//↓定数バッファと呼ばれるもの
//
float4x4 mtx;

cbuffer VS_CONSTANT_BUFFER0 : register(b0)
{
    float4x4 proj;
}

cbuffer VS_CONSTANT_BUFFER1 : register(b1)
{
    float4x4 world;
}

struct VertexShader_INPUT
{ //この構造体に頂点ごとに入れられた情報を実際に入れる
    float4 posL : POSITION;
    float4 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

struct VertexShader_OUTPUT
{ //この構造体で実際に入れた情報を戻り値で戻す
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

//下の : SV_POSITION は「semantics」と呼び、何をするものなのか示すために使う
VertexShader_OUTPUT main(VertexShader_INPUT vs_in)//←ここでついでに VertexShader_INPUT も宣言している。便利だね
{ //描きたい頂点分実行される
    
    //構造体宣言
    VertexShader_OUTPUT vs_out;
    
    //float4x4 mtx = mul(world, proj);
    
     //座標変換(mulは multiple のmul)
    vs_out.posH = mul(vs_in.posL, mtx);
    
    //入れた色素情報を橋渡し
    vs_out.color = vs_in.color;
    
    //テクスチャ座標も同様
    vs_out.texCoord = vs_in.texCoord;
   
    //入れた情報を戻り値で戻す
    return vs_out;
}
