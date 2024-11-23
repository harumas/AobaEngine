struct DirectionalLight
{
    float3 direction;
    float3 color;
    float3 ambientLight;
};

struct PointLight
{
    float3 position;
    float3 color;
    float range;
};

cbuffer cbuff0 : register(b0)
{
    matrix world;
    matrix viewproj;
}

cbuffer cbuff1 : register(b1)
{
    DirectionalLight directionalLight;
}

cbuffer cbuff1 : register(b2)
{
    PointLight pointLight;
}
 
cbuffer cbuff2 : register(b3)
{
    float3 baseColor;
    float3 eyePos;
}

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldPos : TEXCOORD1;
};

float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);

PSInput VSMain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    PSInput result;
    result.worldPos = mul(world, pos);
    result.position = mul(viewproj, result.worldPos);
    result.normal = normalize(mul(world, normal));
    result.uv = uv;
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 directionNormal = normalize(directionalLight.direction);
    float3 diffuseDirection = CalcLambertDiffuse(directionNormal, directionalLight.color, input.normal);

    float3 lightDir = normalize(input.worldPos - pointLight.position);
    float3 distance = length(input.worldPos - pointLight.position);

    float3 diffusePoint = pointLight.color * max(0.0f, dot(input.normal, -lightDir));

	// ‰e‹¿—¦‚Í‹——£‚É”ä—á‚µ‚Ä¬‚³‚­‚È‚Á‚Ä‚¢‚­
    //float affect = (1.0f - distance / pointLight.range) / (1.0f + distance * a * a);
    float affect = 1.0f / max(0.01f, distance * distance);

    // step-11 ŠgU”½ËŒõ‚Æ‹¾–Ê”½ËŒõ‚ÉŒ¸Š—¦‚ğæZ‚µ‚Ä‰e‹¿‚ğã‚ß‚é
    diffusePoint *= affect;

    // step-12 2‚Â‚Ì”½ËŒõ‚ğ‡Z‚µ‚ÄÅI“I‚È”½ËŒõ‚ğ‹‚ß‚é
    float3 diffuse = diffusePoint + diffuseDirection;

    // ŠgU”½ËŒõ‚Æ‹¾–Ê”½ËŒõ‚ğ‘«‚µZ‚µ‚ÄAÅI“I‚ÈŒõ‚ğ‹‚ß‚é
    float3 light = diffuse + directionalLight.ambientLight;
    float4 finalColor = float4(baseColor * light, 1);

    return finalColor;
}


float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ƒsƒNƒZƒ‹‚Ì–@ü‚Æƒ‰ƒCƒg‚Ì•ûŒü‚Ì“àÏ‚ğŒvZ‚·‚é
    float t = dot(normal, -lightDirection);

    t = t * 0.5f + 0.5;
    t = t * t;

    // ŠgU”½ËŒõ‚ğŒvZ‚·‚é
    return lightColor * t;
}

