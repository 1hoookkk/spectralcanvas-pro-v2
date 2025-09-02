// Example of pushing mask events from UI thread
#include "MessageBus.h"
#include "MaskColumn.h" // use your real MaskColumn definition (avoid re-defining)

struct CanvasComponent {
    MaskColumnQueue& maskQueue;

    void mapColumnToBins(/* UI intensity data */, int& binStart, int& binEnd, float& intensity);

    void emitMaskColumn(int x /*, span<const float> columnIntensity */) {
        MaskColumn col{};
        col.x = x;
        mapColumnToBins(/*columnIntensity*/{}, col.binStart, col.binEnd, col.intensity);
        maskQueue.push(col);
    }
};