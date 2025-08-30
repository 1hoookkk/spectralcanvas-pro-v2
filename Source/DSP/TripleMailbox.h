#pragma once
#include <atomic>
#include <cstdint>
#include <cstring>

// Triple‑buffer "mailbox": writer publishes most‑recent snapshot; reader
// always reads latest without blocking. T must be trivially copyable.
template <typename T>
struct TripleMailbox
{
    TripleMailbox() noexcept : latest_(0), writeIdx_(1) {}

    // Writer side
    T& beginWrite() noexcept { return buffers_[writeIdx_]; }

    void publish() noexcept
    {
        latest_.store(writeIdx_, std::memory_order_release);
        writeIdx_ = (writeIdx_ + 1) % 3;
    }

    // Reader side: copies into 'out' and returns true.
    bool read(T& out) const noexcept
    {
        const uint32_t idx = latest_.load(std::memory_order_acquire);
        out = buffers_[idx];
        return true;
    }

private:
    mutable std::atomic<uint32_t> latest_;
    uint32_t writeIdx_;
    T buffers_[3]{};
};
