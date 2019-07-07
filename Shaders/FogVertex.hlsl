cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer FogBuffer
{
    float fogStart;
    float fogEnd;
};

struct VertexInput
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float fogFactor : FOG;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    float4 cameraPosition;

    float4 pos = { input.position.x, input.position.y, input.position.z, 1.0f };

    output.position = mul(pos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3) worldMatrix);

    output.normal = normalize(output.normal);

    cameraPosition = mul(input.position, worldMatrix);
    cameraPosition = mul(cameraPosition, viewMatrix);

    output.fogFactor = saturate((fogEnd - output.position.z) / (fogEnd - fogStart));

    return output;
}