cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
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
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;

    float4 pos = { input.position.x, input.position.y, input.position.z, 1.0f };

    output.position = mul(pos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3) worldMatrix);

    output.normal = normalize(output.normal);

    return output;
}