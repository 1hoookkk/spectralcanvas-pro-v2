#pragma once

#include "../DSP/SpectralMask.h"
#include <vector>
#include <memory>

/**
 * A simple undo manager for SpectralMask data.
 * This implementation is NOT thread-safe and should be managed
 * by the audio processor or a dedicated thread.
 * It currently supports a single level of undo.
 */
class UndoManager
{
public:
    UndoManager() = default;
    ~UndoManager() = default;

    // Non-copyable for safety
    UndoManager(const UndoManager&) = delete;
    UndoManager& operator=(const UndoManager&) = delete;

    /**
     * @brief Saves the current state of the mask for a potential undo operation.
     * @param mask The SpectralMask to save.
     */
    void saveState(const SpectralMask& mask)
    {
        if (mask.isReady())
        {
            undoState = std::make_unique<SpectralMask>();
            undoState->init(mask.width(), mask.height());
            std::copy(mask.raw(), mask.raw() + mask.size(), undoState->raw());
        }
        else
        {
            undoState.reset();
        }
    }

    /**
     * @brief Restores the previously saved state to the provided mask.
     * @param mask The SpectralMask to restore the state into.
     * @return true if an undo state was available and restored, false otherwise.
     */
    bool undo(SpectralMask& mask)
    {
        if (undoState && undoState->isReady() && mask.isReady() && undoState->size() == mask.size())
        {
            std::copy(undoState->raw(), undoState->raw() + undoState->size(), mask.raw());
            undoState.reset(); // Clear the undo state after using it
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if an undo operation is possible.
     * @return true if there is a saved state available.
     */
    bool canUndo() const
    {
        return undoState != nullptr;
    }

private:
    std::unique_ptr<SpectralMask> undoState;
};

