#include "D3D11Renderer.h"

#ifdef _WIN32
#include <fstream>
#include <sstream>
#include <chrono>

D3D11Renderer::D3D11Renderer() = default;

D3D11Renderer::~D3D11Renderer()
{
    shutdown();
}

bool D3D11Renderer::initialize(void* nativeWindowHandle, int width, int height)
{
    if (initialized_)
        return true;
        
    HWND hwnd = static_cast<HWND>(nativeWindowHandle);
    if (!hwnd)
    {
        lastError_ = "Invalid window handle";
        return false;
    }
    
    windowWidth_ = width;
    windowHeight_ = height;
    
    // Create D3D11 device and context
    if (!createDevice(hwnd))
        return false;
        
    // Create swap chain
    if (!createSwapChain(hwnd, width, height))
        return false;
        
    // Create render targets
    if (!createRenderTargets())
        return false;
        
    // Check device capabilities
    checkCapabilities();
    
    // Create shaders
    if (!createShaders())
        return false;
        
    // Create buffers
    if (!createBuffers())
        return false;
        
    // Create state objects
    if (!createStates())
        return false;
        
    // Create spectral texture
    if (!createSpectralTexture(spectralWidth_, spectralHeight_))
        return false;
        
    // Create MaskAtlas for paint-to-audio system
    if (!createMaskAtlas())
        return false;
    
    initialized_ = true;
    lastError_.clear();
    return true;
}

void D3D11Renderer::shutdown()
{
    if (!initialized_)
        return;
        
    cleanupMaskAtlas();
    cleanupBuffers();
    cleanupShaders(); 
    cleanupRenderTargets();
    
    // Release core objects
    swapChain_.Reset();
    deviceContext_.Reset();
    device_.Reset();
    
    initialized_ = false;
}

bool D3D11Renderer::createDevice(HWND windowHandle)
{
    UINT createDeviceFlags = 0;
    #ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif
    
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };
    
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        nullptr,                    // Use default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,                    // No software module
        createDeviceFlags,
        featureLevels,
        ARRAYSIZE(featureLevels),
        D3D11_SDK_VERSION,
        &device_,
        &featureLevel,
        &deviceContext_
    );
    
    if (FAILED(hr))
    {
        logD3D11Error(hr, "D3D11CreateDevice");
        return false;
    }
    
    return true;
}

bool D3D11Renderer::createSwapChain(HWND windowHandle, int width, int height)
{
    ComPtr<IDXGIDevice> dxgiDevice;
    HRESULT hr = device_.As(&dxgiDevice);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "QueryInterface IDXGIDevice");
        return false;
    }
    
    ComPtr<IDXGIAdapter> dxgiAdapter;
    hr = dxgiDevice->GetAdapter(&dxgiAdapter);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "GetAdapter");
        return false;
    }
    
    ComPtr<IDXGIFactory2> dxgiFactory;
    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "GetParent IDXGIFactory2");
        return false;
    }
    
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.Flags = 0;
    
    hr = dxgiFactory->CreateSwapChainForHwnd(
        device_.Get(),
        windowHandle,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain_
    );
    
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateSwapChainForHwnd");
        return false;
    }
    
    return true;
}

bool D3D11Renderer::createRenderTargets()
{
    // Get back buffer
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "GetBuffer");
        return false;
    }
    
    // Create render target view
    hr = device_->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateRenderTargetView");
        return false;
    }
    
    // Create depth buffer
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = windowWidth_;
    depthDesc.Height = windowHeight_;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    
    ComPtr<ID3D11Texture2D> depthBuffer;
    hr = device_->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateTexture2D (depth)");
        return false;
    }
    
    // Create depth stencil view
    hr = device_->CreateDepthStencilView(depthBuffer.Get(), nullptr, &depthStencilView_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateDepthStencilView");
        return false;
    }
    
    return true;
}

bool D3D11Renderer::createShaders()
{
    // For now, create minimal shaders - full implementation will load from .hlsl files
    
    // Simple fullscreen vertex shader
    const char* fullscreenVS = R"(
        struct VS_OUTPUT 
        {
            float4 position : SV_POSITION;
            float2 texCoord : TEXCOORD0;
        };
        
        VS_OUTPUT main(uint vertexID : SV_VertexID)
        {
            VS_OUTPUT output;
            float2 texCoord = float2((vertexID << 1) & 2, vertexID & 2);
            output.position = float4(texCoord * 2.0 - 1.0, 0.0, 1.0);
            output.texCoord = float2(texCoord.x, 1.0 - texCoord.y);
            return output;
        }
    )";
    
    // Simple spectral visualization pixel shader
    const char* spectralPS = R"(
        Texture2D spectralTexture : register(t0);
        SamplerState linearSampler : register(s0);
        
        cbuffer SpectralConstants : register(b0)
        {
            float time;
            float colormapType;
            float brightness;
            float contrast;
            float frequencyScale;
            float amplitudeScale;
            float2 padding;
        };
        
        float3 viridis(float t)
        {
            float3 c0 = float3(0.2777, 0.0055, 0.3921);
            float3 c1 = float3(0.1059, 0.6192, 0.4672);
            float3 c2 = float3(0.8500, 0.9065, 0.1320);
            
            t = saturate(t);
            float t2 = t * t;
            float t3 = t2 * t;
            
            return c0 + t * (c1 - c0) + t2 * (c2 - c1);
        }
        
        float4 main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD0) : SV_TARGET
        {
            float spectralValue = spectralTexture.Sample(linearSampler, texCoord).r;
            spectralValue = pow(spectralValue * brightness, contrast);
            
            float3 color = viridis(spectralValue);
            
            // Add some animation
            float wave = sin(texCoord.x * 10.0 + time) * 0.1 + 0.9;
            color *= wave;
            
            return float4(color, 1.0);
        }
    )";
    
    // Compile shaders
    ComPtr<ID3DBlob> vsBlob, psBlob;
    
    if (!compileShaderFromFile("", "main", "vs_5_0", &vsBlob))
    {
        // Fallback to inline shader
        HRESULT hr = D3DCompile(
            fullscreenVS, strlen(fullscreenVS), nullptr, nullptr, nullptr,
            "main", "vs_5_0", 0, 0, &vsBlob, nullptr
        );
        if (FAILED(hr))
        {
            lastError_ = "Failed to compile fullscreen vertex shader";
            return false;
        }
    }
    
    HRESULT hr = D3DCompile(
        spectralPS, strlen(spectralPS), nullptr, nullptr, nullptr,
        "main", "ps_5_0", 0, 0, &psBlob, nullptr
    );
    if (FAILED(hr))
    {
        lastError_ = "Failed to compile spectral pixel shader";
        return false;
    }
    
    // Create shader objects
    hr = device_->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &fullscreenVS_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateVertexShader");
        return false;
    }
    
    hr = device_->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &spectralPS_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreatePixelShader");
        return false;
    }
    
    return true;
}

bool D3D11Renderer::createBuffers()
{
    // Create constant buffers
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    cbDesc.ByteWidth = sizeof(SpectralConstants);
    HRESULT hr = device_->CreateBuffer(&cbDesc, nullptr, &spectralConstants_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateBuffer (spectral constants)");
        return false;
    }
    
    cbDesc.ByteWidth = sizeof(ParticleConstants);
    hr = device_->CreateBuffer(&cbDesc, nullptr, &particleConstants_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateBuffer (particle constants)");
        return false;
    }
    
    cbDesc.ByteWidth = sizeof(BrushConstants);
    hr = device_->CreateBuffer(&cbDesc, nullptr, &brushConstants_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateBuffer (brush constants)");
        return false;
    }
    
    return true;
}

bool D3D11Renderer::createStates()
{
    // Create rasterizer state
    D3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_NONE;
    rastDesc.FillMode = D3D11_FILL_SOLID;
    rastDesc.DepthClipEnable = TRUE;
    
    HRESULT hr = device_->CreateRasterizerState(&rastDesc, &rasterizerState_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateRasterizerState");
        return false;
    }
    
    // Create blend state for particles
    D3D11_BLEND_DESC blendDesc = {};
    blendDesc.RenderTarget[0].BlendEnable = TRUE;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
    hr = device_->CreateBlendState(&blendDesc, &blendState_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateBlendState");
        return false;
    }
    
    // Create sampler state
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    
    hr = device_->CreateSamplerState(&sampDesc, &linearSampler_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateSamplerState");
        return false;
    }
    
    return true;
}

void D3D11Renderer::checkCapabilities()
{
    D3D11_FEATURE_DATA_D3D11_OPTIONS options = {};
    HRESULT hr = device_->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, &options, sizeof(options));
    
    if (SUCCEEDED(hr))
    {
        supportsCompute_ = true; // D3D11 always supports compute shaders
    }
    
    // Check float texture support
    UINT formatSupport = 0;
    hr = device_->CheckFormatSupport(DXGI_FORMAT_R32G32B32A32_FLOAT, &formatSupport);
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

bool D3D11Renderer::createSpectralTexture(int width, int height)
{
    spectralWidth_ = width;
    spectralHeight_ = height;
    
    // Create texture for spectral data
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32_FLOAT; // Single channel float for magnitude
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DYNAMIC;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    
    HRESULT hr = device_->CreateTexture2D(&texDesc, nullptr, &spectralTexture_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateTexture2D (spectral)");
        return false;
    }
    
    // Create shader resource view
    hr = device_->CreateShaderResourceView(spectralTexture_.Get(), nullptr, &spectralSRV_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateShaderResourceView (spectral)");
        return false;
    }
    
    return true;
}

bool D3D11Renderer::createMaskAtlas()
{
    // Create MaskAtlas texture (same dimensions as spectral texture)
    // Use R16_FLOAT for mask values 0..1 with good precision and bandwidth
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = spectralWidth_;
    texDesc.Height = spectralHeight_;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R16_FLOAT; // Half-precision float for masks
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    texDesc.CPUAccessFlags = 0; // No direct CPU access for main texture
    
    HRESULT hr = device_->CreateTexture2D(&texDesc, nullptr, &maskAtlas_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateTexture2D (MaskAtlas)");
        return false;
    }
    
    // Create shader resource view for sampling in composite pass
    hr = device_->CreateShaderResourceView(maskAtlas_.Get(), nullptr, &maskAtlasSRV_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateShaderResourceView (MaskAtlas)");
        return false;
    }
    
    // Create render target view for brush rendering
    hr = device_->CreateRenderTargetView(maskAtlas_.Get(), nullptr, &maskAtlasRTV_);
    if (FAILED(hr))
    {
        logD3D11Error(hr, "CreateRenderTargetView (MaskAtlas)");
        return false;
    }
    
    // Create triple-buffered staging textures for column readback
    D3D11_TEXTURE2D_DESC stagingDesc = {};
    stagingDesc.Width = 1; // Single column
    stagingDesc.Height = spectralHeight_;
    stagingDesc.MipLevels = 1;
    stagingDesc.ArraySize = 1;
    stagingDesc.Format = DXGI_FORMAT_R16_FLOAT;
    stagingDesc.SampleDesc.Count = 1;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    
    for (int i = 0; i < MASK_READBACK_COUNT; ++i)
    {
        hr = device_->CreateTexture2D(&stagingDesc, nullptr, &maskStagingTextures_[i]);
        if (FAILED(hr))
        {
            logD3D11Error(hr, "CreateTexture2D (MaskStaging)");
            return false;
        }
    }
    
    // Clear MaskAtlas to default value (1.0 = no masking)
    float clearValue[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    deviceContext_->ClearRenderTargetView(maskAtlasRTV_.Get(), clearValue);
    
    // Setup brush blend states
    setupBrushBlendStates();
    
    return true;
}

void D3D11Renderer::setupBrushBlendStates()
{
    HRESULT hr;
    
    // Additive blend (Paint brush)
    D3D11_BLEND_DESC addBlendDesc = {};
    addBlendDesc.RenderTarget[0].BlendEnable = TRUE;
    addBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    addBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    addBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    addBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
    hr = device_->CreateBlendState(&addBlendDesc, &brushAddBlend_);
    if (FAILED(hr)) logD3D11Error(hr, "CreateBlendState (Add)");
    
    // Reverse subtract blend (Erase brush)
    D3D11_BLEND_DESC subtractBlendDesc = addBlendDesc;
    subtractBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
    
    hr = device_->CreateBlendState(&subtractBlendDesc, &brushSubtractBlend_);
    if (FAILED(hr)) logD3D11Error(hr, "CreateBlendState (Subtract)");
    
    // Alpha blend (Soft brushes)
    D3D11_BLEND_DESC alphaBlendDesc = {};
    alphaBlendDesc.RenderTarget[0].BlendEnable = TRUE;
    alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    
    hr = device_->CreateBlendState(&alphaBlendDesc, &brushAlphaBlend_);
    if (FAILED(hr)) logD3D11Error(hr, "CreateBlendState (Alpha)");
}

void D3D11Renderer::beginFrame()
{
    if (!initialized_)
        return;
        
    drawCalls_ = 0;
    
    // Clear render target
    float clearColor[4] = { 0.1f, 0.1f, 0.18f, 1.0f };
    deviceContext_->ClearRenderTargetView(renderTargetView_.Get(), clearColor);
    deviceContext_->ClearDepthStencilView(depthStencilView_.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    
    // Set render targets
    deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());
    
    // Set viewport
    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(windowWidth_);
    viewport.Height = static_cast<float>(windowHeight_);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    deviceContext_->RSSetViewports(1, &viewport);
    
    // Update constants
    updateConstants();
}

void D3D11Renderer::endFrame()
{
    // Frame complete
}

void D3D11Renderer::present()
{
    if (!initialized_)
        return;
        
    swapChain_->Present(1, 0); // VSync enabled
}

void D3D11Renderer::updateSpectralTexture(const SpectralFrame& frame)
{
    if (!spectralTexture_)
        return;
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = deviceContext_->Map(spectralTexture_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr))
        return;
    
    // Copy spectral magnitude data to texture
    float* textureData = static_cast<float*>(mappedResource.pData);
    const size_t pitch = mappedResource.RowPitch / sizeof(float);
    
    // For now, just copy magnitude data - later we'll add proper 2D mapping
    for (int y = 0; y < spectralHeight_; ++y)
    {
        for (int x = 0; x < spectralWidth_ && x < static_cast<int>(SpectralFrame::NUM_BINS); ++x)
        {
            textureData[y * pitch + x] = frame.magnitude[x];
        }
    }
    
    deviceContext_->Unmap(spectralTexture_.Get(), 0);
}

void D3D11Renderer::updateConstants()
{
    // Update spectral constants
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = deviceContext_->Map(spectralConstants_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        SpectralConstants* constants = static_cast<SpectralConstants*>(mappedResource.pData);
        
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        constants->time = std::chrono::duration<float>(duration).count();
        constants->colormapType = static_cast<float>(currentColormap_);
        constants->brightness = 2.0f;
        constants->contrast = 1.2f;
        constants->frequencyScale = 1.0f;
        constants->amplitudeScale = 1.0f;
        
        deviceContext_->Unmap(spectralConstants_.Get(), 0);
    }
}

void D3D11Renderer::renderSpectralVisualization()
{
    if (!initialized_)
        return;
    
    // Set shaders
    deviceContext_->VSSetShader(fullscreenVS_.Get(), nullptr, 0);
    deviceContext_->PSSetShader(spectralPS_.Get(), nullptr, 0);
    
    // Set resources
    deviceContext_->PSSetShaderResources(0, 1, spectralSRV_.GetAddressOf());
    deviceContext_->PSSetSamplers(0, 1, linearSampler_.GetAddressOf());
    deviceContext_->PSSetConstantBuffers(0, 1, spectralConstants_.GetAddressOf());
    
    // Set states
    deviceContext_->RSSetState(rasterizerState_.Get());
    
    // Draw fullscreen triangle
    deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext_->Draw(3, 0);
    
    drawCalls_++;
}

void D3D11Renderer::renderParticleSystem()
{
    // TODO: Implement particle system rendering
}

void D3D11Renderer::renderGestureTrails()
{
    // TODO: Implement gesture trail rendering
}

bool D3D11Renderer::loadShaders()
{
    return createShaders(); // For now, same as create
}

void D3D11Renderer::reloadShaders()
{
    cleanupShaders();
    createShaders();
}

void D3D11Renderer::resizeBuffers(int width, int height)
{
    windowWidth_ = width;
    windowHeight_ = height;
    
    cleanupRenderTargets();
    swapChain_->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    createRenderTargets();
}

void D3D11Renderer::setColormap(int colormapType)
{
    currentColormap_ = colormapType;
}

void D3D11Renderer::setParticleCount(int count)
{
    particleCount_ = count;
}

void D3D11Renderer::setAnimationSpeed(float speed)
{
    animationSpeed_ = speed;
}

bool D3D11Renderer::checkDeviceStatus()
{
    if (!device_)
        return false;
        
    HRESULT hr = device_->GetDeviceRemovedReason();
    return SUCCEEDED(hr);
}

void D3D11Renderer::handleDeviceLost()
{
    // TODO: Implement device lost recovery
    lastError_ = "Device lost - recovery not implemented";
}

bool D3D11Renderer::compileShaderFromFile(const std::string& filename, const std::string& entryPoint,
                                          const std::string& target, ID3DBlob** outBlob)
{
    // TODO: Load and compile HLSL files from disk
    return false; // Fall back to inline shaders for now
}

void D3D11Renderer::renderBrushStroke(float centerU, float centerV, float sizeU, float sizeV, 
                                     float strength, int brushType)
{
    if (!maskAtlasRTV_ || !initialized_)
        return;
        
    // Set MaskAtlas as render target
    deviceContext_->OMSetRenderTargets(1, maskAtlasRTV_.GetAddressOf(), nullptr);
    
    // Set viewport for MaskAtlas rendering
    D3D11_VIEWPORT viewport = {};
    viewport.Width = static_cast<float>(spectralWidth_);
    viewport.Height = static_cast<float>(spectralHeight_);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    deviceContext_->RSSetViewports(1, &viewport);
    
    // Select blend state based on brush type
    const float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    switch (brushType)
    {
        case 0: // Paint - Additive
            deviceContext_->OMSetBlendState(brushAddBlend_.Get(), blendFactor, 0xffffffff);
            break;
        case 2: // Erase - Subtractive  
            deviceContext_->OMSetBlendState(brushSubtractBlend_.Get(), blendFactor, 0xffffffff);
            break;
        default: // Quantize/Comb - Alpha blend
            deviceContext_->OMSetBlendState(brushAlphaBlend_.Get(), blendFactor, 0xffffffff);
            break;
    }
    
    // Setup brush constants
    BrushConstants brushData = {};
    brushData.centerU = centerU;
    brushData.centerV = centerV;
    brushData.sigmaU = sizeU;
    brushData.sigmaV = sizeV;
    brushData.strength = strength;
    brushData.brushType = static_cast<float>(brushType);
    
    // Render brush quad
    renderBrushQuad(brushData);
    
    // Restore main render target
    deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());
    deviceContext_->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    
    // Restore main viewport
    D3D11_VIEWPORT mainViewport = {};
    mainViewport.Width = static_cast<float>(windowWidth_);
    mainViewport.Height = static_cast<float>(windowHeight_);
    mainViewport.MinDepth = 0.0f;
    mainViewport.MaxDepth = 1.0f;
    deviceContext_->RSSetViewports(1, &mainViewport);
}

void D3D11Renderer::renderBrushQuad(const BrushConstants& brushData)
{
    // Update brush constant buffer
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = deviceContext_->Map(brushConstants_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (SUCCEEDED(hr))
    {
        memcpy(mappedResource.pData, &brushData, sizeof(BrushConstants));
        deviceContext_->Unmap(brushConstants_.Get(), 0);
    }
    
    // Set shaders (TODO: Create brush shaders)
    deviceContext_->VSSetShader(brushVS_.Get(), nullptr, 0);
    deviceContext_->PSSetShader(brushPS_.Get(), nullptr, 0);
    
    // Set constant buffer
    deviceContext_->VSSetConstantBuffers(0, 1, brushConstants_.GetAddressOf());
    deviceContext_->PSSetConstantBuffers(0, 1, brushConstants_.GetAddressOf());
    
    // Render fullscreen quad (will be clipped by brush shader)
    if (fullscreenQuadVB_)
    {
        UINT stride = sizeof(float) * 5; // Position + UV
        UINT offset = 0;
        deviceContext_->IASetVertexBuffers(0, 1, fullscreenQuadVB_.GetAddressOf(), &stride, &offset);
        deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        deviceContext_->Draw(6, 0); // Two triangles for quad
    }
    
    drawCalls_++;
}

bool D3D11Renderer::readbackMaskColumn(int columnIndex, float* outputColumn, int numBins)
{
    if (!maskAtlas_ || columnIndex < 0 || columnIndex >= spectralWidth_)
        return false;
    
    // Copy column from MaskAtlas to staging texture
    D3D11_BOX sourceBox = {};
    sourceBox.left = columnIndex;
    sourceBox.right = columnIndex + 1;
    sourceBox.top = 0;
    sourceBox.bottom = spectralHeight_;
    sourceBox.front = 0;
    sourceBox.back = 1;
    
    auto& stagingTex = maskStagingTextures_[readbackRotation_];
    deviceContext_->CopySubresourceRegion(
        stagingTex.Get(), 0,
        0, 0, 0,
        maskAtlas_.Get(), 0,
        &sourceBox
    );
    
    // Try to map previous staging texture (triple buffered)
    int readIndex = (readbackRotation_ + 1) % MASK_READBACK_COUNT;
    auto& readbackTex = maskStagingTextures_[readIndex];
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = deviceContext_->Map(readbackTex.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
    if (FAILED(hr))
    {
        // Rotation for next frame
        readbackRotation_ = (readbackRotation_ + 1) % MASK_READBACK_COUNT;
        return false; // Data not ready yet
    }
    
    // Convert half-float data to float
    const uint16_t* halfData = static_cast<const uint16_t*>(mappedResource.pData);
    const size_t pitch = mappedResource.RowPitch / sizeof(uint16_t);
    
    for (int bin = 0; bin < numBins && bin < spectralHeight_; ++bin)
    {
        // Convert half-precision float to full float
        uint16_t halfValue = halfData[bin * pitch];
        
        // Simple half->float conversion (could use intrinsics for speed)
        uint32_t sign = (halfValue >> 15) & 0x1;
        uint32_t exponent = (halfValue >> 10) & 0x1f;
        uint32_t mantissa = halfValue & 0x3ff;
        
        if (exponent == 0)
        {
            outputColumn[bin] = sign ? -0.0f : 0.0f; // Zero or -zero
        }
        else if (exponent == 31)
        {
            outputColumn[bin] = sign ? -1.0f : 1.0f; // Inf or -Inf (clamp to ±1)
        }
        else
        {
            // Normal number
            uint32_t floatBits = (sign << 31) | ((exponent + 112) << 23) | (mantissa << 13);
            outputColumn[bin] = *reinterpret_cast<float*>(&floatBits);
        }
        
        // Clamp to valid mask range
        outputColumn[bin] = std::max(0.0f, std::min(1.0f, outputColumn[bin]));
    }
    
    deviceContext_->Unmap(readbackTex.Get(), 0);
    
    // Rotation for next frame
    readbackRotation_ = (readbackRotation_ + 1) % MASK_READBACK_COUNT;
    
    return true;
}

void D3D11Renderer::logD3D11Error(HRESULT hr, const std::string& operation)
{
    std::ostringstream oss;
    oss << operation << " failed with HRESULT: 0x" << std::hex << hr;
    lastError_ = oss.str();
}

void D3D11Renderer::cleanupRenderTargets()
{
    renderTargetView_.Reset();
    depthStencilView_.Reset();
}

void D3D11Renderer::cleanupShaders()
{
    fullscreenVS_.Reset();
    spectralPS_.Reset();
    spectralCS_.Reset();
    particleVS_.Reset();
    particleGS_.Reset();
    particlePS_.Reset();
    brushVS_.Reset();
    brushPS_.Reset();
}

void D3D11Renderer::cleanupBuffers()
{
    spectralConstants_.Reset();
    particleConstants_.Reset();
    brushConstants_.Reset();
    fullscreenQuadVB_.Reset();
    particleVB_.Reset();
    particleInstanceVB_.Reset();
}

void D3D11Renderer::cleanupMaskAtlas()
{
    maskAtlas_.Reset();
    maskAtlasSRV_.Reset();
    maskAtlasRTV_.Reset();
    
    for (int i = 0; i < MASK_READBACK_COUNT; ++i)
    {
        maskStagingTextures_[i].Reset();
    }
    
    brushAddBlend_.Reset();
    brushSubtractBlend_.Reset();
    brushAlphaBlend_.Reset();
}

#endif // _WIN32