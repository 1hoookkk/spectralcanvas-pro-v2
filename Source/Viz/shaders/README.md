# SpectralCanvas Pro v2 - HLSL Shader System

This directory contains the complete HLSL shader foundation for SpectralCanvas Pro v2's revolutionary "living nebula" visualization system. The shaders are designed for high-performance real-time spectral visualization with 60fps sustained performance for 1000+ particles.

## Architecture Overview

The shader system implements a multi-stage GPU pipeline that transforms spectral audio data into stunning visual representations:

```
Spectral Data (512x256) → Compute Shader (Color Mapping) → Particle System → Final Composition
```

## Core Shader Files

### 1. `SpectralVisualization.hlsl`
**Compute shader for spectral-to-color conversion**
- Processes 512x256 spectral texture input (magnitude/phase)
- Implements Viridis, Magma, and custom spectral colormaps
- Frequency mapping: Deep purple (bass) → Bright cyan (highs)  
- Amplitude mapping: Loud = brilliant flash, Quiet = dim glow
- Timbre analysis: Pure tones = soft auroras, Complex sounds = sharp particles
- Real-time spectral complexity computation for texture effects

### 2. `ParticleSystem.hlsl`
**GPU instanced particle rendering system**
- Vertex shader: Processes particle instances with spectral data binding
- Geometry shader: Generates billboard quads for efficient rendering
- Pixel shader: Procedural particle shapes based on spectral complexity
- Supports 1000+ particles at 60fps with spectral-driven animation
- Turbulence and physics simulation for organic movement
- Optional compute shader for advanced particle physics

### 3. `FullscreenQuad.hlsl`
**Fullscreen rendering and post-processing**
- Efficient fullscreen triangle generation (no vertex buffer)
- Multiple pixel shader variants for different display modes
- Color space conversions and adjustments
- Distortion effects: barrel distortion, chromatic aberration, vignette
- Blending modes: additive, multiply, screen, overlay
- Direct spectrogram visualization mode

## Utility Files

### 4. `ShaderCommon.hlsl`
**Shared utilities and optimizations**
- Fast mathematical approximations (sin, cos, atan2, exp, log)
- Spectral processing utilities (windowing, dB conversion)
- Color space conversion functions
- Noise and turbulence generators for organic effects
- Distance field functions for procedural shapes
- Performance optimization macros and debugging tools

### 5. `ShaderConfig.hlsl`
**Resource binding configuration**
- Centralized documentation of all binding slots
- Constant buffer structure definitions
- Feature flags for quality/performance scaling
- Integration guidelines for D3D11Renderer
- Performance optimization recommendations

## Visual Design Principles

### Frequency-to-Color Mapping
- **Bass (20-200 Hz)**: Deep purple/violet tones
- **Midrange (200-2000 Hz)**: Blue to green transition  
- **Presence (2-8 kHz)**: Green to yellow
- **Brilliance (8+ kHz)**: Yellow to bright cyan

### Amplitude-to-Brightness
- **Loud signals**: Brilliant flashes with high saturation
- **Quiet signals**: Dim glows with subtle presence
- **Dynamic range**: -120dB to 0dB mapped to visual intensity

### Timbre-to-Texture
- **Pure tones**: Soft, aurora-like glows with smooth gradients
- **Complex sounds**: Sharp, particle-like effects with defined edges
- **Harmonic content**: Analyzed via spectral complexity algorithm

## Performance Characteristics

### Target Performance
- **Framerate**: 60fps sustained with complex particle effects
- **Particle Count**: 1000+ particles with full spectral interaction
- **Latency**: <16ms visual response to audio changes
- **Memory**: <64MB VRAM for all textures and buffers

### Optimization Features
- Conservative GPU feature usage for broad compatibility
- Efficient memory access patterns for optimal cache utilization  
- Thread group sizes optimized for modern GPU architectures
- Quality scaling with feature flags for adaptive performance

## Resource Binding Slots

### Textures (Shader Resource Views)
- `t0`: Primary spectral texture (R32G32_FLOAT, 512x256)
- `t1`: Secondary texture for blending effects
- `t2`: Particle state structured buffer
- `t3`: Font/UI texture atlas
- `t4`: Noise texture for organic effects

### Constant Buffers
- `b0`: Primary parameters (varies by shader)
- `b1`: Camera and view matrices
- `b2`: Time and animation parameters
- `b3`: Performance and quality settings

### Unordered Access Views (Compute Only)
- `u0`: Output color texture (R8G8B8A8_UNORM)
- `u1`: Particle update buffer
- `u2`: Debug output buffer

## Integration with D3D11Renderer

### Initialization
1. Create all constant buffers with appropriate usage flags
2. Load and compile shader variants based on feature support
3. Create texture resources with optimal formats
4. Set up sampler states for different filtering modes

### Runtime Usage
1. Update spectral texture each frame from audio thread data
2. Update constant buffers with current parameters
3. Dispatch compute shader for spectral processing
4. Render particles with instanced drawing
5. Composite final result with fullscreen pass

### Error Handling
- Graceful fallback for unsupported features
- Validate all resource operations in debug builds
- Comprehensive logging with performance metrics

## Quality Levels

### Low Quality (Mobile/Integrated GPUs)
- Reduced particle count (256 particles)
- Simplified colormap (linear interpolation)
- Basic particle shapes (circles only)
- No post-processing effects

### Medium Quality (Entry Gaming GPUs)
- Standard particle count (512 particles)  
- Full colormap implementation
- Basic particle shapes with complexity variation
- Essential post-processing only

### High Quality (Mid-range Gaming GPUs)
- High particle count (1024 particles)
- Advanced particle physics simulation
- Full procedural shapes and effects
- Standard post-processing pipeline

### Ultra Quality (High-end Gaming GPUs)
- Maximum particle count (2048 particles)
- Compute shader particle physics
- Advanced procedural effects
- Full post-processing with all distortions

## Building and Compilation

### HLSL Compiler Settings
```
fxc.exe /T cs_5_0 /E CSMain /O3 /Zi SpectralVisualization.hlsl
fxc.exe /T vs_5_0 /E VSMain /T gs_5_0 /E GSMain /T ps_5_0 /E PSMain ParticleSystem.hlsl
fxc.exe /T vs_5_0 /E VSMain /T ps_5_0 /E PSMain FullscreenQuad.hlsl
```

### Feature Compilation
Use preprocessor definitions for quality variants:
```
/D QUALITY_LEVEL=2 /D FEATURE_ADVANCED_PARTICLES=1
```

## Debugging and Profiling

### Debug Features
- Instruction counting in debug builds
- Visual debugging outputs (heat maps, wireframes)
- Performance counters for GPU timing
- Memory usage tracking for resources

### Common Issues
- **Black screen**: Check resource binding and constant buffer updates
- **Poor performance**: Verify thread group sizes and memory access patterns
- **Visual artifacts**: Validate texture formats and sampling modes
- **Particle flickering**: Check particle state synchronization

## Future Enhancements

### Planned Features
- Temporal anti-aliasing for smoother motion
- HDR rendering pipeline for expanded color range
- Volumetric particle effects for 3D depth
- Machine learning-based spectral feature extraction

### Performance Targets
- Support for 4K displays (3840x2160) at 60fps
- 8K particle counts for ultra-high-end systems
- Real-time ray-traced particle interactions
- Multi-GPU rendering for extreme performance

## Contributing

When modifying shaders, ensure:
1. All changes maintain 60fps performance target
2. Resource binding slots remain consistent  
3. Constant buffer layouts match C++ structures exactly
4. Quality scaling is properly implemented
5. Debug information is preserved for troubleshooting

For questions or support, refer to the main project documentation and CLAUDE.md standards.