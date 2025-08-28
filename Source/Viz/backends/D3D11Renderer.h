#pragma once

#include "../GpuRenderer.h"

#ifdef _WIN32
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <array>
#include <vector>

using Microsoft::WRL::ComPtr;

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
    ComPtr<ID3D11Device> device_;
    ComPtr<ID3D11DeviceContext> deviceContext_;
    ComPtr<IDXGISwapChain1> swapChain_;
    ComPtr<ID3D11RenderTargetView> renderTargetView_;
    ComPtr<ID3D11DepthStencilView> depthStencilView_;
    
    // Spectral visualization resources
    ComPtr<ID3D11Texture2D> spectralTexture_;
    ComPtr<ID3D11ShaderResourceView> spectralSRV_;
    ComPtr<ID3D11UnorderedAccessView> spectralUAV_;
    
    // MaskAtlas resources for paint-to-audio system
    ComPtr<ID3D11Texture2D> maskAtlas_;
    ComPtr<ID3D11ShaderResourceView> maskAtlasSRV_;
    ComPtr<ID3D11RenderTargetView> maskAtlasRTV_;
    
    // MaskAtlas readback (triple-buffered staging textures)
    static constexpr int MASK_READBACK_COUNT = 3;
    ComPtr<ID3D11Texture2D> maskStagingTextures_[MASK_READBACK_COUNT];
    int currentStagingIndex_ = 0;
    int readbackRotation_ = 0;
    
    // Shader resources
    ComPtr<ID3D11VertexShader> fullscreenVS_;
    ComPtr<ID3D11PixelShader> spectralPS_;
    ComPtr<ID3D11ComputeShader> spectralCS_;
    ComPtr<ID3D11VertexShader> particleVS_;
    ComPtr<ID3D11GeometryShader> particleGS_;
    ComPtr<ID3D11PixelShader> particlePS_;
    
    // Brush rendering shaders
    ComPtr<ID3D11VertexShader> brushVS_;
    ComPtr<ID3D11PixelShader> brushPS_;
    
    // Constant buffers
    struct SpectralConstants
    {
        float time;
        float colormapType;
        float brightness;
        float contrast;
        float frequencyScale;
        float amplitudeScale;
        float padding[2];
    };
    
    struct ParticleConstants
    {
        float deltaTime;
        float particleCount;
        float animationSpeed;
        float intensity;
        float viewMatrix[16];
        float projMatrix[16];
    };
    
    struct BrushConstants
    {
        float centerU, centerV;        // Brush center in UV space (0-1)
        float sigmaU, sigmaV;          // Brush size in UV space
        float strength;                // Brush strength (0-1)
        float brushType;               // 0=Paint, 1=Quantize, 2=Erase, 3=Comb
        float padding[2];
    };
    
    ComPtr<ID3D11Buffer> spectralConstants_;
    ComPtr<ID3D11Buffer> particleConstants_;
    ComPtr<ID3D11Buffer> brushConstants_;
    
    // Vertex buffers
    ComPtr<ID3D11Buffer> fullscreenQuadVB_;
    ComPtr<ID3D11Buffer> particleVB_;
    ComPtr<ID3D11Buffer> particleInstanceVB_;
    
    // State objects
    ComPtr<ID3D11RasterizerState> rasterizerState_;
    ComPtr<ID3D11BlendState> blendState_;
    ComPtr<ID3D11DepthStencilState> depthStencilState_;
    ComPtr<ID3D11SamplerState> linearSampler_;
    
    // Brush blend states for MaskAtlas rendering
    ComPtr<ID3D11BlendState> brushAddBlend_;      // Add (paint)
    ComPtr<ID3D11BlendState> brushSubtractBlend_; // Subtract (erase)
    ComPtr<ID3D11BlendState> brushAlphaBlend_;    // Alpha blend (soft tools)
    
    // Capabilities and state
    bool initialized_ = false;
    bool supportsCompute_ = false;
    bool supportsFloatTextures_ = false;
    int maxTextureSize_ = 1024;
    
    int windowWidth_ = 800;
    int windowHeight_ = 600;
    int spectralWidth_ = 512;
    int spectralHeight_ = 256;
    
    // Settings
    int currentColormap_ = 0; // 0=Viridis, 1=Magma
    int particleCount_ = 1000;
    float animationSpeed_ = 1.0f;
    
    // Performance tracking
    ComPtr<ID3D11Query> frameTimeQuery_;
    mutable UINT64 lastFrameStart_ = 0;
    mutable UINT64 lastFrameEnd_ = 0;
    
    // Private methods
    bool createDevice(HWND windowHandle);
    bool createSwapChain(HWND windowHandle, int width, int height);
    bool createRenderTargets();
    bool createShaders();
    bool createBuffers();
    bool createStates();
    void checkCapabilities();
    void updateConstants();
    void renderFullscreenQuad();
    bool compileShaderFromFile(const std::string& filename, const std::string& entryPoint,
                               const std::string& target, ID3DBlob** outBlob);
    void logD3D11Error(HRESULT hr, const std::string& operation);
    
    // MaskAtlas system methods
    bool createMaskAtlas();
    void renderBrushStroke(float centerU, float centerV, float sizeU, float sizeV, 
                          float strength, int brushType);
    bool readbackMaskColumn(int columnIndex, float* outputColumn, int numBins);
    void setupBrushBlendStates();
    void renderBrushQuad(const BrushConstants& brushData);
    
    // Resource cleanup helpers
    void cleanupRenderTargets();
    void cleanupShaders();
    void cleanupBuffers();
    void cleanupMaskAtlas();
}; 

#endif // _WIN32