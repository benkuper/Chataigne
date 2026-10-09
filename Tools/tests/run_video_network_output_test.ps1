param([switch]$RuntimeOnly, [string]$BuildFolder = "VisualStudio2022", [string]$Configuration = "Debug", [string]$RuntimeDirectory = "Binaries/Debug/App")
$ErrorActionPreference = 'Stop'
$repoPath = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$projectDir = Join-Path $repoPath "Builds/$BuildFolder"
$objectDir = Join-Path $projectDir "x64/$Configuration/App"
$testDir = Join-Path $objectDir 'video_network_output_test'
New-Item -ItemType Directory -Path $testDir -Force | Out-Null
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vsPath) { throw 'Visual Studio C++ tools are required.' }
[xml]$project = Get-Content -LiteralPath (Join-Path $projectDir 'Chataigne_App.vcxproj')
$configIndex = if ($Configuration -eq "Debug") { 0 } else { 1 }
$compile = $project.Project.ItemDefinitionGroup[$configIndex].ClCompile
$testObject = Join-Path $testDir 'test.obj'
$testExe = Join-Path $repoPath "$RuntimeDirectory/video_network_output_test.exe"
$compileArguments = @('/nologo', '/c', '/std:c++17', '/EHsc', '/bigobj', '/Zi', "/Fo`"$testObject`"", "/Fd`"$testDir\compile.pdb`"")
if ($Configuration -eq "Debug") { $compileArguments += "/MDd" } else { $compileArguments += "/MD" }
foreach ($include in $compile.AdditionalIncludeDirectories.Split(';')) {
    if (!$include.StartsWith('%(')) { $compileArguments += "/I`"$include`"" }
}
foreach ($define in $compile.PreprocessorDefinitions.Split(';')) {
    if (!$define.StartsWith('%(')) { $compileArguments += "/D`"$define`"" }
}
if ($RuntimeOnly) { $compileArguments += "/DCHATAIGNE_VIDEO_RUNTIME_ONLY=1" }
$compileArguments += "`"$(Join-Path $PSScriptRoot 'video_network_output_test.cpp')`""
$compileResponse = Join-Path $testDir 'compile.rsp'
Set-Content -LiteralPath $compileResponse -Value ($compileArguments -join "`r`n")
$linkLines = Get-Content -LiteralPath (Join-Path $objectDir 'Chataigne_App.tlog/link.command.1.tlog')
$linkArguments = $linkLines | Where-Object { $_.StartsWith('/OUT:') } | Select-Object -First 1
$objectLine = $linkLines | Where-Object { $_.StartsWith('^') } | Select-Object -First 1
if (!$linkArguments -or !$objectLine) { throw 'Build Chataigne Debug first.' }
$linkArguments = $linkArguments -replace '/OUT:"[^"]+"', "/OUT:`"$testExe`""
$linkArguments = $linkArguments -replace '/PDB:"[^"]+"', "/PDB:`"$testDir\test.pdb`""
$linkArguments = $linkArguments -replace '/IMPLIB:"[^"]+"', "/IMPLIB:`"$testDir\test.lib`""
$linkArguments = $linkArguments -replace '/SUBSYSTEM:WINDOWS', '/SUBSYSTEM:CONSOLE'
foreach ($object in $objectLine.Substring(1).Split('|')) {
    if ($object.EndsWith('.OBJ') -and (!$RuntimeOnly -or $object.EndsWith('INCLUDE_JUCE_CORE.OBJ'))) { $linkArguments += " `"$object`"" }
}
$linkArguments += " /ENTRY:mainCRTStartup `"$testObject`""
$linkResponse = Join-Path $testDir 'link.rsp'
Set-Content -LiteralPath $linkResponse -Value $linkArguments
Push-Location $projectDir
try {
    & cmd /c "`"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul && cl @`"$compileResponse`" && link @`"$linkResponse`""
    if ($LASTEXITCODE -ne 0) { throw 'Video network integration test build failed.' }
    $env:NDI_RUNTIME_DIR_V5 = Split-Path $testExe
    $env:NDI_RUNTIME_DIR_V6 = Split-Path $testExe
    $env:CHATAIGNE_OMT_RUNTIME_DIR = Split-Path $testExe
    # Existing native dependencies may log diagnostics on stderr.
    $previousErrorPreference = $ErrorActionPreference
    try {
        $ErrorActionPreference = 'Continue'
        $testOutput = & $testExe 2>&1
        $testOutput | ForEach-Object { "$PSItem" }
        $testExitCode = $LASTEXITCODE
    } finally { $ErrorActionPreference = $previousErrorPreference }
    if (!($testOutput -match 'Video network integration tests passed')) { throw 'Video network test did not reach its completion marker.' }
    if ($testExitCode -ne 0) { throw "Video network integration tests failed (exit $testExitCode)." }
} finally { Pop-Location }
