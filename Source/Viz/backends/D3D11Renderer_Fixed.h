#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../GpuRenderer.h"

#ifdef _WIN32
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <string>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <algorithm>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class D3D11Renderer : public GpuRenderer
{
public:
    D3D11Renderer();
    ~D3D11Renderer() override;
    
    // GpuRenderer interface implementation
    bool initialize(void* nativeWindowHandle, int width, int height) override;
    void shutdown() override;
    bool isInitialized() const override { return initialized_; }
    
    void beginFrame() override;
    void endFrame() override;
    void present() override;
    
    bool createSpectralTexture(int width, int height) override;
    void updateSpectralTexture(const SpectralFrame& frame) override;
    void resizeBuffers(int width, int height) override;
    
    bool loadShaders() override;
    void reloadShaders() override;
    
    void renderSpectralVisualization() override;
    void renderParticleSystem() override;
    void renderGestureTrails() override;
    
    void setColormap(int colormapType) override;
    void setParticleCount(int count) override;
    void setAnimationSpeed(float speed) override;
    
    float getFrameTime() const override { return lastFrameTime_; }
    int getDrawCalls() const override { return drawCalls_; }
    size_t getGpuMemoryUsage() const override { return gpuMemoryUsage_; }
    
    bool supportsComputeShaders() const override { return supportsCompute_; }
    int getMaxTextureSize() const override { return maxTextureSize_; }
    bool supportsFloatTextures() const override { return supportsFloatTextures_; }
    
    bool checkDeviceStatus() override;
    void handleDeviceLost() override;
    std::string getLastError() const override { return lastError_; }

private:
    // D3D11 Core objects
    Microsoft::WRL::ComPtr<ID3D11Device> device_;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> context_;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv_;
    
    // Spectral visualization resources
    Microsoft::WRL::ComPtr<ID3D11Texture2D> spectralTexture_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spectralSRV_;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> linearSampler_;
    
    // Shader resources
    Microsoft::WRL::ComPtr<ID3D11VertexShader> fullscreenVS_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> spectralPS_;
    
    // State and capabilities
    bool initialized_ = false;
    bool supportsCompute_ = false;
    bool supportsFloatTextures_ = false;
    int maxTextureSize_ = 1024;
    
    int windowWidth_ = 800;
    int windowHeight_ = 600;
    int spectralWidth_ = 512;
    int spectralHeight_ = 256;
    
    HWND hwnd_ = nullptr;
    
    // Settings
    int currentColormap_ = 0; 
    int particleCount_ = 1000;
    float animationSpeed_ = 1.0f;
    
    // Private methods
    void createDeviceAndSwapChain();
    void createRenderTarget();
    void destroyRenderTarget();
    void checkCapabilities();
}; 

#endif // _WIN32