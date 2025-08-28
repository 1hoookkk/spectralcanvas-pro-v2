// ShaderConfig.hlsl
// SpectralCanvas Pro v2 - Shader Resource Binding Configuration
// Centralized resource binding documentation and validation

#ifndef SHADER_CONFIG_HLSL
#define SHADER_CONFIG_HLSL

// =============================================================================
// RESOURCE BINDING SLOTS DOCUMENTATION
// =============================================================================
// This file serves as the authoritative documentation for all GPU resource
// binding slots used throughout the SpectralCanvas Pro v2 shader system.
// All shaders MUST follow these binding conventions for proper integration
// with the D3D11Renderer backend.

// -----------------------------------------------------------------------------
// TEXTURE BINDINGS (Shader Resource Views)
// -----------------------------------------------------------------------------
// Slot t0: Primary spectral texture (R32G32_FLOAT)
//   - R channel: Spectral magnitude (linear, 0.0 to 10.0+)
//   - G channel: Phase information (radians, -π to π)
//   - Size: 512x256 (configurable via constant buffer)
//   - Usage: All visualization shaders, particle system input
#define TEXTURE_SLOT_SPECTRAL       0

// Slot t1: Secondary texture for blending/effects
//   - Format: R8G8B8A8_UNORM (standard RGBA)
//   - Usage: Fullscreen quad blending, particle textures
#define TEXTURE_SLOT_SECONDARY      1

// Slot t2: Particle state buffer (StructuredBuffer<ParticleState>)
//   - Element count: MAX_PARTICLES (2048)
//   - Usage: Particle system vertex shader input
#define TEXTURE_SLOT_PARTICLE_BUFFER 2

// Slot t3: Font/UI texture atlas
//   - Format: R8G8B8A8_UNORM
//   - Usage: Debug text rendering, UI overlays
#define TEXTURE_SLOT_FONT_ATLAS     3

// Slot t4: Noise texture for organic effects
//   - Format: R8_UNORM or R16_UNORM
//   - Size: 256x256 tileable noise
//   - Usage: Particle turbulence, organic motion
#define TEXTURE_SLOT_NOISE          4

// -----------------------------------------------------------------------------
// CONSTANT BUFFER BINDINGS
// -----------------------------------------------------------------------------
// Slot b0: Primary parameters (varies by shader type)
//   - SpectralVisualization: SpectralParams
//   - ParticleSystem: ParticleParams  
//   - FullscreenQuad: DisplayParams
#define CBUFFER_SLOT_PRIMARY        0

// Slot b1: Camera/view matrices
//   - ViewMatrix (4x4 float)
//   - ProjectionMatrix (4x4 float)
//   - ViewProjectionMatrix (4x4 float, derived)
//   - CameraPosition (float3 + padding)
#define CBUFFER_SLOT_CAMERA         1

// Slot b2: Time and animation parameters
//   - CurrentTime (float, in seconds since startup)
//   - DeltaTime (float, frame time in seconds)
//   - FrameCount (uint32, total frames rendered)
//   - Reserved (float, for future use)
#define CBUFFER_SLOT_TIME           2

// Slot b3: Performance and quality settings
//   - ParticleQuality (0=low, 1=medium, 2=high, 3=ultra)
//   - MaxParticles (uint32, runtime particle limit)
//   - MSAALevel (0=off, 2=2x, 4=4x, 8=8x)
//   - FeatureFlags (bitfield for shader features)
#define CBUFFER_SLOT_QUALITY        3

// -----------------------------------------------------------------------------
// UNORDERED ACCESS VIEW BINDINGS (Compute Shaders Only)
// -----------------------------------------------------------------------------
// Slot u0: Output color texture (R8G8B8A8_UNORM)
//   - Usage: SpectralVisualization compute shader output
#define UAV_SLOT_COLOR_OUTPUT       0

// Slot u1: Particle update buffer (RWStructuredBuffer<ParticleState>)
//   - Usage: Particle physics update compute shader
#define UAV_SLOT_PARTICLE_UPDATE    1

// Slot u2: Debug output buffer
//   - Format: RWBuffer<float4> for debugging data
//   - Usage: Performance profiling, shader debugging
#define UAV_SLOT_DEBUG_OUTPUT       2

// -----------------------------------------------------------------------------
// SAMPLER STATE BINDINGS
// -----------------------------------------------------------------------------
// Slot s0: Linear filtering sampler
//   - Filter: D3D11_FILTER_MIN_MAG_MIP_LINEAR
//   - Address: D3D11_TEXTURE_ADDRESS_CLAMP
#define SAMPLER_SLOT_LINEAR         0

// Slot s1: Point filtering sampler  
//   - Filter: D3D11_FILTER_MIN_MAG_MIP_POINT
//   - Address: D3D11_TEXTURE_ADDRESS_CLAMP
#define SAMPLER_SLOT_POINT          1

// Slot s2: Wrap sampler for noise textures
//   - Filter: D3D11_FILTER_MIN_MAG_MIP_LINEAR
//   - Address: D3D11_TEXTURE_ADDRESS_WRAP
#define SAMPLER_SLOT_WRAP           2

// =============================================================================
// CONSTANT BUFFER STRUCTURES
// =============================================================================
// These structures MUST match the C++ side exactly for proper data binding

// Primary constant buffer for spectral visualization
struct SpectralParams
{
    float4 FrequencyRange;      // .xy = min/max freq, .zw = log scale params
    float4 AmplitudeParams;     // .x = threshold, .y = gain, .z = gamma, .w = saturation  
    float4 TimbreSettings;      // .x = complexity threshold, .y = texture strength
    float4 ColorParams;         // .x = colormap type, .y = brightness, .z = contrast, .w = hue shift
    float4 TimeParams;          // .x = current time, .y = decay rate, .z = persistence, .w = phase offset
};

// Camera and view matrices
struct CameraParams
{
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4x4 ViewProjectionMatrix;  // Derived: Projection * View
    float4   CameraPosition;        // .xyz = position, .w = reserved
    float4   CameraDirection;       // .xyz = forward vector, .w = reserved
    float4   ViewportParams;        // .xy = viewport size, .zw = near/far planes
};

// Particle system parameters
struct ParticleParams
{
    float4x4 ViewMatrix;            // Duplicated for convenience
    float4x4 ProjectionMatrix;      // Duplicated for convenience  
    float4   CameraPos;             // .xyz = position, .w = reserved
    float4   ParticleSettings;      // .x = size scale, .y = velocity scale, .z = lifetime, .w = spawn rate
    float4   SpectralMapping;       // .x = freq min, .y = freq max, .z = amp min, .w = amp max
    float4   AnimationParams;       // .x = time, .y = turbulence, .z = gravity, .w = damping
    float4   PhysicsParams;         // .x = dt, .y = wind, .z = attraction, .w = repulsion
};

// Display and post-processing parameters
struct DisplayParams
{
    float4 ScreenParams;        // .xy = screen size, .zw = texel size
    float4 ColorAdjustment;     // .x = brightness, .y = contrast, .z = saturation, .w = gamma
    float4 BlendParams;         // .x = blend mode, .y = blend factor, .z = fade, .w = time
    float4 DistortionParams;    // .x = barrel, .y = chromatic aberration, .z = vignette, .w = noise
};

// Time and animation parameters
struct TimeParams
{
    float CurrentTime;          // Seconds since startup
    float DeltaTime;            // Frame time in seconds
    uint  FrameCount;           // Total frames rendered
    float Reserved;             // Future use
};

// Performance and quality settings
struct QualityParams
{
    uint ParticleQuality;       // 0=low, 1=medium, 2=high, 3=ultra
    uint MaxParticles;          // Runtime particle limit
    uint MSAALevel;             // 0=off, 2=2x, 4=4x, 8=8x
    uint FeatureFlags;          // Bitfield for shader features
};

// =============================================================================
// FEATURE FLAGS (for QualityParams.FeatureFlags)
// =============================================================================
#define FEATURE_FLAG_CHROMATIC_ABERRATION    (1 << 0)
#define FEATURE_FLAG_BARREL_DISTORTION       (1 << 1)
#define FEATURE_FLAG_VIGNETTE                (1 << 2)
#define FEATURE_FLAG_NOISE_OVERLAY           (1 << 3)
#define FEATURE_FLAG_ADVANCED_PARTICLES      (1 << 4)
#define FEATURE_FLAG_TEMPORAL_FILTERING      (1 << 5)
#define FEATURE_FLAG_HDR_PROCESSING          (1 << 6)
#define FEATURE_FLAG_DEBUG_VISUALIZATION     (1 << 7)

// =============================================================================
// PERFORMANCE OPTIMIZATION GUIDELINES
// =============================================================================
// 
// Thread Group Sizes:
// - Compute shaders: Use 16x16 for 2D processing (256 threads per group)
// - Large datasets: Consider 32x32 for better cache utilization
// - Linear processing: Use 64x1x1 or 128x1x1 for 1D data
//
// Memory Access Patterns:
// - Always use linear memory access within thread groups
// - Avoid divergent branching in inner loops
// - Use groupshared memory for data shared within thread groups
//
// Resource Binding:
// - Bind frequently used resources to lower slots (t0-t3, b0-b1)
// - Use structured buffers for complex data types
// - Prefer R32G32_FLOAT over R32G32B32A32_FLOAT when possible
//
// Shader Variants:
// - Use #ifdef directives for optional features
// - Compile multiple variants for different quality levels
// - Avoid runtime branching on constant values

// =============================================================================
// INTEGRATION NOTES FOR D3D11RENDERER
// =============================================================================
//
// 1. Constant Buffer Updates:
//    - Update constant buffers once per frame, not per draw call
//    - Use D3D11_USAGE_DYNAMIC for frequently updated buffers
//    - Batch parameter updates to minimize API calls
//
// 2. Resource State Management:
//    - Transition resources properly between pipeline stages
//    - Use resource barriers for compute shader outputs
//    - Cache resource views to avoid recreation
//
// 3. Performance Monitoring:
//    - Use D3D11 query objects for GPU timing
//    - Monitor VRAM usage and texture streaming
//    - Implement adaptive quality based on performance metrics
//
// 4. Error Handling:
//    - Validate all resource creations in debug builds
//    - Implement graceful fallbacks for feature unavailability
//    - Log shader compilation errors with context information

#endif // SHADER_CONFIG_HLSL