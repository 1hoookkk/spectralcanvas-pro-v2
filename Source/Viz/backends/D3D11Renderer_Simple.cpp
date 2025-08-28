#include "D3D11Renderer.h"

#ifdef _WIN32
#include <fstream>
#include <sstream>

D3D11Renderer::D3D11Renderer() = default;
D3D11Renderer::~D3D11Renderer() { shutdown(); }

bool D3D11Renderer::initialize(void* nativeWindowHandle, int width, int height)
{
    if (initialized_) return true;
    
    HWND hwnd = static_cast<HWND>(nativeWindowHandle);
    if (!hwnd) return false;
    
    windowWidth_ = width;
    windowHeight_ = height;
    
    // Create basic D3D11 device and swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;
    
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &swapChainDesc, (IDXGISwapChain**)&swapChain_,
        &device_, &featureLevel, &deviceContext_
    );
    
    if (FAILED(hr)) return false;
    
    // Create render target view
    ComPtr<ID3D11Texture2D> backBuffer;
    hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr)) return false;
    
    hr = device_->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView_);
    if (FAILED(hr)) return false;
    
    initialized_ = true;
    return true;
}

void D3D11Renderer::shutdown()
{
    if (!initialized_) return;
    renderTargetView_.Reset();
    swapChain_.Reset();
    deviceContext_.Reset();
    device_.Reset();
    initialized_ = false;
}

void D3D11Renderer::beginFrame()
{
    if (!initialized_) return;
    
    float clearColor[4] = { 0.1f, 0.1f, 0.18f, 1.0f };
    deviceContext_->ClearRenderTargetView(renderTargetView_.Get(), clearColor);
    deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), nullptr);
    
    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(windowWidth_);
    viewport.Height = static_cast<float>(windowHeight_);
    viewport.MaxDepth = 1.0f;
    deviceContext_->RSSetViewports(1, &viewport);
}

void D3D11Renderer::endFrame() { }

void D3D11Renderer::present()
{
    if (swapChain_) swapChain_->Present(1, 0);
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
    return SUCCEEDED(hr);
}

void D3D11Renderer::updateSpectralTexture(const SpectralFrame& frame)
{
    if (!spectralTexture_) return;
    
    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = deviceContext_->Map(spectralTexture_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
    if (SUCCEEDED(hr))
    {
        float* data = static_cast<float*>(mapped.pData);
        const size_t pitch = mapped.RowPitch / sizeof(float);
        
        for (int y = 0; y < spectralHeight_; ++y)
        {
            for (int x = 0; x < spectralWidth_ && x < static_cast<int>(SpectralFrame::NUM_BINS); ++x)
            {
                data[y * pitch + x] = frame.magnitude[x];
            }
        }
        deviceContext_->Unmap(spectralTexture_.Get(), 0);
    }
}

void D3D11Renderer::resizeBuffers(int width, int height)
{
    windowWidth_ = width;
    windowHeight_ = height;
    if (swapChain_)
    {
        renderTargetView_.Reset();
        swapChain_->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        
        ComPtr<ID3D11Texture2D> backBuffer;
        swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        device_->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView_);
    }
}

bool D3D11Renderer::loadShaders() { return true; }
void D3D11Renderer::reloadShaders() { }
void D3D11Renderer::renderSpectralVisualization() { }
void D3D11Renderer::renderParticleSystem() { }
void D3D11Renderer::renderGestureTrails() { }
void D3D11Renderer::setColormap(int) { }
void D3D11Renderer::setParticleCount(int) { }
void D3D11Renderer::setAnimationSpeed(float) { }
bool D3D11Renderer::checkDeviceStatus() { return device_ != nullptr; }
void D3D11Renderer::handleDeviceLost() { }

#endif // _WIN32