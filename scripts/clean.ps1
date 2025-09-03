# Clean build and generated outputs (Windows PowerShell)
param(
  [switch]$Hard
)

$paths = @(
  'build', 'out', 'build_minimal', 'Release', 'Debug', 'RelWithDebInfo', 'MinSizeRel',
  'x64', 'x86', 'JuceLibraryCode', 'Builds'
)

foreach ($p in $paths) {
  if (Test-Path $p) {
    Write-Host "Removing $p" -ForegroundColor Yellow
    Remove-Item -Recurse -Force $p -ErrorAction SilentlyContinue
  }
}

if ($Hard) {
  Get-ChildItem -Recurse -Include *.exe,*.dll,*.lib,*.pdb,*.ilk,*.exp,*.obj,*.pch,*.idb,*.ipdb,*.iobj,*.res,*.manifest,*.tlog,*.lastbuildstate | ForEach-Object {
    Write-Host "Removing $_" -ForegroundColor Yellow
    Remove-Item -Force $_.FullName -ErrorAction SilentlyContinue
  }
}

Write-Host "Clean complete." -ForegroundColor Green

