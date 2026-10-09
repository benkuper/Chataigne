[CmdletBinding()]
param(
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Debug',
    [string[]]$InterfaceIPs = @(),
    [ValidateSet('sacn_network_interface_test', 'artnet_network_interface_test')]
    [string]$TestName = 'sacn_network_interface_test',
    [switch]$SkipBuild
)
$ErrorActionPreference = 'Stop'
$repoPath = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$projectDir = Join-Path $repoPath 'Builds/VisualStudio2022'
$objectDir = Join-Path $projectDir "x64/$Configuration/App"
$testDir = Join-Path $objectDir $TestName
New-Item -ItemType Directory -Path $testDir -Force | Out-Null
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vsPath) { throw 'Visual Studio C++ tools are required.' }
if (!$SkipBuild) {
    $msbuild = Join-Path $vsPath 'MSBuild/Current/Bin/MSBuild.exe'
    & $msbuild (Join-Path $projectDir 'Chataigne_App.vcxproj') /t:Build "/p:Configuration=$Configuration" /p:Platform=x64 /p:PreferredToolArchitecture=x64 /m:2 /v:minimal /nologo
    if ($LASTEXITCODE -ne 0) { throw "Chataigne $Configuration build failed." }
}
[xml]$project = Get-Content -LiteralPath (Join-Path $projectDir 'Chataigne_App.vcxproj')
$compile = ($project.Project.ItemDefinitionGroup | Where-Object { $_.Condition.Contains("$Configuration|x64") } | Select-Object -First 1).ClCompile
$compileOutputDir = $testDir.Replace('\', '/') + '/'
$runtimeFlag = if ($Configuration -eq 'Debug') { '/MDd' } else { '/MD' }
$compileArguments = @('/nologo', '/c', '/std:c++17', '/EHsc', $runtimeFlag, '/bigobj', '/Zi', "/Fo`"$compileOutputDir`"", "/Fd`"$testDir\compile.pdb`"")
foreach ($include in $compile.AdditionalIncludeDirectories.Split(';')) {
    if (!$include.StartsWith('%(')) { $compileArguments += "/I`"$include`"" }
}
foreach ($define in $compile.PreprocessorDefinitions.Split(';')) {
    if (!$define.StartsWith('%(')) { $compileArguments += "/D`"$define`"" }
}
# Link the current application objects, including production changes rebuilt above.
$testSource = Join-Path $PSScriptRoot "$TestName.cpp"
$compileArguments += "`"$testSource`""
$compileResponse = Join-Path $testDir 'compile.rsp'
Set-Content -LiteralPath $compileResponse -Value ($compileArguments -join "`r`n")
$linkLines = Get-Content -LiteralPath (Join-Path $objectDir 'Chataigne_App.tlog/link.command.1.tlog')
$linkArguments = $linkLines | Where-Object { $_.StartsWith('/OUT:') } | Select-Object -First 1
$objectLine = $linkLines | Where-Object { $_.StartsWith('^') } | Select-Object -First 1
if (!$linkArguments -or !$objectLine) { throw "Build Chataigne $Configuration first." }
$applicationOutput = [regex]::Match($linkArguments, '/OUT:"([^"]+)"').Groups[1].Value
if (![IO.Path]::IsPathRooted($applicationOutput)) {
    $applicationOutput = [IO.Path]::GetFullPath((Join-Path $projectDir $applicationOutput))
}
# Keep the test executable beside the application's runtime DLLs in both configurations.
$testExe = Join-Path (Split-Path -Parent $applicationOutput) "$TestName.exe"
New-Item -ItemType Directory -Path (Split-Path -Parent $testExe) -Force | Out-Null
$linkArguments = $linkArguments -replace '/OUT:"[^"]+"', "/OUT:`"$testExe`""
$linkArguments = $linkArguments -replace '/PDB:"[^"]+"', "/PDB:`"$testDir\test.pdb`""
$linkArguments = $linkArguments -replace '/IMPLIB:"[^"]+"', "/IMPLIB:`"$testDir\test.lib`""
$linkArguments = $linkArguments -replace '/LTCGOUT:"[^"]+"', "/LTCGOUT:`"$testDir\test.iobj`""
$linkArguments = $linkArguments -replace '/SUBSYSTEM:WINDOWS', '/SUBSYSTEM:CONSOLE'
foreach ($object in $objectLine.Substring(1).Split('|')) {
    if ($object.EndsWith('.OBJ')) { $linkArguments += " `"$object`"" }
}
$linkArguments += " `"$testDir\$TestName.obj`""
$linkResponse = Join-Path $testDir 'link.rsp'
Set-Content -LiteralPath $linkResponse -Value $linkArguments
Push-Location $projectDir
try {
    & cmd /c "`"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul && cl @`"$compileResponse`" && link @`"$linkResponse`""
    if ($LASTEXITCODE -ne 0) { throw "$TestName build failed." }
    $previousErrorPreference = $ErrorActionPreference
    try {
        $ErrorActionPreference = 'Continue'
        & $testExe @InterfaceIPs
        $testExitCode = $LASTEXITCODE
    } finally { $ErrorActionPreference = $previousErrorPreference }
    if ($testExitCode -ne 0) { throw "$TestName failed (exit $testExitCode)." }
} finally { Pop-Location }
