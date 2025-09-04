# Requires: PowerShell 5+ (Windows), CMake + MSVC installed in PATH
# Usage:
#   powershell -ExecutionPolicy Bypass -File tools/build-validator.ps1 -BuildDir build -Config RelWithDebInfo -PluginName SpectralCanvasPro
#   powershell -File tools/build-validator.ps1  # uses defaults

[CmdletBinding()]
param(
  [string]$BuildDir = "build",
  [string]$Config = "RelWithDebInfo",
  [string]$PluginName = "SpectralCanvasPro",
  [int]$StrictWarnings = 0,
  [int]$TimeoutSec = 1800,
  [string]$Preset = ""     # optional: CMakePresets.json preset name
)

$ErrorActionPreference = "Stop"
$ts = [int][double]::Parse((Get-Date -UFormat %s))
$artDir = Join-Path "artifacts/build-validator" $ts
New-Item -Force -ItemType Directory -Path $artDir | Out-Null
$log = Join-Path $artDir "build.log"
$summary = Join-Path $artDir "Build_Summary.md"

function Append-Log($msg) {
  $msg | Tee-Object -FilePath $log -Append | Out-Host
}

Append-Log "[build-validator] start @ $ts (Config=$Config)"

# Configure (only if cache missing)
if ($Preset -and (Test-Path "CMakePresets.json")) {
  Append-Log "[configure] using preset: $Preset"
  cmake --preset $Preset | Tee-Object -FilePath $log -Append | Out-Null
} elseif (!(Test-Path (Join-Path $BuildDir "CMakeCache.txt"))) {
  New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
  Append-Log "[configure] cmake -S . -B $BuildDir -DCMAKE_BUILD_TYPE=$Config"
  cmake -S . -B $BuildDir -DCMAKE_BUILD_TYPE=$Config | Tee-Object -FilePath $log -Append | Out-Null
} else {
  Append-Log "[configure] cache present → skipping"
}

# Build with timeout
$start = Get-Date
Append-Log "[build] cmake --build $BuildDir --config $Config"
$buildJob = Start-Job -ScriptBlock {
  param($BuildDir, $Config, $log)
  $ErrorActionPreference = "Stop"
  cmake --build $BuildDir --config $Config | Tee-Object -FilePath $log -Append | Out-Null
} -ArgumentList $BuildDir, $Config, $log

if (-not (Wait-Job $buildJob -Timeout $TimeoutSec)) {
  Stop-Job $buildJob | Out-Null
  Receive-Job $buildJob -ErrorAction SilentlyContinue | Out-Null
  $status = "FAIL"
  $duration = [int]((Get-Date) - $start).TotalSeconds
  $vst3 = $null
  $errSnippet = "Build timeout after ${TimeoutSec}s"
  goto EmitSummary
}

# Pull job output (ensures $LASTEXITCODE set)
Receive-Job $buildJob | Out-Null
Remove-Job $buildJob | Out-Null

# Determine build return (cmake returns via $LASTEXITCODE)
$rc = $LASTEXITCODE
$duration = [int]((Get-Date) - $start).TotalSeconds
$status = "PASS"
if ($rc -ne 0) { $status = "FAIL" }

# Artefact discovery: look for fresh *.vst3 newer than start of the *whole* run ($ts)
# Typical JUCE layout:
#   build\<PluginName>_artefacts\<Config>\VST3\<PluginName>.vst3
# Fallback to any .vst3 in build tree.
$vst3 = $null
$buildStart = [DateTimeOffset]::FromUnixTimeSeconds($ts).UtcDateTime
$allVst3 = Get-ChildItem -Path $BuildDir -Filter *.vst3 -Recurse -ErrorAction SilentlyContinue | `
  Where-Object { $_.LastWriteTimeUtc -gt $buildStart } | Sort-Object LastWriteTimeUtc -Descending
if ($allVst3.Count -gt 0) { $vst3 = $allVst3[0] }

if (-not $vst3 -or $vst3.Length -le 0) { $status = "FAIL" }

# Optional strict warnings (MSVC/Clang/GCC all emit "warning:")
$warnings = (Select-String -Path $log -Pattern "warning:" -SimpleMatch -ErrorAction SilentlyContinue | Measure-Object).Count
$errorsInLog = (Select-String -Path $log -Pattern " error:" -SimpleMatch -ErrorAction SilentlyContinue | Measure-Object).Count
if ($StrictWarnings -eq 1 -and $warnings -gt 0) { $status = "FAIL" }

# First error snippet (context ~5 lines) if failed
$errSnippet = ""
if ($status -eq "FAIL") {
  $firstErr = Select-String -Path $log -Pattern " error:" -SimpleMatch -Context 2,3 -ErrorAction SilentlyContinue | Select-Object -First 1
  if ($firstErr) {
    $errSnippet = "```\n" + ($firstErr | Format-List * | Out-String) + "```"
  } elseif ($rc -ne 0) {
    $errSnippet = "Build failed (rc=$rc), see build.log"
  }
}

# Toolchain metadata
$cache = Join-Path $BuildDir "CMakeCache.txt"
$generator = ""
$cxxPath = ""
if (Test-Path $cache) {
  $generator = (Select-String -Path $cache -Pattern "^CMAKE_GENERATOR:INTERNAL=" -ErrorAction SilentlyContinue).Line -replace ".*=",""
  $cxxPath = (Select-String -Path $cache -Pattern "^CMAKE_CXX_COMPILER:FILEPATH=" -ErrorAction SilentlyContinue).Line -replace ".*=",""
}

:EmitSummary
# Emit Build_Summary.md
$lines = @()
$lines += "STATUS: $status"
$lines += "DURATION: ${duration}s"
$lines += "CONFIG: $Config"
$lines += "GENERATOR: $generator"
$lines += "COMPILER: $cxxPath"
$lines += "VST3: " + ($(if ($vst3) { $vst3.FullName } else { "MISSING" }))
$lines += "WARNINGS: $warnings  ERRORS: $errorsInLog"
if ($errSnippet) {
  $lines += ""
  $lines += "First error context:"
  $lines += $errSnippet
}
$lines -join "`r`n" | Out-File -FilePath $summary -Encoding utf8

Write-Host "[build-validator] $status (${duration}s)"
Write-Host "Summary: $summary"
if ($status -ne "PASS") { exit 2 } else { exit 0 }