# Validate Plugin

Fast one-click plugin validation for CI/CD pipeline.

## Usage
Run this command to validate the plugin build with pluginval at strictness level 8.

## Commands

### Build specific target (not ALL_BUILD)
```bash
cmake --build build --config Debug --target SpectralCanvasPro_VST3 -- /m:8
```

### Run pluginval validation
```bash
build/pluginval.exe --strictness-level 8 --skip-gui-tests --timeout-ms 60000 --output-log build/_pluginval.log build/SpectralCanvasPro_artefacts/Debug/VST3/SpectralCanvasPro.vst3
```

### Check exit code and show results
```bash
if [ $? -eq 0 ]; then
  echo "✅ Plugin validation PASSED"
  tail -n 50 build/_pluginval.log
else
  echo "❌ Plugin validation FAILED"
  tail -n 100 build/_pluginval.log
  exit 1
fi
```

## PowerShell version
```powershell
cmake --build build --config Debug --target SpectralCanvasPro_VST3 -- /m:8
build/pluginval.exe --strictness-level 8 --skip-gui-tests --timeout-ms 60000 --output-log build/_pluginval.log build/SpectralCanvasPro_artefacts/Debug/VST3/SpectralCanvasPro.vst3
if ($LASTEXITCODE -eq 0) {
  Write-Host "✅ Plugin validation PASSED" -ForegroundColor Green
  Get-Content build/_pluginval.log -Tail 50
} else {
  Write-Host "❌ Plugin validation FAILED" -ForegroundColor Red
  Get-Content build/_pluginval.log -Tail 100
  exit 1
}
```

## CI Integration
Add this to your GitHub Actions workflow:
```yaml
- name: Validate Plugin
  run: |
    cmake --build build --config Release --target SpectralCanvasPro_VST3 -- /m:8
    build/pluginval.exe --strictness-level 8 --skip-gui-tests --timeout-ms 60000 build/SpectralCanvasPro_artefacts/Release/VST3/SpectralCanvasPro.vst3
```

## Expected Results
- All tests should pass with no failures
- Plugin loads and processes audio cleanly
- State save/restore works correctly
- Parameter automation is thread-safe
- No memory leaks or RT-safety violations

## Performance Targets
- Plugin load time: < 500ms
- Audio processing: No dropouts across 44.1kHz - 96kHz
- Memory usage: < 256MB
- CPU usage: < 15% at 48kHz/512 samples