[CmdletBinding()]
param(
    [ValidateSet('Build', 'Run')]
    [string]$Action = 'Run'
)

$ErrorActionPreference = 'Stop'
$repoPath = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$solutionPath = Join-Path $repoPath 'Builds/VisualStudio2022/Chataigne.sln'
$appDirectory = Join-Path $repoPath 'Binaries/Debug/App'
$executablePath = Join-Path $appDirectory 'Chataigne.exe'
$vswherePath = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'

if (!(Test-Path -LiteralPath $vswherePath)) {
    throw 'Install Visual Studio Build Tools with the Desktop development with C++ workload.'
}

$msbuildPath = & $vswherePath -latest -products '*' `
    -requires Microsoft.Component.MSBuild Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1

if (!$msbuildPath) {
    throw 'Visual Studio MSBuild and the C++ build tools are required.'
}

if (!(Test-Path -LiteralPath 'D:/Projects/Dev/JUCE/modules/juce_core/juce_core.h')) {
    throw 'The generated project requires the shared JUCE checkout at D:\Projects\Dev\JUCE.'
}

Push-Location $repoPath
try {
    Write-Host 'Building Chataigne Debug (x64)...'
    & $msbuildPath $solutionPath /t:Build /p:Configuration=Debug /p:Platform=x64 `
        /p:PreferredToolArchitecture=x64 /m:2 /v:minimal /nologo
    if ($LASTEXITCODE -ne 0) {
        throw "Debug build failed (exit $LASTEXITCODE)."
    }

    & (Join-Path $repoPath 'stage_mpv.bat') 'Binaries\Debug\App'
    if ($LASTEXITCODE -ne 0) {
        throw "Could not stage the Debug runtime dependencies (exit $LASTEXITCODE)."
    }

    if (!(Test-Path -LiteralPath $executablePath)) {
        throw "Debug executable was not produced: $executablePath"
    }

    if ($Action -eq 'Run') {
        Start-Process -FilePath $executablePath -WorkingDirectory $appDirectory -WindowStyle Normal
        Write-Host 'Launched Chataigne Debug.'
    } else {
        Write-Host 'Chataigne Debug is ready.'
    }
} finally {
    Pop-Location
}
