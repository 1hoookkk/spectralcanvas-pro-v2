// SpectralVisualization.hlsl
// SpectralCanvas Pro v2 - Spectral-to-Color Conversion Compute Shader
// Converts 512x256 spectral texture input to visual representation
//
// Resource Bindings:
// - t0: Input spectral texture (R32G32_FLOAT, magnitude/phase)
// - u0: Output color texture (R8G8B8A8_UNORM)
// - b0: Spectral parameters constant buffer

// Thread group configuration for optimal GPU utilization
#define THREAD_GROUP_SIZE_X 16
#define THREAD_GROUP_SIZE_Y 16

// Spectral processing parameters
cbuffer SpectralParams : register(b0)
{
    float4 FrequencyRange;      // .xy = min/max freq, .zw = log scale params
    float4 AmplitudeParams;     // .x = threshold, .y = gain, .z = gamma, .w = saturation
    float4 TimbreSettings;      // .x = complexity threshold, .y = texture strength, .zw = reserved
    float4 ColorParams;         // .x = colormap type, .y = brightness, .z = contrast, .w = hue shift
    float4 TimeParams;          // .x = current time, .y = decay rate, .z = persistence, .w = phase offset
};

// Input textures
Texture2D<float2> SpectralTexture : register(t0);  // R=magnitude, G=phase
RWTexture2D<float4> OutputTexture : register(u0);  // RGBA output

// Viridis colormap implementation (perceptually uniform)
float3 Viridis(float t)
{
    t = saturate(t);
    
    // Viridis polynomial coefficients (optimized for GPU)
    const float3 c0 = float3(0.2777273, 0.005407344, 0.3340998);
    const float3 c1 = float3(0.1050930, 1.404613, 1.384590);
    const float3 c2 = float3(-0.3308618, 0.214847, 0.09509516);
    const float3 c3 = float3(-4.634230, -5.799100, -19.33244);
    const float3 c4 = float3(6.228269, 14.17993, 56.69055);
    const float3 c5 = float3(4.776384, -13.74514, -65.35303);
    const float3 c6 = float3(-5.435455, 4.645852, 26.31258);
    
    return c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
}

// Magma colormap implementation (high contrast for particle effects)
float3 Magma(float t)
{
    t = saturate(t);
    
    // Magma polynomial coefficients
    const float3 c0 = float3(-0.002136485, 0.000466216, 0.013866518);
    const float3 c1 = float3(0.2516605, 0.6775232, 0.2622504);
    const float3 c2 = float3(8.353717, -3.577719, 0.3144679);
    const float3 c3 = float3(-27.66873, 14.26473, -13.64921);
    const float3 c4 = float3(52.17613, -27.94360, 50.12426);
    const float3 c5 = float3(-50.76852, 29.04658, -50.76852);
    const float3 c6 = float3(18.65570, -11.48977, 18.65570);
    
    return c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
}

// Custom spectral colormap: Deep purple (bass) → Bright cyan (highs)
float3 SpectralColormap(float frequency_norm, float amplitude)
{
    // Frequency-based hue: Purple (bass) to Cyan (highs)
    float hue = lerp(0.75, 0.5, frequency_norm); // 270° to 180° in normalized hue
    
    // Amplitude affects saturation and brightness
    float saturation = lerp(0.6, 1.0, amplitude);
    float brightness = lerp(0.1, 1.0, amplitude);
    
    // HSV to RGB conversion
    float c = brightness * saturation;
    float x = c * (1.0 - abs(fmod(hue * 6.0, 2.0) - 1.0));
    float m = brightness - c;
    
    float3 rgb;
    if (hue < 0.5)      rgb = float3(c, x, 0);  // Cyan spectrum
    else                rgb = float3(x, 0, c);  // Purple spectrum
    
    return rgb + m;
}

// Compute spectral complexity for timbre analysis
float ComputeSpectralComplexity(float2 uv, float2 texel_size)
{
    float2 spectral_data = SpectralTexture.SampleLevel(0, uv, 0);
    float magnitude = spectral_data.x;
    
    // Sample neighboring frequencies for spectral variation
    float left = SpectralTexture.SampleLevel(0, uv + float2(-texel_size.x, 0), 0).x;
    float right = SpectralTexture.SampleLevel(0, uv + float2(texel_size.x, 0), 0).x;
    float up = SpectralTexture.SampleLevel(0, uv + float2(0, texel_size.y), 0).x;
    float down = SpectralTexture.SampleLevel(0, uv + float2(0, -texel_size.y), 0).x;
    
    // Compute spectral gradient (measure of harmonics/noise)
    float gradient = abs(left - right) + abs(up - down);
    return gradient * 4.0; // Scale for visibility
}

// Main compute shader entry point
[numthreads(THREAD_GROUP_SIZE_X, THREAD_GROUP_SIZE_Y, 1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
    // Get output texture dimensions
    uint2 output_size;
    OutputTexture.GetDimensions(output_size.x, output_size.y);
    
    // Early exit for out-of-bounds threads
    if (id.x >= output_size.x || id.y >= output_size.y)
        return;
    
    // Convert pixel coordinates to UV coordinates
    float2 uv = (float2(id.xy) + 0.5) / float2(output_size);
    
    // Sample spectral data (magnitude and phase)
    float2 spectral_data = SpectralTexture.SampleLevel(0, uv, 0);
    float magnitude = spectral_data.x;
    float phase = spectral_data.y;
    
    // Apply amplitude processing
    magnitude = saturate((magnitude - AmplitudeParams.x) * AmplitudeParams.y);
    magnitude = pow(magnitude, AmplitudeParams.z); // Gamma correction
    
    // Frequency mapping: Y-coordinate represents frequency
    float frequency_norm = uv.y;
    
    // Time-based effects using phase information
    float time_phase = TimeParams.x + phase + TimeParams.w;
    float temporal_modulation = 0.5 + 0.5 * sin(time_phase);
    
    // Compute spectral complexity for timbre effects
    float2 texel_size = 1.0 / float2(output_size);
    float complexity = ComputeSpectralComplexity(uv, texel_size);
    
    // Choose color based on colormap type
    float3 base_color;
    if (ColorParams.x < 0.5)
    {
        // Spectral colormap mode
        base_color = SpectralColormap(frequency_norm, magnitude);
    }
    else if (ColorParams.x < 1.5)
    {
        // Viridis colormap mode
        base_color = Viridis(magnitude);
    }
    else
    {
        // Magma colormap mode
        base_color = Magma(magnitude);
    }
    
    // Apply brightness and contrast adjustments
    base_color *= ColorParams.y; // Brightness
    base_color = saturate((base_color - 0.5) * ColorParams.z + 0.5); // Contrast
    
    // Timbre-based texture effects
    float texture_strength = TimbreSettings.y;
    if (complexity > TimbreSettings.x)
    {
        // Complex sounds: Add sharp, particle-like highlights
        float sharpness = 1.0 + (complexity - TimbreSettings.x) * texture_strength;
        base_color = pow(base_color, 1.0 / sharpness);
    }
    else
    {
        // Pure tones: Soft aurora-like glow
        float softness = 0.8 + 0.2 * (1.0 - complexity / TimbreSettings.x);
        base_color = pow(base_color, softness);
    }
    
    // Apply temporal modulation for living effect
    base_color *= lerp(0.8, 1.2, temporal_modulation * TimeParams.z);
    
    // Final alpha based on magnitude with persistence
    float alpha = saturate(magnitude + TimeParams.z * 0.3);
    
    // Output final color
    OutputTexture[id.xy] = float4(base_color, alpha);
}