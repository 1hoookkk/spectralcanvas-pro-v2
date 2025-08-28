// FullscreenQuad.hlsl
// SpectralCanvas Pro v2 - Fullscreen Quad Rendering
// Basic vertex/pixel shaders for spectrogram display and post-processing
//
// Resource Bindings:
// - t0: Primary texture (spectrogram or rendered scene)
// - t1: Secondary texture (for blending effects)
// - b0: Display parameters

// Display parameters
cbuffer DisplayParams : register(b0)
{
    float4 ScreenParams;        // .xy = screen size, .zw = texel size
    float4 ColorAdjustment;     // .x = brightness, .y = contrast, .z = saturation, .w = gamma
    float4 BlendParams;         // .x = blend mode, .y = blend factor, .z = fade, .w = time
    float4 DistortionParams;    // .x = barrel, .y = chromatic aberration, .z = vignette, .w = noise
};

// Vertex shader input (no vertex buffer needed - generate fullscreen triangle)
struct VSInput
{
    uint vertex_id : SV_VertexID;
};

// Vertex shader output
struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

// Pixel shader output
struct PSOutput
{
    float4 color : SV_Target;
};

// Textures and samplers
Texture2D<float4> PrimaryTexture : register(t0);
Texture2D<float4> SecondaryTexture : register(t1);
SamplerState LinearSampler : register(s0);
SamplerState PointSampler : register(s1);

// Vertex Shader - Generate fullscreen triangle (no vertex buffer required)
VSOutput VSMain(VSInput input)
{
    VSOutput output;
    
    // Generate fullscreen triangle using vertex ID
    // This technique uses 3 vertices to cover the entire screen efficiently
    // Vertex 0: (-1, -1), Vertex 1: (3, -1), Vertex 2: (-1, 3)
    float2 positions[3] = {
        float2(-1.0, -1.0),  // Bottom-left
        float2( 3.0, -1.0),  // Bottom-right (extends beyond screen)
        float2(-1.0,  3.0)   // Top-left (extends beyond screen)
    };
    
    float2 texcoords[3] = {
        float2(0.0, 1.0),    // Bottom-left UV
        float2(2.0, 1.0),    // Bottom-right UV (extends beyond [0,1])
        float2(0.0, -1.0)    // Top-left UV (extends beyond [0,1])
    };
    
    output.position = float4(positions[input.vertex_id], 0.0, 1.0);
    output.texcoord = texcoords[input.vertex_id];
    
    return output;
}

// Utility function: Convert RGB to HSV
float3 RGBtoHSV(float3 rgb)
{
    float4 K = float4(0.0, -1.0/3.0, 2.0/3.0, -1.0);
    float4 p = lerp(float4(rgb.bg, K.wz), float4(rgb.gb, K.xy), step(rgb.b, rgb.g));
    float4 q = lerp(float4(p.xyw, rgb.r), float4(rgb.r, p.yzx), step(p.x, rgb.r));
    
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return float3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// Utility function: Convert HSV to RGB
float3 HSVtoRGB(float3 hsv)
{
    float4 K = float4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    float3 p = abs(frac(hsv.xxx + K.xyz) * 6.0 - K.www);
    return hsv.z * lerp(K.xxx, saturate(p - K.xxx), hsv.y);
}

// Apply barrel distortion
float2 ApplyBarrelDistortion(float2 uv, float strength)
{
    float2 centered = uv - 0.5;
    float r2 = dot(centered, centered);
    float distortion = 1.0 + strength * r2;
    return 0.5 + centered * distortion;
}

// Apply chromatic aberration
float3 ApplyChromaticAberration(Texture2D tex, float2 uv, float strength)
{
    float2 offset = (uv - 0.5) * strength;
    
    float r = tex.Sample(LinearSampler, uv - offset).r;
    float g = tex.Sample(LinearSampler, uv).g;
    float b = tex.Sample(LinearSampler, uv + offset).b;
    
    return float3(r, g, b);
}

// Apply vignette effect
float ApplyVignette(float2 uv, float strength)
{
    float2 centered = uv - 0.5;
    float dist = length(centered);
    return 1.0 - smoothstep(0.3, 0.8, dist * strength);
}

// Generate procedural noise
float Noise(float2 uv, float time)
{
    float2 p = uv * 12.9898;
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453 + time);
}

// Main Pixel Shader - Fullscreen processing
PSOutput PSMain(VSOutput input)
{
    PSOutput output;
    float2 uv = input.texcoord;
    
    // Apply barrel distortion if enabled
    if (abs(DistortionParams.x) > 0.001)
    {
        uv = ApplyBarrelDistortion(uv, DistortionParams.x);
        
        // Clamp to avoid sampling outside texture
        if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
        {
            output.color = float4(0, 0, 0, 1);
            return output;
        }
    }
    
    // Sample primary texture with chromatic aberration
    float3 primary_color;
    if (abs(DistortionParams.y) > 0.001)
    {
        primary_color = ApplyChromaticAberration(PrimaryTexture, uv, DistortionParams.y);
    }
    else
    {
        primary_color = PrimaryTexture.Sample(LinearSampler, uv).rgb;
    }
    
    // Sample secondary texture for blending
    float3 secondary_color = SecondaryTexture.Sample(LinearSampler, uv).rgb;
    
    // Apply blending based on blend mode
    float3 final_color = primary_color;
    float blend_factor = BlendParams.y;
    
    if (BlendParams.x < 0.5) // Additive blending
    {
        final_color = primary_color + secondary_color * blend_factor;
    }
    else if (BlendParams.x < 1.5) // Multiply blending
    {
        final_color = lerp(primary_color, primary_color * secondary_color, blend_factor);
    }
    else if (BlendParams.x < 2.5) // Screen blending
    {
        final_color = lerp(primary_color, 1.0 - (1.0 - primary_color) * (1.0 - secondary_color), blend_factor);
    }
    else // Overlay blending
    {
        float3 overlay = (primary_color < 0.5) ? 
            2.0 * primary_color * secondary_color : 
            1.0 - 2.0 * (1.0 - primary_color) * (1.0 - secondary_color);
        final_color = lerp(primary_color, overlay, blend_factor);
    }
    
    // Apply color adjustments
    // Brightness
    final_color *= ColorAdjustment.x;
    
    // Contrast
    final_color = (final_color - 0.5) * ColorAdjustment.y + 0.5;
    
    // Saturation
    if (abs(ColorAdjustment.z - 1.0) > 0.001)
    {
        float3 hsv = RGBtoHSV(final_color);
        hsv.y *= ColorAdjustment.z;
        final_color = HSVtoRGB(hsv);
    }
    
    // Gamma correction
    final_color = pow(abs(final_color), 1.0 / ColorAdjustment.w);
    
    // Apply vignette if enabled
    if (abs(DistortionParams.z) > 0.001)
    {
        float vignette = ApplyVignette(uv, DistortionParams.z);
        final_color *= vignette;
    }
    
    // Add noise if enabled
    if (abs(DistortionParams.w) > 0.001)
    {
        float noise = Noise(uv, BlendParams.w) * DistortionParams.w;
        final_color += (noise - 0.5 * DistortionParams.w);
    }
    
    // Apply fade
    final_color *= BlendParams.z;
    
    // Ensure color is in valid range
    final_color = saturate(final_color);
    
    output.color = float4(final_color, 1.0);
    return output;
}

// Alternative pixel shader for direct spectrogram display
PSOutput PSSpectrogram(VSOutput input)
{
    PSOutput output;
    float2 uv = input.texcoord;
    
    // Sample spectral data as color texture
    float4 spectral_data = PrimaryTexture.Sample(LinearSampler, uv);
    
    // Enhance visualization for spectrogram
    float magnitude = spectral_data.r;
    float phase = spectral_data.g;
    
    // Apply logarithmic scaling for better visibility
    magnitude = log(1.0 + magnitude * 10.0) / log(11.0);
    
    // Create color based on frequency (Y axis) and amplitude
    float frequency_norm = 1.0 - uv.y; // Invert Y for traditional spectrogram layout
    
    // Color mapping: Blue (low) -> Green -> Yellow -> Red (high frequency)
    float3 color;
    if (frequency_norm < 0.25)
    {
        // Blue to Cyan
        color = lerp(float3(0, 0, 1), float3(0, 1, 1), frequency_norm * 4.0);
    }
    else if (frequency_norm < 0.5)
    {
        // Cyan to Green
        color = lerp(float3(0, 1, 1), float3(0, 1, 0), (frequency_norm - 0.25) * 4.0);
    }
    else if (frequency_norm < 0.75)
    {
        // Green to Yellow
        color = lerp(float3(0, 1, 0), float3(1, 1, 0), (frequency_norm - 0.5) * 4.0);
    }
    else
    {
        // Yellow to Red
        color = lerp(float3(1, 1, 0), float3(1, 0, 0), (frequency_norm - 0.75) * 4.0);
    }
    
    // Apply magnitude as brightness
    color *= magnitude;
    
    // Add phase information as subtle hue shift
    color.r += sin(phase) * 0.1;
    color.g += sin(phase + 2.094) * 0.1; // 120 degrees
    color.b += sin(phase + 4.189) * 0.1; // 240 degrees
    
    output.color = float4(saturate(color), 1.0);
    return output;
}

// Utility pixel shader for color space conversion
PSOutput PSColorSpaceConvert(VSOutput input)
{
    PSOutput output;
    float2 uv = input.texcoord;
    
    float4 input_color = PrimaryTexture.Sample(LinearSampler, uv);
    
    // Convert based on ColorAdjustment.x parameter
    if (ColorAdjustment.x < 0.5)
    {
        // RGB to HSV conversion
        float3 hsv = RGBtoHSV(input_color.rgb);
        output.color = float4(hsv, input_color.a);
    }
    else if (ColorAdjustment.x < 1.5)
    {
        // HSV to RGB conversion
        float3 rgb = HSVtoRGB(input_color.rgb);
        output.color = float4(rgb, input_color.a);
    }
    else
    {
        // Linear to sRGB conversion
        float3 srgb = pow(abs(input_color.rgb), 1.0/2.2);
        output.color = float4(srgb, input_color.a);
    }
    
    return output;
}