cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};
cbuffer ReflectionBuffer
{
    matrix reflectionMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 instancePosition : TEXCOORD1;
    float3 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 reflectionPosiiton : TEXCOORD1;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    matrix reflectProjectWorld;

    input.position.w = 1.0f;

     // Update the position of the vertices based on the data for this particular instance.
    input.position.x += input.instancePosition.x;
    input.position.y += input.instancePosition.y;
    input.position.z += input.instancePosition.z;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3) worldMatrix);

    output.normal = normalize(output.normal);

    //Create the reflection projection world matrix
    reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
    reflectProjectWorld = mul(worldMatrix, reflectProjectWorld);

    // Calculate the input position against the reflectionProjectionWorld matrix
    output.reflectionPosiiton = mul(input.position, reflectProjectWorld);

    return output;
}