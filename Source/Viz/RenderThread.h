#pragma once

#ifdef _WIN32
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <string>

using Microsoft::WRL::ComPtr;

/**
 * RenderThread - Dedicated D3D11 immediate context owner
 * 
 * Implements Pattern #6: Renderer-owns-D3D11 Immediate Context
 * 
 * Key principles:
 * - Only ONE thread owns the D3D11 immediate context and calls Present()
 * - UI thread posts render commands via lock-free queue  
 * - GPU thread processes commands and presents frames
 * - Handles device-lost scenarios gracefully with WARP fallback
 * - Uses dynamic buffers with MAP_WRITE_DISCARD for per-frame updates
 */
class RenderThread
{
public:
    // Render command types for UI → GPU communication
    enum class CommandType : uint8_t
    {
        SetClearColor,
        UpdateSpectralTexture,
        UpdateMaskAtlas,
        SetViewport,
        RenderSpectralFrame,
        RenderParticles,
        Present,
        Resize,
        Shutdown
    };
    
    // POD render command (lock-free queue compatible)
    struct RenderCommand
    {
        CommandType type;
        uint32_t version = 1;
        uint64_t frameId = 0;
        
        union Data
        {
            struct { float r, g, b, a; } clearColor;
            struct { const float* data; int width, height; } texture;
            struct { int width, height; } viewport;
            struct { int width, height; } resize;
        } data;
        
        RenderCommand(CommandType t = CommandType::Present) : type(t) { data = {}; }
    };
    
    // GPU capabilities and status
    struct DeviceInfo
    {
        std::string deviceName;
        bool supportsCompute = false;
        bool supportsFloat32 = false;
        bool supportsTearing = false;
        int maxTextureSize = 0;
        size_t videoMemoryMB = 0;
        bool usingWARP = false;
        bool deviceLost = false;
    };
    
    RenderThread() noexcept;
    ~RenderThread();
    
    // Non-copyable
    RenderThread(const RenderThread&) = delete;
    RenderThread& operator=(const RenderThread&) = delete;
    
    // Initialization and lifecycle
    bool initialize(HWND windowHandle, int width, int height);
    void shutdown();
    bool isInitialized() const noexcept { return initialized_.load(std::memory_order_acquire); }
    
    // Command submission (called from UI thread)
    bool submitCommand(const RenderCommand& cmd) noexcept;
    template<typename... Args>
    bool submitCommand(CommandType type, Args&&... args) noexcept;
    
    // Immediate operations (called from UI thread)
    bool resize(int width, int height);
    bool present();
    
    // Device information
    DeviceInfo getDeviceInfo() const;
    std::string getLastError() const;
    
    // Performance metrics
    float getGPUFrameTime() const noexcept { return lastFrameTime_.load(std::memory_order_relaxed); }
    int getQueueDepth() const noexcept;
    uint64_t getFramesRendered() const noexcept { return framesRendered_.load(std::memory_order_relaxed); }
    
private:
    // Thread management
    std::thread renderThread_;
    std::atomic<bool> initialized_{false};
    std::atomic<bool> shouldShutdown_{false};
    std::atomic<bool> threadRunning_{false};
    
    // D3D11 core objects (owned by render thread)
    ComPtr<ID3D11Device> device_;
    ComPtr<ID3D11DeviceContext> immediateContext_;
    ComPtr<IDXGISwapChain1> swapChain_;
    ComPtr<ID3D11RenderTargetView> renderTargetView_;
    ComPtr<ID3D11DepthStencilView> depthStencilView_;
    
    // Dynamic resources for per-frame updates
    ComPtr<ID3D11Buffer> dynamicVertexBuffer_;
    ComPtr<ID3D11Buffer> dynamicConstantBuffer_;
    ComPtr<ID3D11Texture2D> dynamicSpectralTexture_;
    ComPtr<ID3D11ShaderResourceView> spectralTextureSRV_;
    
    // MaskAtlas triple-buffering
    static constexpr int MASK_ATLAS_COUNT = 3;
    ComPtr<ID3D11Texture2D> maskAtlasTextures_[MASK_ATLAS_COUNT];
    ComPtr<ID3D11RenderTargetView> maskAtlasRTVs_[MASK_ATLAS_COUNT];
    ComPtr<ID3D11ShaderResourceView> maskAtlasSRVs_[MASK_ATLAS_COUNT];
    int currentMaskAtlasIndex_ = 0;
    
    // Command queue (lock-free SPSC)
    static constexpr int COMMAND_QUEUE_SIZE = 256;
    using CommandQueue = SpscRing<RenderCommand, COMMAND_QUEUE_SIZE>;
    std::unique_ptr<CommandQueue> commandQueue_;
    
    // Device state
    HWND windowHandle_ = nullptr;
    int currentWidth_ = 0;
    int currentHeight_ = 0;
    std::atomic<bool> deviceLost_{false};
    std::atomic<bool> usingWARP_{false};
    
    // Performance tracking
    std::atomic<float> lastFrameTime_{0.0f};
    std::atomic<uint64_t> framesRendered_{0};
    
    // Error handling
    mutable std::mutex errorMutex_;
    mutable std::string lastError_;
    
    // Thread entry point
    void renderThreadMain();
    
    // D3D11 setup and management
    bool createDevice();
    bool createSwapChain(HWND hwnd, int width, int height);
    bool createRenderTargets();
    bool createDynamicResources();
    bool recreateDeviceIfLost();
    bool fallbackToWARP();
    void releaseD3DResources();
    
    // Resource creation helpers
    bool createSpectralTexture(int width, int height);
    bool createMaskAtlas(int width, int height);
    bool createDynamicBuffers();
    
    // Command processing
    void processCommands();
    void executeCommand(const RenderCommand& cmd);
    void executeClearColor(const RenderCommand::Data& data);
    void executeUpdateTexture(const RenderCommand::Data& data);
    void executeResize(const RenderCommand::Data& data);
    void executePresent();
    
    // Rendering operations (all called from render thread)
    void renderSpectralVisualization();
    void renderParticleSystem();
    void clearRenderTarget(float r, float g, float b, float a);
    
    // Error handling
    void setLastError(const std::string& error) const;
    HRESULT checkDeviceStatus();
    
    // Performance measurement
    class ScopedGPUTimer
    {
    public:
        ScopedGPUTimer(std::atomic<float>& frameTime) noexcept
            : frameTime_(frameTime)
        {
            startTime_ = std::chrono::high_resolution_clock::now();
        }
        
        ~ScopedGPUTimer() noexcept
        {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime_);
            frameTime_.store(duration.count() / 1000.0f, std::memory_order_relaxed);
        }
        
    private:
        std::atomic<float>& frameTime_;
        std::chrono::high_resolution_clock::time_point startTime_;
    };
    
    // D3D11 debug helpers
    #ifdef _DEBUG
    void enableD3DDebugLayer();
    void reportLiveObjects();
    #endif
};

// Template implementation for convenience command submission
template<typename... Args>
bool RenderThread::submitCommand(CommandType type, Args&&... args) noexcept
{
    RenderCommand cmd(type);
    // This would need specific implementations for each command type
    // Omitted for brevity, but would handle packing args into cmd.data
    return submitCommand(cmd);
}

/**
 * RAII helper for D3D11 resource mapping
 * Ensures resources are properly unmapped even if exceptions occur
 */
class D3D11MappedResource
{
public:
    D3D11MappedResource(ID3D11DeviceContext* context, ID3D11Resource* resource, 
                       UINT subresource, D3D11_MAP mapType, UINT mapFlags = 0)
        : context_(context), resource_(resource), subresource_(subresource)
    {
        HRESULT hr = context_->Map(resource, subresource, mapType, mapFlags, &mappedResource_);
        success_ = SUCCEEDED(hr);
    }
    
    ~D3D11MappedResource()
    {
        if (success_ && context_ && resource_)
        {
            context_->Unmap(resource_, subresource_);
        }
    }
    
    bool isValid() const noexcept { return success_; }
    void* getData() const noexcept { return success_ ? mappedResource_.pData : nullptr; }
    UINT getRowPitch() const noexcept { return success_ ? mappedResource_.RowPitch : 0; }
    UINT getDepthPitch() const noexcept { return success_ ? mappedResource_.DepthPitch : 0; }
    
    // Non-copyable but moveable
    D3D11MappedResource(const D3D11MappedResource&) = delete;
    D3D11MappedResource& operator=(const D3D11MappedResource&) = delete;
    D3D11MappedResource(D3D11MappedResource&&) = default;
    D3D11MappedResource& operator=(D3D11MappedResource&&) = default;
    
private:
    ID3D11DeviceContext* context_;
    ID3D11Resource* resource_;
    UINT subresource_;
    D3D11_MAPPED_SUBRESOURCE mappedResource_{};
    bool success_ = false;
};

#endif // _WIN32