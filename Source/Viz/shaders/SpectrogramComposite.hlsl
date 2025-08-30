// SpectrogramComposite.hlsl
// Combines spectrogram data with paint mask and applies color LUT for nebula visualization
// Part of the SpectralCanvas Pro v2 paint-to-audio pipeline

Texture2D spectrogram : register(t0);   // R16F spectral magnitude data  
Texture2D mask : register(t1);          // R16F paint mask (0=muted, 1=full)
Texture1D colorLUT : register(t2);      // Purple→Cyan gradient LUT
SamplerState linearSampler : register(s0);

cbuffer SpectralConstants : register(b0)
{
    float time;
    float colormapType;      // 0=Viridis, 1=Magma, 2=Purple_Cyan  
    float brightness;
    float contrast;
    float frequencyScale;
    float amplitudeScale;
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

// Vertex shader - fullscreen quad
VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    output.position = float4(input.position.xy, 0.0, 1.0);
    output.uv = input.uv;
    return output;
}

// Pixel shader - spectral visualization with mask composite
float4 PSMain(VertexOutput input) : SV_Target
{
    float2 uv = input.uv;
    
    // Sample spectral magnitude (dB scale, normalized 0-1)
    float magnitude = spectrogram.Sample(linearSampler, uv).r;
    
    // Sample paint mask (0=muted, 1=full amplitude)
    float maskValue = mask.Sample(linearSampler, uv).r;
    
    // Apply mask to magnitude with subtle floor to avoid complete silence
    float maskedMag = magnitude * (0.02 + 0.98 * maskValue);
    
    // Apply brightness/contrast adjustments
    maskedMag = saturate((maskedMag - 0.5) * contrast + 0.5 + brightness);
    
    // Convert to dB visualization curve (emphasize mid-range, compress peaks)
    float colorIndex = pow(saturate(maskedMag * amplitudeScale), 0.6);
    
    // Sample color LUT (purple→cyan nebula gradient)
    float4 nebulaColor = colorLUT.Sample(linearSampler, colorIndex);
    
    // Add subtle time-based shimmer for "alive" canvas
    float shimmer = 0.95 + 0.05 * sin(time * 2.0 + uv.x * 10.0 + uv.y * 8.0);
    nebulaColor.rgb *= shimmer;
    
    // Alpha based on energy (fades to transparent in quiet regions)
    nebulaColor.a = colorIndex * maskValue;
    
    return nebulaColor;
}

// Alternative compute shader version for GPU-accelerated spectral processing
[numthreads(8, 8, 1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
    // TODO: Implement compute shader version for real-time spectral manipulation
    // Useful for brush effects, quantization, and spectral morphing
}