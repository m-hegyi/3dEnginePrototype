Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float fogFactor : FOG;
};

float4 main(PixelInput input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 fogColor;
    float4 color;


    textureColor = shaderTexture.Sample(SampleType, input.tex);

    color = ambientColor;

    lightDir = -lightDirection;

    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
    }

    color = saturate(color);

    color = color * textureColor;

    fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

    //color = input.fogFactor * color + (1.0 - input.fogFactor) * fogColor;

    return color;
}