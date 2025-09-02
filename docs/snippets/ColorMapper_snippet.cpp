// Color → spectral UI->Audio wiring (no RT allocs)
#include "MessageBus.h"
#include "ParameterChange.h"   // use your real ParameterChange type
#include <vector>

// Prefer the real mapper header if present; otherwise provide minimal fwd decls for docs/snippet use.
#if __has_include("ColorToSpectralMapper.h")
  #include "ColorToSpectralMapper.h"
#else
  struct HSB { float h{}, s{}, b{}; };
  struct ColorAnalysis {};
  struct ColorToSpectralMapper {
      static ColorAnalysis analyze(const HSB&);
      static std::vector<ParameterChange> mapToSpectralParams(const ColorAnalysis&);
  };
#endif

struct BrushPanel {
    ParameterQueue& parameterQueue;
    void onColorChanged(const HSB& hsb) {
        // Heavy work on UI thread; RT only gets tiny POD ParameterChange messages.
        auto analysis = ColorToSpectralMapper::analyze(hsb);
        auto params   = ColorToSpectralMapper::mapToSpectralParams(analysis);
        for (const auto& p : params)
            parameterQueue.push(p);
    }
};