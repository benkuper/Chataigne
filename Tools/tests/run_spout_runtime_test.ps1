param([string]$RuntimeDirectory = (Join-Path $PSScriptRoot '../../Binaries/App'))
$ErrorActionPreference = 'Stop'
$repoPath = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$runtimePath = (Resolve-Path -LiteralPath $RuntimeDirectory).Path
$runtimeDll = Join-Path $runtimePath 'SpoutLibrary.dll'
if (!(Test-Path -LiteralPath $runtimeDll)) { throw 'The staged SpoutLibrary.dll is missing.' }

$testDir = Join-Path $repoPath 'Binaries/tests/spout-runtime'
New-Item -ItemType Directory -Path $testDir -Force | Out-Null
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vsPath) { throw 'Visual Studio C++ tools are required.' }
$source = Join-Path $PSScriptRoot 'spout_runtime_test.cpp'
$includeDir = Join-Path $repoPath 'Modules/juce_sharedtexture'
$testExe = Join-Path $testDir 'spout_runtime_test.exe'
$testObject = Join-Path $testDir 'spout_runtime_test.obj'

& cmd /c "`"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul && cl /nologo /std:c++17 /EHsc /MD /I`"$includeDir`" `"$source`" /Fo`"$testObject`" /Fe`"$testExe`""
if ($LASTEXITCODE -ne 0) { throw 'Spout runtime test compilation failed.' }
& $testExe $runtimeDll
if ($LASTEXITCODE -ne 0) { throw "Spout runtime test failed with exit code $LASTEXITCODE. Check the header/DLL compatibility." }
