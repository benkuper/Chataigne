$ErrorActionPreference = 'Stop'
$repoPath = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$projectDir = Join-Path $repoPath 'Builds/VisualStudio2022'
$objectDir = Join-Path $projectDir 'x64/Debug/App'
$testDir = Join-Path $objectDir 'timeline_audio_transport_test'
New-Item -ItemType Directory -Path $testDir -Force | Out-Null
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vsPath) { throw 'Visual Studio C++ tools are required.' }
[xml]$project = Get-Content -LiteralPath (Join-Path $projectDir 'Chataigne_App.vcxproj')
$compile = $project.Project.ItemDefinitionGroup[0].ClCompile
$compileArguments = @('/nologo', '/c', '/std:c++17', '/EHsc', '/MDd', '/Zi', "/Fo`"$testDir\test.obj`"", "/Fd`"$testDir\compile.pdb`"")
foreach ($include in $compile.AdditionalIncludeDirectories.Split(';')) {
    if (!$include.StartsWith('%(')) { $compileArguments += "/I`"$include`"" }
}
foreach ($define in $compile.PreprocessorDefinitions.Split(';')) {
    if (!$define.StartsWith('%(')) { $compileArguments += "/D`"$define`"" }
}
$compileArguments += "`"$(Join-Path $PSScriptRoot 'timeline_audio_transport_test.cpp')`""
$compileResponse = Join-Path $testDir 'compile.rsp'
Set-Content -LiteralPath $compileResponse -Value ($compileArguments -join "`r`n")
$testExe = Join-Path $repoPath 'Binaries/Debug/App/timeline_audio_transport_test.exe'
$linkLines = Get-Content -LiteralPath (Join-Path $objectDir 'Chataigne_App.tlog/link.command.1.tlog')
$linkArguments = $linkLines | Where-Object { $_.StartsWith('/OUT:') } | Select-Object -First 1
if (!$linkArguments) { throw 'Build Chataigne Debug first.' }
$linkArguments = $linkArguments -replace '/OUT:"[^"]+"', "/OUT:`"$testExe`""
$linkArguments = $linkArguments -replace '/PDB:"[^"]+"', "/PDB:`"$testDir\test.pdb`""
$linkArguments = $linkArguments -replace '/IMPLIB:"[^"]+"', "/IMPLIB:`"$testDir\test.lib`""
$linkArguments = $linkArguments -replace '/SUBSYSTEM:WINDOWS', '/SUBSYSTEM:CONSOLE'
$linkArguments += " `"$testDir\test.obj`""
foreach ($module in @('juce_core', 'juce_events', 'juce_data_structures', 'juce_graphics', 'juce_gui_basics', 'juce_gui_extra', 'juce_audio_basics', 'juce_audio_devices', 'juce_audio_formats')) {
    $dependency = Join-Path $objectDir "include_$module.obj"
    if (!(Test-Path -LiteralPath $dependency)) { throw 'Build Chataigne Debug first.' }
    $linkArguments += " `"$dependency`""
}
$linkResponse = Join-Path $testDir 'link.rsp'
Set-Content -LiteralPath $linkResponse -Value $linkArguments
Push-Location $projectDir
try {
    & cmd /c "`"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul && cl @`"$compileResponse`" && link @`"$linkResponse`""
    if ($LASTEXITCODE -ne 0) { throw 'Timeline audio transport test build failed.' }
    & $testExe
    if ($LASTEXITCODE -ne 0) { throw 'Timeline audio transport tests failed.' }
} finally { Pop-Location }
