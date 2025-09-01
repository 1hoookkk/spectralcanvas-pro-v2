#pragma once
#include "SpscRingBuffer.h"
#include "SampleMessage.h"
#include "MaskColumn.h"
#include "ParameterUpdate.h"
#include "SpectralFrame.h"

// sample messages (UI → Audio)
using SampleQueue     = SpscRingBuffer<SampleMessage, 8>;
// paint columns (UI → Audio)
using MaskColumnQueue = SpscRingBuffer<MaskColumn, 128>;
// color→spectral mapped params (UI → Audio)
using ParameterQueue  = SpscRingBuffer<ParameterUpdate, 128>;
// spectral data (Audio → UI)
using SpectralDataQueue = SpscRingBuffer<SpectralFrame, 16>;