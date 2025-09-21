/*------------------------------
* ポリゴンシェーダー[PixelShader.hlsl]
*
* 制作者：山室飛龍		日付：2025/9/21
------------------------------*/

cbuffer PS_CONSTANT_BUFFER : register(b0)
{
    float4 color;
}
struct PixelShader_INPUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR0;
    float2 texCoord : TEXCOORD0;
};

Texture2D major_texture; //テクスチャ
SamplerState major_sampler; //テクスチャサンプラ

float4 main(PixelShader_INPUT ps_in) : SV_TARGET
{ //染めたい頂点シェーダー分実行される

    //テクスチャの色素情報を戻り値として返す
    float4 texcolor = major_texture.Sample(major_sampler, ps_in.texCoord);
    
    return texcolor * color; //とすると、各要素の演算結果が出力される

}