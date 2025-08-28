#include "GpuRenderer.h"

#ifdef _WIN32
    #include "backends/D3D11Renderer.h"
#endif

std::unique_ptr<GpuRenderer> GpuRenderer::create()
{
#ifdef _WIN32
    return std::make_unique<D3D11Renderer>();
#else
    // TODO: Add OpenGL renderer for cross-platform support
    return nullptr;
#endif
}