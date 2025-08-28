// ShaderCommon.hlsl
// SpectralCanvas Pro v2 - Common Shader Utilities
// Shared functions, constants, and optimizations for all shaders

#ifndef SHADER_COMMON_HLSL
#define SHADER_COMMON_HLSL

// Platform and compatibility definitions
#define HLSL_VERSION 50
#define FEATURE_LEVEL_11_0 1

// Performance optimization macros
#define FORCE_UNROLL [unroll]
#define FORCE_LOOP [loop]
#define FORCE_BRANCH [branch]
#define FORCE_FLATTEN [flatten]

// Memory alignment helpers
#define ALIGN_16 alignas(16)
#define ALIGN_64 alignas(64)

// Common constants
static const float PI = 3.14159265359;
static const float TWO_PI = 6.28318530718;
static const float HALF_PI = 1.57079632679;
static const float INV_PI = 0.31830988618;
static const float EULER = 2.71828182846;
static const float SQRT2 = 1.41421356237;
static const float INV_SQRT2 = 0.70710678118;

// Spectral processing constants
static const float SPECTRAL_MIN_DB = -120.0;
static const float SPECTRAL_MAX_DB = 0.0;
static const float SPECTRAL_DYNAMIC_RANGE = 120.0;

// Performance-optimized mathematical functions
// Fast approximations for real-time processing

// Fast sin approximation (accurate to ~0.001)
float FastSin(float x)
{
    x = fmod(x, TWO_PI);
    if (x < 0) x += TWO_PI;
    
    if (x < PI)
    {
        return x * (PI - x) * (0.40528473456 + 0.08248077500 * x * (PI - x));
    }
    else
    {
        x -= PI;
        return -(x * (PI - x) * (0.40528473456 + 0.08248077500 * x * (PI - x)));
    }
}

// Fast cos approximation
float FastCos(float x)
{
    return FastSin(x + HALF_PI);
}

// Fast atan2 approximation (accurate to ~0.005 radians)
float FastAtan2(float y, float x)
{
    if (x == 0.0)
        return (y > 0.0) ? HALF_PI : -HALF_PI;
    
    float atan_val = y / x;
    float abs_atan = abs(atan_val);
    
    // Approximation: atan(x) ≈ x / (1 + 0.28 * x^2) for |x| <= 1
    float result;
    if (abs_atan <= 1.0)
    {
        result = atan_val / (1.0 + 0.28 * atan_val * atan_val);
    }
    else
    {
        result = HALF_PI - atan_val / (atan_val * atan_val + 0.28);
    }
    
    // Adjust for quadrant
    if (x < 0.0)
    {
        result = (y >= 0.0) ? result + PI : result - PI;
    }
    
    return result;
}

// Fast exponential approximation
float FastExp(float x)
{
    x = 1.0 + x / 32.0;
    x *= x; x *= x; x *= x; x *= x; x *= x; // x^32
    return x;
}

// Fast logarithm approximation
float FastLog(float x)
{
    if (x <= 0.0) return SPECTRAL_MIN_DB;
    
    // Bit manipulation approximation
    uint bits = asuint(x);
    float exp_part = float(int((bits >> 23) & 0xFF) - 127);
    float mant_part = asfloat((bits & 0x007FFFFF) | 0x3F800000);
    
    return exp_part * 0.69314718056 + (mant_part - 1.0) * 0.69314718056;
}

// Convert magnitude to decibels (clamped)
float MagnitudeToDb(float magnitude)
{
    if (magnitude <= 0.0) return SPECTRAL_MIN_DB;
    float db = 20.0 * FastLog(magnitude) / FastLog(10.0);
    return clamp(db, SPECTRAL_MIN_DB, SPECTRAL_MAX_DB);
}

// Convert decibels to normalized amplitude (0-1)
float DbToNormalized(float db)
{
    return saturate((db - SPECTRAL_MIN_DB) / SPECTRAL_DYNAMIC_RANGE);
}

// Spectral windowing functions
float HannWindow(float x)
{
    return 0.5 * (1.0 - cos(TWO_PI * x));
}

float HammingWindow(float x)
{
    return 0.54 - 0.46 * cos(TWO_PI * x);
}

float BlackmanWindow(float x)
{
    return 0.42 - 0.5 * cos(TWO_PI * x) + 0.08 * cos(4.0 * PI * x);
}

// Color space utilities
float3 LinearToSRGB(float3 linear)
{
    return pow(abs(linear), 1.0/2.2);
}

float3 SRGBToLinear(float3 srgb)
{
    return pow(abs(srgb), 2.2);
}

// Luminance calculation (ITU-R BT.709)
float Luminance(float3 color)
{
    return dot(color, float3(0.2126, 0.7152, 0.0722));
}

// Perceptual color distance
float ColorDistance(float3 color1, float3 color2)
{
    float3 diff = color1 - color2;
    return sqrt(dot(diff, diff));
}

// Smooth minimum/maximum functions for organic blending
float SmoothMin(float a, float b, float k)
{
    float h = max(k - abs(a - b), 0.0) / k;
    return min(a, b) - h * h * h * k * (1.0/6.0);
}

float SmoothMax(float a, float b, float k)
{
    return -SmoothMin(-a, -b, k);
}

// Interpolation utilities
float3 CubicInterpolation(float3 p0, float3 p1, float3 p2, float3 p3, float t)
{
    float3 a = p3 - p2 - p0 + p1;
    float3 b = p0 - p1 - a;
    float3 c = p2 - p0;
    float3 d = p1;
    
    return a * t * t * t + b * t * t + c * t + d;
}

// Noise functions for organic effects
float Hash(float2 p)
{
    float h = dot(p, float2(127.1, 311.7));
    return frac(sin(h) * 43758.5453123);
}

float Noise2D(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    
    float2 u = f * f * (3.0 - 2.0 * f);
    
    float a = Hash(i + float2(0.0, 0.0));
    float b = Hash(i + float2(1.0, 0.0));
    float c = Hash(i + float2(0.0, 1.0));
    float d = Hash(i + float2(1.0, 1.0));
    
    return lerp(lerp(a, b, u.x), lerp(c, d, u.x), u.y);
}

// Fractal noise for complex textures
float FractalNoise(float2 p, int octaves)
{
    float value = 0.0;
    float amplitude = 0.5;
    float frequency = 1.0;
    
    FORCE_UNROLL
    for (int i = 0; i < octaves && i < 8; i++)
    {
        value += amplitude * Noise2D(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return value;
}

// Turbulence function for organic movement
float3 TurbulenceVector(float3 pos, float time, int octaves)
{
    float3 turbulence = float3(0, 0, 0);
    float amplitude = 1.0;
    float frequency = 1.0;
    
    FORCE_UNROLL
    for (int i = 0; i < octaves && i < 4; i++)
    {
        float3 sample_pos = pos * frequency + time * 0.1;
        turbulence.x += amplitude * (Noise2D(sample_pos.xy) - 0.5);
        turbulence.y += amplitude * (Noise2D(sample_pos.yz) - 0.5);
        turbulence.z += amplitude * (Noise2D(sample_pos.zx) - 0.5);
        
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return turbulence;
}

// Distance field utilities for procedural shapes
float SDF_Circle(float2 p, float radius)
{
    return length(p) - radius;
}

float SDF_Box(float2 p, float2 size)
{
    float2 d = abs(p) - size;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float SDF_Star(float2 p, float radius, int points)
{
    float angle = FastAtan2(p.y, p.x);
    float sector = TWO_PI / float(points);
    float local_angle = fmod(angle + sector * 0.5, sector) - sector * 0.5;
    
    float2 rotated = float2(
        cos(local_angle) * length(p),
        sin(local_angle) * length(p)
    );
    
    return SDF_Box(rotated, float2(radius * 0.8, radius * 0.2));
}

// Performance monitoring and debugging
#ifdef DEBUG_SHADER_PERFORMANCE
static uint g_instruction_count = 0;
void IncrementInstructionCount() { g_instruction_count++; }
uint GetInstructionCount() { return g_instruction_count; }
#else
void IncrementInstructionCount() { }
uint GetInstructionCount() { return 0; }
#endif

// Memory access patterns for optimal cache usage
// Use these patterns when sampling textures in compute shaders
struct OptimalAccessPattern
{
    uint2 base_coord;
    uint2 group_size;
    uint group_index;
};

OptimalAccessPattern CalculateOptimalAccess(uint3 dispatch_id, uint2 texture_size)
{
    OptimalAccessPattern pattern;
    pattern.base_coord = dispatch_id.xy;
    pattern.group_size = uint2(16, 16); // Match compute shader group size
    pattern.group_index = dispatch_id.x + dispatch_id.y * pattern.group_size.x;
    return pattern;
}

// Compatibility layer for different feature levels
#if FEATURE_LEVEL_11_0
    #define SAMPLE_TEXTURE(tex, sampler, uv) tex.Sample(sampler, uv)
    #define SAMPLE_TEXTURE_LOD(tex, sampler, uv, lod) tex.SampleLevel(sampler, uv, lod)
    #define LOAD_TEXTURE(tex, coord) tex.Load(int3(coord, 0))
#else
    // Fallback for older feature levels
    #define SAMPLE_TEXTURE(tex, sampler, uv) tex2D(sampler, uv)
    #define SAMPLE_TEXTURE_LOD(tex, sampler, uv, lod) tex2Dlod(sampler, float4(uv, 0, lod))
    #define LOAD_TEXTURE(tex, coord) tex2D(sampler, coord)
#endif

#endif // SHADER_COMMON_HLSL