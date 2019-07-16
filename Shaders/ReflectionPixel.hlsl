Texture2D shaderTexture;
SamplerState SampleType;
Texture2D reflectionTexture;

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
    float4 reflectionPosition : TEXCOORD1;
};

float4 main(PixelInput input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float2 reflectTexCoord;
    float4 reflectionColor;

    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // Calculate the projected reflection texture coordinates.
    reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
    reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

    // Sample the texture pixel from the reflection texture using the projected texture coordinates.
    reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

    // Do a linear interpolation between the two textures for a blend effect.
    color = lerp(textureColor, reflectionColor, 0.15f);

    /*color = ambientColor;

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
*/
    return color;
}