// ParticleSystem.hlsl
// SpectralCanvas Pro v2 - GPU Instanced Particle System
// Supports 1000+ particles at 60fps with spectral data driven animation
//
// Resource Bindings:
// - t0: Spectral texture (R32G32_FLOAT)
// - t1: Particle state buffer (StructuredBuffer)
// - b0: Particle system parameters
// - b1: Camera/view matrices

#include "SpectralVisualization.hlsl" // Reuse colormap functions

// Particle system constants
#define MAX_PARTICLES 2048
#define PARTICLE_VERTICES_PER_QUAD 4

// Particle system parameters
cbuffer ParticleParams : register(b0)
{
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4 CameraPos;           // .xyz = position, .w = reserved
    float4 ParticleSettings;    // .x = size scale, .y = velocity scale, .z = lifetime, .w = spawn rate
    float4 SpectralMapping;     // .x = freq min, .y = freq max, .z = amp min, .w = amp max
    float4 AnimationParams;     // .x = time, .y = turbulence, .z = gravity, .w = damping
};

// Particle state structure (matches CPU layout)
struct ParticleState
{
    float3 position;
    float life_remaining;
    float3 velocity;
    float size;
    float2 spectral_coord;  // UV coordinate in spectral texture
    float birth_time;
    float complexity;       // Spectral complexity measure
};

// Particle vertex structure
struct ParticleVertex
{
    float3 position : POSITION;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
    float size : TEXCOORD1;
    float complexity : TEXCOORD2;
};

// Geometry shader output structure
struct GSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
    float complexity : TEXCOORD1;
};

// Structured buffer for particle states
StructuredBuffer<ParticleState> ParticleBuffer : register(t1);

// Spectral texture (shared with compute shader)
Texture2D<float2> SpectralTexture : register(t0);
SamplerState SpectralSampler : register(s0);

// Vertex Shader - Process particle instances
ParticleVertex VSMain(uint vertex_id : SV_VertexID, uint instance_id : SV_InstanceID)
{
    ParticleVertex output;
    
    // Early exit for invalid particles
    if (instance_id >= MAX_PARTICLES)
    {
        output.position = float3(0, 0, 0);
        output.color = float4(0, 0, 0, 0);
        output.texcoord = float2(0, 0);
        output.size = 0;
        output.complexity = 0;
        return output;
    }
    
    // Load particle state
    ParticleState particle = ParticleBuffer[instance_id];
    
    // Skip dead particles
    if (particle.life_remaining <= 0.0)
    {
        output.position = float3(0, 0, 0);
        output.color = float4(0, 0, 0, 0);
        output.texcoord = float2(0, 0);
        output.size = 0;
        output.complexity = 0;
        return output;
    }
    
    // Sample spectral data at particle's frequency
    float2 spectral_data = SpectralTexture.SampleLevel(SpectralSampler, particle.spectral_coord, 0);
    float magnitude = spectral_data.x;
    float phase = spectral_data.y;
    
    // Calculate particle age and life factor
    float age = AnimationParams.x - particle.birth_time;
    float life_factor = particle.life_remaining / ParticleSettings.z;
    
    // Animate position based on spectral data
    float3 animated_pos = particle.position;
    
    // Add spectral-driven movement
    float spectral_force = magnitude * ParticleSettings.y;
    animated_pos.y += sin(phase + AnimationParams.x * 2.0) * spectral_force * 0.1;
    animated_pos.x += cos(phase * 1.7 + AnimationParams.x * 1.5) * spectral_force * 0.05;
    
    // Add turbulence for organic movement
    float turbulence = AnimationParams.y;
    animated_pos.x += sin(AnimationParams.x * 3.0 + particle.birth_time) * turbulence;
    animated_pos.z += cos(AnimationParams.x * 2.3 + particle.birth_time * 1.7) * turbulence;
    
    // Apply gravity and damping
    animated_pos.y -= AnimationParams.z * age * age * 0.5; // Gravity
    
    output.position = animated_pos;
    
    // Calculate particle size based on magnitude and life
    float base_size = particle.size * ParticleSettings.x;
    float spectral_size_boost = 1.0 + magnitude * 2.0;
    float life_size_factor = 1.0 - (1.0 - life_factor) * (1.0 - life_factor); // Ease-out
    output.size = base_size * spectral_size_boost * life_size_factor;
    
    // Determine color based on frequency and complexity
    float frequency_norm = particle.spectral_coord.y;
    
    float3 particle_color;
    if (particle.complexity > 0.3) // Complex sounds
    {
        // Sharp, bright particles using Magma colormap
        particle_color = Magma(magnitude);
    }
    else // Pure tones
    {
        // Soft aurora using spectral colormap
        particle_color = SpectralColormap(frequency_norm, magnitude);
    }
    
    // Apply life-based fading
    float alpha = life_factor * magnitude;
    output.color = float4(particle_color, alpha);
    
    // Store complexity for geometry shader
    output.complexity = particle.complexity;
    
    // Texture coordinates (will be set in geometry shader)
    output.texcoord = float2(0, 0);
    
    return output;
}

// Geometry Shader - Generate billboard quads
[maxvertexcount(4)]
void GSMain(point ParticleVertex input[1], inout TriangleStream<GSOutput> output_stream)
{
    ParticleVertex particle = input[0];
    
    // Skip invisible particles
    if (particle.color.a <= 0.001 || particle.size <= 0.001)
        return;
    
    // Calculate billboard vectors
    float3 world_pos = particle.position;
    float3 to_camera = normalize(CameraPos.xyz - world_pos);
    
    // Generate billboard orientation vectors
    float3 up = float3(0, 1, 0);
    float3 right = normalize(cross(to_camera, up));
    up = cross(right, to_camera);
    
    // Scale by particle size
    right *= particle.size * 0.5;
    up *= particle.size * 0.5;
    
    // Modify shape based on complexity
    if (particle.complexity > 0.5)
    {
        // Sharp particles: Make more angular
        right *= 1.2;
        up *= 0.8;
    }
    else
    {
        // Soft particles: Keep circular
        // No modification needed
    }
    
    // Generate quad vertices
    GSOutput vertices[4];
    
    // Bottom-left
    vertices[0].position = mul(float4(world_pos - right - up, 1.0), mul(ViewMatrix, ProjectionMatrix));
    vertices[0].color = particle.color;
    vertices[0].texcoord = float2(0, 1);
    vertices[0].complexity = particle.complexity;
    
    // Bottom-right
    vertices[1].position = mul(float4(world_pos + right - up, 1.0), mul(ViewMatrix, ProjectionMatrix));
    vertices[1].color = particle.color;
    vertices[1].texcoord = float2(1, 1);
    vertices[1].complexity = particle.complexity;
    
    // Top-left
    vertices[2].position = mul(float4(world_pos - right + up, 1.0), mul(ViewMatrix, ProjectionMatrix));
    vertices[2].color = particle.color;
    vertices[2].texcoord = float2(0, 0);
    vertices[2].complexity = particle.complexity;
    
    // Top-right
    vertices[3].position = mul(float4(world_pos + right + up, 1.0), mul(ViewMatrix, ProjectionMatrix));
    vertices[3].color = particle.color;
    vertices[3].texcoord = float2(1, 0);
    vertices[3].complexity = particle.complexity;
    
    // Output triangle strip
    output_stream.Append(vertices[0]);
    output_stream.Append(vertices[1]);
    output_stream.Append(vertices[2]);
    output_stream.Append(vertices[3]);
    output_stream.RestartStrip();
}

// Pixel Shader - Render particle with procedural shape
float4 PSMain(GSOutput input) : SV_Target
{
    float2 uv = input.texcoord;
    float4 color = input.color;
    
    // Center coordinates (-1 to 1)
    float2 centered_uv = uv * 2.0 - 1.0;
    float distance_from_center = length(centered_uv);
    
    // Generate particle shape based on complexity
    float alpha_mask;
    
    if (input.complexity > 0.5)
    {
        // Sharp particles: Diamond/star shape
        float diamond = abs(centered_uv.x) + abs(centered_uv.y);
        alpha_mask = 1.0 - smoothstep(0.7, 1.0, diamond);
        
        // Add star points for very complex particles
        if (input.complexity > 0.8)
        {
            float angle = atan2(centered_uv.y, centered_uv.x);
            float star_points = abs(sin(angle * 4.0)) * 0.3;
            alpha_mask *= 1.0 + star_points;
        }
    }
    else
    {
        // Soft particles: Gaussian falloff for aurora effect
        alpha_mask = exp(-distance_from_center * distance_from_center * 2.0);
        
        // Add soft pulsing for pure tones
        float pulse = 0.8 + 0.2 * sin(AnimationParams.x * 8.0);
        alpha_mask *= pulse;
    }
    
    // Apply distance falloff
    alpha_mask = saturate(alpha_mask);
    
    // Enhance center brightness for impact
    float center_boost = 1.0 + (1.0 - distance_from_center) * 0.5;
    color.rgb *= center_boost;
    
    // Final alpha combines mask with particle alpha
    color.a *= alpha_mask;
    
    return color;
}

// Alternative compute shader for particle updates (if needed for advanced physics)
cbuffer ParticleUpdateParams : register(b2)
{
    float4 PhysicsParams;       // .x = dt, .y = wind, .z = attraction, .w = repulsion
    float4 SpawnParams;         // .x = spawn rate, .y = spawn radius, .z = initial velocity, .w = reserved
};

RWStructuredBuffer<ParticleState> ParticleUpdateBuffer : register(u1);

[numthreads(64, 1, 1)]
void CSParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    uint particle_id = id.x;
    if (particle_id >= MAX_PARTICLES)
        return;
    
    ParticleState particle = ParticleUpdateBuffer[particle_id];
    
    // Skip dead particles
    if (particle.life_remaining <= 0.0)
        return;
    
    float dt = PhysicsParams.x;
    
    // Update lifetime
    particle.life_remaining -= dt;
    
    // Update physics
    particle.velocity.y -= AnimationParams.z * dt; // Gravity
    particle.velocity *= (1.0 - AnimationParams.w * dt); // Damping
    
    // Add spectral-driven forces
    float2 spectral_data = SpectralTexture.SampleLevel(SpectralSampler, particle.spectral_coord, 0);
    float magnitude = spectral_data.x;
    
    particle.velocity.y += magnitude * PhysicsParams.z * dt; // Spectral lift
    
    // Update position
    particle.position += particle.velocity * dt;
    
    // Write back
    ParticleUpdateBuffer[particle_id] = particle;
}