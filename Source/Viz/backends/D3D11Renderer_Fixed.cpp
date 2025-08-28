#include "D3D11Renderer_Fixed.h"

#ifdef _WIN32
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

D3D11Renderer::D3D11Renderer() = default;
D3D11Renderer::~D3D11Renderer() { shutdown(); }

bool D3D11Renderer::initialize(void* nativeWindowHandle, int width, int height)
{
    if (initialized_) return true;
    
    hwnd_ = static_cast<HWND>(nativeWindowHandle);
    if (!hwnd_) return false;
    
    windowWidth_ = std::max(1, width);
    windowHeight_ = std::max(1, height);
    
    try 
    {
        createDeviceAndSwapChain();
        createRenderTarget();
        checkCapabilities();
        
        if (!createSpectralTexture(spectralWidth_, spectralHeight_))
            return false;
            
        initialized_ = true;
        return true;
    }
    catch (const std::exception& e)
    {
        lastError_ = e.what();
        return false;
    }
}

void D3D11Renderer::shutdown()
{
    if (!initialized_) return;
    
    destroyRenderTarget();
    spectralSRV_.Reset();
    spectralTexture_.Reset();
    linearSampler_.Reset();
    fullscreenVS_.Reset();
    spectralPS_.Reset();
    swapChain_.Reset();
    context_.Reset();
    device_.Reset();
    
    initialized_ = false;
}

void D3D11Renderer::createDeviceAndSwapChain()
{
    UINT flags = 0;
   #if defined(_DEBUG)
    flags |= D3D11_CREATE_DEVICE_DEBUG;
   #endif

    const D3D_FEATURE_LEVEL req[] = {
        D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
    };
    D3D_FEATURE_LEVEL got{};

    Microsoft::WRL::ComPtr<ID3D11Device> dev;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> ctx;

    if (FAILED(D3D11CreateDevice(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags,
            req, (UINT)std::size(req), D3D11_SDK_VERSION, &dev, &got, &ctx)))
        throw std::runtime_error("D3D11CreateDevice failed");

    device_  = dev;
    context_ = ctx;

    // Create swap chain via factory from adapter (works in plugin hosts)
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice; device_.As(&dxgiDevice);
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;    dxgiDevice->GetAdapter(&adapter);
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;    adapter->GetParent(__uuidof(IDXGIFactory), &factory);

    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferDesc.Width  = (UINT)std::max(1, windowWidth_);
    sd.BufferDesc.Height = (UINT)std::max(1, windowHeight_);
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count  = 1;
    sd.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount       = 2;
    sd.OutputWindow      = hwnd_;
    sd.Windowed          = TRUE;
    sd.SwapEffect        = DXGI_SWAP_EFFECT_DISCARD;  // conservative, widely supported
    sd.Flags             = 0;

    Microsoft::WRL::ComPtr<IDXGISwapChain> sc;
    if (FAILED(factory->CreateSwapChain(device_.Get(), &sd, &sc)))
        throw std::runtime_error("CreateSwapChain failed");

    swapChain_ = sc;
}

void D3D11Renderer::createRenderTarget()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuf;
    if (FAILED(swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                     reinterpret_cast<void**>(backBuf.GetAddressOf()))))
        throw std::runtime_error("GetBuffer failed");

    if (FAILED(device_->CreateRenderTargetView(backBuf.Get(), nullptr, &rtv_)))
        throw std::runtime_error("CreateRenderTargetView failed");

    D3D11_VIEWPORT vp{};
    vp.Width    = (float)windowWidth_;
    vp.Height   = (float)windowHeight_;
    vp.MinDepth = 0.0f; vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f; vp.TopLeftY = 0.0f;
    context_->RSSetViewports(1, &vp);
}

void D3D11Renderer::destroyRenderTarget()
{
    rtv_.Reset();
}

void D3D11Renderer::checkCapabilities()
{
    supportsCompute_ = true; // D3D11 always supports compute shaders
    
    // Check float texture support
    UINT formatSupport = 0;
    HRESULT hr = device_->CheckFormatSupport(DXGI_FORMAT_R32_FLOAT, &formatSupport);
    supportsFloatTextures_ = SUCCEEDED(hr) && (formatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D);
    
    // Get max texture size
    switch (device_->GetFeatureLevel())
    {
        case D3D_FEATURE_LEVEL_11_1:
        case D3D_FEATURE_LEVEL_11_0:
            maxTextureSize_ = 16384;
            break;
        case D3D_FEATURE_LEVEL_10_1:
        case D3D_FEATURE_LEVEL_10_0:
            maxTextureSize_ = 8192;
            break;
        default:
            maxTextureSize_ = 2048;
            break;
    }
}

void D3D11Renderer::resizeBuffers(int width, int height)
{
    windowWidth_ = std::max(1, width);
    windowHeight_ = std::max(1, height);
    if (!swapChain_) return;

    destroyRenderTarget(); // rtv_.Reset() before resize
    HRESULT hr = swapChain_->ResizeBuffers(0, (UINT)windowWidth_, (UINT)windowHeight_, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr)) throw std::runtime_error("ResizeBuffers failed");
    createRenderTarget();
}

void D3D11Renderer::beginFrame()
{
    if (!initialized_) return;
        
    drawCalls_ = 0;
    
    // Clear render target
    float clearColor[4] = { 0.1f, 0.1f, 0.18f, 1.0f };
    context_->ClearRenderTargetView(rtv_.Get(), clearColor);
    context_->OMSetRenderTargets(1, rtv_.GetAddressOf(), nullptr);
}

void D3D11Renderer::endFrame() { }

void D3D11Renderer::present()
{
    if (swapChain_) swapChain_->Present(1, 0); // VSync enabled
}

bool D3D11Renderer::createSpectralTexture(int width, int height)
{
    spectralWidth_ = width;
    spectralHeight_ = height;
    
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    HRESULT hr = device_->CreateTexture2D(&desc, nullptr, &spectralTexture_);
    if (FAILED(hr)) return false;
    
    hr = device_->CreateShaderResourceView(spectralTexture_.Get(), nullptr, &spectralSRV_);
    if (FAILED(hr)) return false;
    
    // Create sampler state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
    hr = device_->CreateSamplerState(&sampDesc, &linearSampler_);
    return SUCCEEDED(hr);
}

void D3D11Renderer::updateSpectralTexture(const SpectralFrame& frame)
{
    if (!spectralTexture_) return;
    
    // Ensure SRV slot is unbound BEFORE updating the resource
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    context_->PSSetShaderResources(0, 1, nullSRV);
    
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = context_->Map(spectralTexture_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (SUCCEEDED(hr))
    {
        float* data = static_cast<float*>(mapped.pData);
        const size_t pitch = mapped.RowPitch / sizeof(float);
        
        // Copy magnitude data to texture (simple 1D to 2D mapping for now)
        for (int y = 0; y < spectralHeight_; ++y)
        {
            for (int x = 0; x < spectralWidth_ && x < static_cast<int>(SpectralFrame::NUM_BINS); ++x)
            {
                // Normalize and clamp magnitude for visualization
                float mag = std::clamp(frame.magnitude[x], 0.0f, 1.0f);
                data[y * pitch + x] = mag;
            }
        }
        context_->Unmap(spectralTexture_.Get(), 0);
    }
}

bool D3D11Renderer::loadShaders() { return true; } // Stub for now
void D3D11Renderer::reloadShaders() { } // Stub for now
void D3D11Renderer::renderSpectralVisualization() { } // Stub for now
void D3D11Renderer::renderParticleSystem() { } // Stub for now  
void D3D11Renderer::renderGestureTrails() { } // Stub for now
void D3D11Renderer::setColormap(int) { } // Stub for now
void D3D11Renderer::setParticleCount(int) { } // Stub for now
void D3D11Renderer::setAnimationSpeed(float) { } // Stub for now

bool D3D11Renderer::checkDeviceStatus() 
{ 
    return device_ && SUCCEEDED(device_->GetDeviceRemovedReason()); 
}

void D3D11Renderer::handleDeviceLost() 
{
    lastError_ = "Device lost - recovery not implemented";
}

#endif // _WIN32