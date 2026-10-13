param([string]$JucePath = 'D:\Projects\Dev\JUCE')
$ErrorActionPreference = 'Stop'
$repoPath = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$projectDir = Join-Path $repoPath 'Builds/VisualStudio2022'
$buildDir = Join-Path $repoPath 'Binaries/mcp-framework-check'
New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vsPath) { throw 'Visual Studio C++ tools are required.' }
if (!(Test-Path -LiteralPath (Join-Path $JucePath 'modules/juce_core/juce_core.h'))) { throw 'JucePath must identify the existing shared JUCE checkout.' }
[xml]$project = Get-Content -LiteralPath (Join-Path $projectDir 'Chataigne_App.vcxproj')
$compile = $project.Project.ItemDefinitionGroup[0].ClCompile
foreach ($enabled in @(0, 1)) {
    $arguments = @('/nologo', '/c', '/std:c++17', '/EHsc', '/MDd', '/bigobj', '/Y-', "/DORGANICUI_USE_MCP=$enabled", "/DORGANICUI_USE_ASSISTANT=$enabled", "/Fo`"$buildDir/organicui-mcp-$enabled.obj`"", "/I`"$JucePath/modules`"")
    foreach ($include in $compile.AdditionalIncludeDirectories.Split(';')) {
        if (!$include.StartsWith('%(') -and $include -notmatch 'JUCE[\\/]modules') { $arguments += "/I`"$include`"" }
    }
    foreach ($define in $compile.PreprocessorDefinitions.Split(';')) {
        if (!$define.StartsWith('%(') -and $define -notmatch '^ORGANICUI_USE_(MCP|ASSISTANT)=') { $arguments += "/D`"$define`"" }
    }
    $arguments += "`"$repoPath/JuceLibraryCode/include_juce_organicui2.cpp`""
    $response = Join-Path $buildDir "compile-$enabled.rsp"
    Set-Content -LiteralPath $response -Value ($arguments -join "`r`n")
    Push-Location $projectDir
    try {
        & cmd /c "`"$vsPath\VC\Auxiliary\Build\vcvars64.bat`" >nul && cl @`"$response`""
        if ($LASTEXITCODE -ne 0) { throw "OrganicUI compilation failed with MCP=$enabled" }
    } finally { Pop-Location }
}
Write-Output 'OrganicUI compiles with MCP disabled and enabled.'
