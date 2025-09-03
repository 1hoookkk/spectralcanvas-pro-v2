# SpectralCanvas Pro - Stability Test Loop
# Tests for heap corruption fixes with repeated pluginval runs
# Usage: powershell -ExecutionPolicy Bypass -File test-stability.ps1

param(
    [int]$Iterations = 10,
    [int]$StrictnessLevel = 8,
    [string]$PluginPath = "build\SpectralCanvasPro_artefacts\RelWithDebInfo\VST3\SpectralCanvasPro.vst3"
)

Write-Host "SpectralCanvas Pro - Stability Test Loop" -ForegroundColor Cyan
Write-Host "Targeting heap corruption fixes (0xC0000374)" -ForegroundColor Yellow
Write-Host "Plugin: $PluginPath" -ForegroundColor Gray
Write-Host "Iterations: $Iterations, Strictness: $StrictnessLevel" -ForegroundColor Gray
Write-Host ""

# Check if plugin exists
if (!(Test-Path $PluginPath)) {
    Write-Host "ERROR: Plugin not found at $PluginPath" -ForegroundColor Red
    Write-Host "Build the plugin first with: cmake --build build --config RelWithDebInfo" -ForegroundColor Yellow
    exit 1
}

# Check if pluginval is available
$pluginvalPath = Get-Command pluginval.exe -ErrorAction SilentlyContinue
if (!$pluginvalPath) {
    # Try the build directory
    $buildPluginval = "build\pluginval.exe"
    if (Test-Path $buildPluginval) {
        $pluginvalPath = $buildPluginval
        Write-Host "Using pluginval from build directory" -ForegroundColor Green
    } else {
        Write-Host "ERROR: pluginval.exe not found in PATH or build directory" -ForegroundColor Red
        exit 1
    }
}

$successCount = 0
$failureCount = 0
$startTime = Get-Date

Write-Host "Starting stability test loop..." -ForegroundColor Green
Write-Host "Press Ctrl+C to abort" -ForegroundColor Gray
Write-Host ""

for ($i = 1; $i -le $Iterations; $i++) {
    $runStart = Get-Date
    Write-Host "Run $i/$Iterations - " -NoNewline -ForegroundColor White
    
    try {
        # Run pluginval with error capturing
        $output = & $pluginvalPath --strictness-level $StrictnessLevel --skip-gui-tests $PluginPath 2>&1
        $exitCode = $LASTEXITCODE
        
        $runEnd = Get-Date
        $runTime = ($runEnd - $runStart).TotalSeconds
        
        if ($exitCode -eq 0) {
            Write-Host "PASS" -ForegroundColor Green -NoNewline
            Write-Host " (${runTime}s)" -ForegroundColor Gray
            $successCount++
        } else {
            Write-Host "FAIL" -ForegroundColor Red -NoNewline 
            Write-Host " (exit code: $exitCode, ${runTime}s)" -ForegroundColor Gray
            $failureCount++
            
            # Show error details for failures
            Write-Host "  Error output:" -ForegroundColor Yellow
            $output | Select-Object -Last 10 | ForEach-Object { Write-Host "    $_" -ForegroundColor Red }
            
            # Stop on first failure to preserve crash state
            Write-Host ""
            Write-Host "STOPPING on first failure to preserve crash state" -ForegroundColor Red
            break
        }
    } catch {
        Write-Host "EXCEPTION: $($_.Exception.Message)" -ForegroundColor Magenta
        $failureCount++
        break
    }
}

$endTime = Get-Date
$totalTime = ($endTime - $startTime).TotalMinutes

Write-Host ""
Write-Host "=== STABILITY TEST RESULTS ===" -ForegroundColor Cyan
Write-Host "Total runs: $($successCount + $failureCount)" -ForegroundColor White
Write-Host "Successes: $successCount" -ForegroundColor Green  
Write-Host "Failures: $failureCount" -ForegroundColor Red
Write-Host "Success rate: $(if($successCount + $failureCount -gt 0) { [math]::Round(($successCount / ($successCount + $failureCount)) * 100, 1) } else { 0 })%" -ForegroundColor White
Write-Host "Total time: $([math]::Round($totalTime, 1)) minutes" -ForegroundColor Gray
Write-Host ""

if ($failureCount -eq 0) {
    Write-Host "🎉 ALL TESTS PASSED - Heap corruption fixes are working!" -ForegroundColor Green
    Write-Host "Plugin is stable under repeated validation stress" -ForegroundColor Green
    exit 0
} else {
    Write-Host "❌ STABILITY ISSUES DETECTED" -ForegroundColor Red
    Write-Host "Check tripwire counters in PerfHUD during testing:" -ForegroundColor Yellow
    Write-Host "- TW: badBinSkips/badColSkips should remain 0" -ForegroundColor Yellow  
    Write-Host "- ΔD: deltaDrains should be reasonable (<16/block)" -ForegroundColor Yellow
    exit 1
}