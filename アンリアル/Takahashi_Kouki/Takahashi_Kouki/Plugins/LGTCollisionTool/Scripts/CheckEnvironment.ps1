$ErrorActionPreference = "Stop"

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$PluginDir = (Resolve-Path (Join-Path $ScriptDir "..")).Path
$ProjectDir = (Resolve-Path (Join-Path $PluginDir "..\..")).Path

$EngineRoot = if (-not [string]::IsNullOrWhiteSpace($env:UE57_ROOT)) {
    $env:UE57_ROOT
} elseif (-not [string]::IsNullOrWhiteSpace($env:UE_ROOT)) {
    $env:UE_ROOT
} else {
    "C:\Program Files\Epic Games\UE_5.7"
}

$BuildBat = Join-Path $EngineRoot "Engine\Build\BatchFiles\Build.bat"
$UPlugin = @(Get-ChildItem -LiteralPath $PluginDir -Filter "*.uplugin" -File -ErrorAction SilentlyContinue)
$UProject = @(Get-ChildItem -LiteralPath $ProjectDir -Filter "*.uproject" -File -ErrorAction SilentlyContinue)

Write-Host "PluginDir : $PluginDir"
Write-Host "ProjectDir: $ProjectDir"
Write-Host "EngineRoot: $EngineRoot"
Write-Host "Build.bat : $(Test-Path -LiteralPath $BuildBat)"
Write-Host "uplugin   : $($UPlugin.Count) file(s)"
Write-Host "uproject  : $($UProject.Count) file(s)"
Write-Host "EditorRun : $([bool](Get-Process -Name 'UnrealEditor' -ErrorAction SilentlyContinue))"
