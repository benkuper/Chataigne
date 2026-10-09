param([string]$FixturePath = '')
$ErrorActionPreference = 'Stop'
$repoPath = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$projectDir = Join-Path $repoPath 'Builds/VisualStudio2022'
$objectDir = Join-Path $projectDir 'x64/Debug/App'
$testDir = Join-Path $objectDir 'cv_values_integration_test'
New-Item -ItemType Directory -Path $testDir -Force | Out-Null
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vsPath) { throw 'Visual Studio C++ tools are required.' }
[xml]$project = Get-Content -LiteralPath (Join-Path $projectDir 'Chataigne_App.vcxproj')
$compile = $project.Project.ItemDefinitionGroup[0].ClCompile
$testObject = Join-Path $testDir 'cv_values_integration_test.obj'
$testExe = Join-Path $repoPath 'Binaries/Debug/App/cv_values_integration_test.exe'
$compileOutputDir = $testDir.Replace('\', '/') + '/'
$compileArguments = @('/nologo', '/c', '/std:c++17', '/EHsc', '/MDd', '/bigobj', '/Zi', "/Fo`"$compileOutputDir`"", "/Fd`"$testDir\compile.pdb`"")
foreach ($include in $compile.AdditionalIncludeDirectories.Split(';')) {
    if (!$include.StartsWith('%(')) { $compileArguments += "/I`"$include`"" }
}
foreach ($define in $compile.PreprocessorDefinitions.Split(';')) {
    if (!$define.StartsWith('%(')) { $compileArguments += "/D`"$define`"" }
}
$compileArguments += "`"$(Join-Path $PSScriptRoot 'cv_values_integration_test.cpp')`""
$compileArguments += "`"$(Join-Path $repoPath 'Source/TimeMachine/TimeMachineIncludes.cpp')`""
$compileResponse = Join-Path $testDir 'compile.rsp'
Set-Content -LiteralPath $compileResponse -Value ($compileArguments -join "`r`n")
$linkLines = Get-Content -LiteralPath (Join-Path $objectDir 'Chataigne_App.tlog/link.command.1.tlog')
$linkArguments = $linkLines | Where-Object { $_.StartsWith('/OUT:') } | Select-Object -First 1
$objectLine = $linkLines | Where-Object { $_.StartsWith('^') } | Select-Object -First 1
if (!$linkArguments -or !$objectLine) { throw 'Build Chataigne Debug first.' }
$linkArguments = $linkArguments -replace '/OUT:"[^"]+"', "/OUT:`"$testExe`""
$linkArguments = $linkArguments -replace '/PDB:"[^"]+"', "/PDB:`"$objectDir\cv_values_test.pdb`""
$linkArguments = $linkArguments -replace '/IMPLIB:"[^"]+"', "/IMPLIB:`"$objectDir\cv_values_test.lib`""
$linkArguments = $linkArguments -replace '/SUBSYSTEM:WINDOWS', '/SUBSYSTEM:CONSOLE'
foreach ($object in $objectLine.Substring(1).Split('|')) {
    if ($object.EndsWith('.OBJ') -and [IO.Path]::GetFileName($object) -ine 'TimeMachineIncludes.obj') {
        $linkArguments += " `"$object`""
    }
}
$linkArguments += " `"$testDir\TimeMachineIncludes.obj`""
$linkArguments += " `"$testObject`""
$linkResponse = Join-Path $objectDir 'cv_values_test_link.rsp'
Set-Content -LiteralPath $linkResponse -Value $linkArguments
Push-Location $projectDir
try {
    & cmd /c "`"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul && cl @`"$compileResponse`" && link @`"$linkResponse`""
    if ($LASTEXITCODE -ne 0) { throw 'Integration test build failed.' }
    # Native dependency diagnostics on stderr must not become PowerShell exceptions.
    $previousErrorPreference = $ErrorActionPreference
    try {
        $ErrorActionPreference = 'Continue'
        if ($FixturePath) { & $testExe $FixturePath 2>&1 | ForEach-Object { "$PSItem" } }
        else { & $testExe 2>&1 | ForEach-Object { "$PSItem" } }
        $testExitCode = $LASTEXITCODE
    } finally { $ErrorActionPreference = $previousErrorPreference }
    if ($testExitCode -ne 0) { throw 'Integration tests failed.' }
} finally { Pop-Location }
