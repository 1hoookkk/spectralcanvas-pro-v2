#pragma once

#include "../Core/MessageBus.h"
#include <memory>
#include <string>

// Abstract GPU renderer interface for cross-platform support
class GpuRenderer
{
public:
    virtual ~GpuRenderer() = default;
    
    // Initialization and shutdown
    virtual bool initialize(void* nativeWindowHandle, int width, int height) = 0;
    virtual void shutdown() = 0;
    virtual bool isInitialized() const = 0;
    
    // Frame rendering
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void present() = 0;
    
    // Resource management
    virtual bool createSpectralTexture(int width, int height) = 0;
    virtual void updateSpectralTexture(const SpectralFrame& frame) = 0;
    virtual void resizeBuffers(int width, int height) = 0;
    
    // Shader management
    virtual bool loadShaders() = 0;
    virtual void reloadShaders() = 0;
    
    // Rendering operations
    virtual void renderSpectralVisualization() = 0;
    virtual void renderParticleSystem() = 0;
    virtual void renderGestureTrails() = 0;
    
    // Settings
    virtual void setColormap(int colormapType) = 0;
    virtual void setParticleCount(int count) = 0;
    virtual void setAnimationSpeed(float speed) = 0;
    
    // Performance metrics
    virtual float getFrameTime() const = 0;
    virtual int getDrawCalls() const = 0;
    virtual size_t getGpuMemoryUsage() const = 0;
    
    // Device capabilities
    virtual bool supportsComputeShaders() const = 0;
    virtual int getMaxTextureSize() const = 0;
    virtual bool supportsFloatTextures() const = 0;
    
    // Error handling
    virtual bool checkDeviceStatus() = 0;
    virtual void handleDeviceLost() = 0;
    virtual std::string getLastError() const = 0;
    
    // Factory method
    static std::unique_ptr<GpuRenderer> create();
    
protected:
    GpuRenderer() = default;
    
    // Performance tracking
    mutable float lastFrameTime_ = 0.0f;
    mutable int drawCalls_ = 0;
    mutable size_t gpuMemoryUsage_ = 0;
    
    // Error state
    mutable std::string lastError_;
};