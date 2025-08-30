// BrushMask.hlsl  
// Renders brush strokes into the MaskAtlas for real-time paint-to-audio conversion
// Supports different brush types: Paint, Quantize, Erase, Comb

cbuffer BrushConstants : register(b0)
{
    float2 centerUV;        // Brush center in UV space (0-1)
    float2 sigmaUV;         // Brush size in UV space  
    float strength;         // Brush strength (0-1)
    float brushType;        // 0=Paint, 1=Quantize, 2=Erase, 3=Comb
    float2 padding;
}

struct VertexInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// Vertex shader - brush quad
VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    output.position = float4(input.position.xy, 0.0, 1.0);
    output.uv = input.uv;
    return output;
}

// Gaussian brush kernel
float gaussianKernel(float2 uv, float2 center, float2 sigma)
{
    float2 delta = (uv - center) / sigma;
    return exp(-0.5 * dot(delta, delta));
}

// Quantize brush - snaps to harmonic frequencies  
float quantizeBrush(float2 uv, float2 center, float2 sigma)
{
    float gaussian = gaussianKernel(uv, center, sigma);
    
    // Snap to nearest harmonic (12TET for now)
    float freq = uv.y; // Frequency axis
    float octave = floor(freq * 12.0) / 12.0;
    float snap = 1.0 - abs(freq - octave) * 24.0; // Sharp harmonic peaks
    
    return gaussian * saturate(snap);
}

// Comb filter brush - creates harmonic series
float combBrush(float2 uv, float2 center, float2 sigma)
{
    float gaussian = gaussianKernel(uv, center, sigma);
    
    // Harmonic series based on fundamental
    float fundamental = center.y;
    float freq = uv.y;
    float harmonic = fmod(freq / fundamental, 1.0);
    float comb = 1.0 - 4.0 * harmonic * (1.0 - harmonic); // Triangular peaks
    
    return gaussian * comb;
}

// Pixel shader - brush rendering
float4 PSMain(VertexOutput input) : SV_Target
{
    float2 uv = input.uv;
    float brushValue = 0.0;
    
    if (brushType < 0.5) // Paint brush
    {
        brushValue = gaussianKernel(uv, centerUV, sigmaUV);
    }
    else if (brushType < 1.5) // Quantize brush  
    {
        brushValue = quantizeBrush(uv, centerUV, sigmaUV);
    }
    else if (brushType < 2.5) // Erase brush (negative)
    {
        brushValue = -gaussianKernel(uv, centerUV, sigmaUV);
    }
    else // Comb brush
    {
        brushValue = combBrush(uv, centerUV, sigmaUV);
    }
    
    // Apply strength and return as R16F
    return float4(brushValue * strength, 0, 0, 1);
}