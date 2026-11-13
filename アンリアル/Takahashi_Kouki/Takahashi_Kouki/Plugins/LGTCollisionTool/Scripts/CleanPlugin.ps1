param(
    [switch]$Force
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$PluginDir = (Resolve-Path (Join-Path $ScriptDir "..")).Path

if (-not $Force)
{
    $answer = Read-Host "Delete LGTCollisionTool Binaries and Intermediate? Type YES to continue"
    if ($answer -ne "YES")
    {
        Write-Host "Cancelled."
        exit 0
    }
}

foreach ($name in @("Binaries", "Intermediate"))
{
    $path = Join-Path $PluginDir $name
    if (Test-Path -LiteralPath $path)
    {
        Remove-Item -LiteralPath $path -Recurse -Force
        Write-Host "Deleted: $path"
    }
}
