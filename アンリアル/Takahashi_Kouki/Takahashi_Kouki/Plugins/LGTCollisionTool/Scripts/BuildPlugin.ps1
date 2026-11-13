$ErrorActionPreference = "Stop"

function Resolve-SingleFile([string]$Directory, [string]$Filter, [string]$Label)
{
    $files = @(Get-ChildItem -LiteralPath $Directory -Filter $Filter -File -ErrorAction Stop)
    if ($files.Count -eq 0)
    {
        throw "$Label was not found in: $Directory"
    }
    if ($files.Count -gt 1)
    {
        $names = ($files | ForEach-Object { $_.Name }) -join ", "
        throw "Multiple $Label files were found in $Directory : $names"
    }
    return $files[0]
}

try
{
    $ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
    $PluginDir = (Resolve-Path (Join-Path $ScriptDir "..")).Path
    $ProjectDir = (Resolve-Path (Join-Path $PluginDir "..\..")).Path

    $UPlugin = Resolve-SingleFile $PluginDir "*.uplugin" "uplugin"
    $UProject = Resolve-SingleFile $ProjectDir "*.uproject" "uproject"

    $EngineRoot = $null
    if (-not [string]::IsNullOrWhiteSpace($env:UE57_ROOT))
    {
        $EngineRoot = $env:UE57_ROOT
    }
    elseif (-not [string]::IsNullOrWhiteSpace($env:UE_ROOT))
    {
        $EngineRoot = $env:UE_ROOT
    }
    else
    {
        $EngineRoot = "C:\Program Files\Epic Games\UE_5.7"
    }

    $BuildBat = Join-Path $EngineRoot "Engine\Build\BatchFiles\Build.bat"
    if (-not (Test-Path -LiteralPath $BuildBat))
    {
        throw "UE5.7 Build.bat was not found: $BuildBat`nSet UE57_ROOT if UE5.7 is installed elsewhere."
    }

    $EditorProcesses = @(Get-Process -Name "UnrealEditor" -ErrorAction SilentlyContinue)
    if ($EditorProcesses.Count -gt 0)
    {
        throw "Unreal Editor is running. Close UE5.7 before building so the plugin DLL can be replaced."
    }

    Write-Host "Project : $($UProject.FullName)"
    Write-Host "Plugin  : $($UPlugin.FullName)"
    Write-Host "Engine  : $EngineRoot"
    Write-Host ""
    Write-Host "Building directly into: $PluginDir\Binaries\Win64"
    Write-Host ""

    & $BuildBat `
        UnrealEditor `
        Win64 `
        Development `
        "-Project=$($UProject.FullName)" `
        "-Plugin=$($UPlugin.FullName)" `
        -WaitMutex `
        -NoHotReload

    $BuildExitCode = $LASTEXITCODE
    if ($BuildExitCode -ne 0)
    {
        throw "Unreal Build Tool failed. ExitCode=$BuildExitCode"
    }

    $BinaryDir = Join-Path $PluginDir "Binaries\Win64"
    $Dll = Get-ChildItem -LiteralPath $BinaryDir -Filter "UnrealEditor-LGTCollisionTool.dll" -File -ErrorAction SilentlyContinue | Select-Object -First 1

    if ($null -eq $Dll)
    {
        throw "Build succeeded, but UnrealEditor-LGTCollisionTool.dll was not found in: $BinaryDir"
    }

    Write-Host ""
    Write-Host "Build succeeded."
    Write-Host "DLL : $($Dll.FullName)"
    Write-Host "Time: $($Dll.LastWriteTime.ToString('yyyy-MM-dd HH:mm:ss'))"
    exit 0
}
catch
{
    Write-Host ""
    Write-Host "ERROR: $($_.Exception.Message)" -ForegroundColor Red
    exit 1
}
