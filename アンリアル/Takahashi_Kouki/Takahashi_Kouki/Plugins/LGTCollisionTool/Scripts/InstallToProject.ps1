param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectDir
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$SourcePluginDir = (Resolve-Path (Join-Path $ScriptDir "..")).Path
$ProjectDir = (Resolve-Path $ProjectDir).Path
$DestPluginDir = Join-Path $ProjectDir "Plugins\LGTCollisionTool"

if (-not (Get-ChildItem -LiteralPath $ProjectDir -Filter "*.uproject" -File -ErrorAction SilentlyContinue))
{
    throw "No .uproject was found in: $ProjectDir"
}

New-Item -ItemType Directory -Path $DestPluginDir -Force | Out-Null

foreach ($name in @("Source", "Scripts", "Config", "Resources"))
{
    $src = Join-Path $SourcePluginDir $name
    if (Test-Path -LiteralPath $src)
    {
        Copy-Item -LiteralPath $src -Destination $DestPluginDir -Recurse -Force
    }
}

$uplugin = Get-ChildItem -LiteralPath $SourcePluginDir -Filter "*.uplugin" -File -ErrorAction SilentlyContinue | Select-Object -First 1
if ($null -ne $uplugin)
{
    Copy-Item -LiteralPath $uplugin.FullName -Destination $DestPluginDir -Force
}

Write-Host "Installed/updated: $DestPluginDir"
