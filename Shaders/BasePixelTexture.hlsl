Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelInput input) : SV_TARGET
{
    float4 textureColor;

    //textureColor = float4(0.3f, 0.3f, 0.3f, 1.0f);

    textureColor = shaderTexture.Sample(SampleType, input.tex);

    return textureColor;
}