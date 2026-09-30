param(
    [Parameter(Mandatory = $true)] [string] $ProjectPath,
    [string] $AppPath = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../../Binaries/Debug/App/Chataigne.exe')),
    [string] $EasingType = '',
    [int] $FramesPerSecond = 0,
    [double] $CycleSeconds = 0.0,
    [switch] $IncludeState,
    [double] $PlaybackSpeed = [double]::NaN,
    [double] $StartTime = [double]::NaN,
    [switch] $LoopPlayback,
    [switch] $AssertLinear,
    [switch] $AssertFinite,
    [int] $Port = 8000,
    [double] $CaptureSeconds = 9.0,
    [string] $CapturePath = (Join-Path ([System.IO.Path]::GetTempPath()) 'chataigne_sequence_osc_timing.csv')
)

$ErrorActionPreference = 'Stop'
if (-not (Test-Path -LiteralPath $ProjectPath)) { throw "Project not found: $ProjectPath" }
if (-not (Test-Path -LiteralPath $AppPath)) { throw "Debug app not found: $AppPath" }
$temporaryDirectory = Join-Path ([System.IO.Path]::GetTempPath()) ('chataigne_timing_' + [guid]::NewGuid().ToString('N'))
$null = New-Item -ItemType Directory -Path $temporaryDirectory
$temporaryProject = Join-Path $temporaryDirectory 'timing-test.noisette'
$originalAppData = $env:APPDATA
$appProcess = $null
$receiver = $null

function Set-SequenceParameter($sequence, [string] $address, $parameterValue) {
    foreach ($parameter in $sequence.parameters) {
        if ($parameter.controlAddress -eq $address) {
            $parameter.value = $parameterValue
            return
        }
    }
    $sequence.parameters += [pscustomobject]@{ value = $parameterValue; controlAddress = $address }
}

try {
    $project = Get-Content -LiteralPath $ProjectPath -Raw | ConvertFrom-Json
    # This temporary copy contains only the timing fixture and loads directly
    # in the current binary, without an interactive migration prompt.
    $project.metaData.version = '1.10.5b1'
    $project.metaData.versionNumber = 0x10a05
    # The fixture's active State also maps its Signal module to OSC.
    # Isolate the sequence's automation output for this measurement.
    if (-not $IncludeState) {
        foreach ($state in $project.states.items) {
            foreach ($parameter in $state.parameters) {
                if ($parameter.controlAddress -eq '/active') { $parameter.value = $false }
            }
        }
    }
    $sequence = $project.sequences.items[0]
    if ($FramesPerSecond -gt 0) { Set-SequenceParameter $sequence '/fps' $FramesPerSecond }
    if ($CycleSeconds -gt 0.0) {
        Set-SequenceParameter $sequence '/totalTime' $CycleSeconds
        $automation = $sequence.layers.items[0].containers.automation
        foreach ($parameter in $automation.parameters) {
            if ($parameter.controlAddress -eq '/length') { $parameter.value = $CycleSeconds }
        }
        $keys = $automation.items
        for ($keyIndex = 0; $keyIndex -lt $keys.Count; ++$keyIndex) {
            foreach ($parameter in $keys[$keyIndex].parameters) {
                if ($parameter.controlAddress -eq '/position') {
                    $parameter.value = $CycleSeconds * $keyIndex / ($keys.Count - 1)
                }
            }
        }
    }
    if (-not [double]::IsNaN($PlaybackSpeed)) { Set-SequenceParameter $sequence '/playSpeed' $PlaybackSpeed }
    if (-not [double]::IsNaN($StartTime)) {
        Set-SequenceParameter $sequence '/saveCurrentTime' $true
        Set-SequenceParameter $sequence '/currentTime' $StartTime
    }
    if ($LoopPlayback) { Set-SequenceParameter $sequence '/loop' $true }
    if ($EasingType -ne '') {
        foreach ($key in $sequence.layers.items[0].containers.automation.items) {
            foreach ($parameter in $key.parameters) {
                if ($parameter.controlAddress -eq '/easingType') { $parameter.value = $EasingType }
            }
        }
    }
    Set-SequenceParameter $sequence '/playAtLoad' $true
    $sequence.layers.items[0].containers.mapping.outputs.items[0].parameters[0].value = $true
    $project | ConvertTo-Json -Depth 100 -Compress | Set-Content -LiteralPath $temporaryProject -Encoding utf8

    $receiver = [System.Net.Sockets.UdpClient]::new($Port)
    $receiver.Client.ReceiveTimeout = 1000
    $endpoint = [System.Net.IPEndPoint]::new([System.Net.IPAddress]::Any, 0)
    $rows = [System.Collections.Generic.List[object]]::new()
    $watch = [System.Diagnostics.Stopwatch]::StartNew()
    $firstPacketMs = $null

    $env:APPDATA = $temporaryDirectory
    $appProcess = Start-Process -FilePath $AppPath -ArgumentList @('-f', ('"' + $temporaryProject + '"')) -WindowStyle Hidden -PassThru
    $env:APPDATA = $originalAppData

    while ($watch.Elapsed.TotalSeconds -lt ($CaptureSeconds + 5.0)) {
        if ($null -ne $firstPacketMs -and ($watch.Elapsed.TotalMilliseconds - $firstPacketMs) / 1000.0 -ge $CaptureSeconds) { break }
        if ($appProcess.HasExited) { break }

        try {
            $bytes = $receiver.Receive([ref] $endpoint)
        }
        catch [System.Net.Sockets.SocketException] {
            if ($_.Exception.SocketErrorCode -eq [System.Net.Sockets.SocketError]::TimedOut) { continue }
            throw
        }

        if ($bytes.Length -lt 12) { continue }
        $addressEnd = [Array]::IndexOf($bytes, [byte] 0)
        if ($addressEnd -lt 0) { continue }
        $address = [System.Text.Encoding]::ASCII.GetString($bytes, 0, $addressEnd)
        if ($address -ne '/example') {
            Write-Output "Unexpected OSC address: $address"
            continue
        }

        $word = [byte[]]::new(4)
        [Array]::Copy($bytes, $bytes.Length - 4, $word, 0, 4)
        if ([BitConverter]::IsLittleEndian) { [Array]::Reverse($word) }
        $value = [BitConverter]::ToSingle($word, 0)
        $receivedMs = $watch.Elapsed.TotalMilliseconds
        if ($null -eq $firstPacketMs) { $firstPacketMs = $receivedMs }
        $rows.Add([pscustomobject]@{
            receivedMs = [math]::Round($receivedMs - $firstPacketMs, 3)
            value = $value
        })
    }

    if ($rows.Count -eq 0) {
        Write-Output "App exited: $($appProcess.HasExited)"
        Get-CimInstance Win32_Process -Filter "ProcessId = $($appProcess.Id)" | Select-Object -ExpandProperty CommandLine | Write-Output
        Get-ChildItem -LiteralPath $temporaryDirectory -Recurse -File | Select-Object FullName,Length | Format-Table | Out-String | Write-Output
        throw 'No /example OSC packets received from the test sequence.'
    }

    $previous = $null
    $shortIntervals = 0
    $zeroSteps = 0
    $doubleSteps = 0
    foreach ($row in $rows) {
        $interval = if ($null -ne $previous) { $row.receivedMs - $previous.receivedMs } else { $null }
        $step = if ($null -ne $previous) { $row.value - $previous.value } else { $null }
        if ($null -ne $interval -and $interval -lt 20.0) { ++$shortIntervals }
        if ($null -ne $step -and [math]::Abs($step) -lt 0.0001) { ++$zeroSteps }
        if ($null -ne $step -and [math]::Abs($step) -ge 0.006) { ++$doubleSteps }
        $row | Add-Member -NotePropertyName intervalMs -NotePropertyValue $interval
        $row | Add-Member -NotePropertyName step -NotePropertyValue $step
        $previous = $row
    }

    $rows | Export-Csv -LiteralPath $CapturePath -NoTypeInformation

    if ($AssertFinite) {
        foreach ($row in $rows) {
            if ([double]::IsNaN($row.value) -or [double]::IsInfinity($row.value)) {
                throw "Non-finite value at $($row.receivedMs) ms"
            }
        }
    }

    if ($AssertLinear) {
        if ($IncludeState) { throw '-AssertLinear requires the state mapping to be disabled.' }
        if ($EasingType -ne '' -and $EasingType -ne 'Linear') { throw '-AssertLinear requires Linear easing.' }
        $speed = if ([double]::IsNaN($PlaybackSpeed)) { 1.0 } else { [math]::Abs($PlaybackSpeed) }
        $fps = if ($FramesPerSecond -gt 0) { $FramesPerSecond } else { 25 }
        $expectedStep = $speed / (10.0 * $fps)
        $expectedInterval = 1000.0 / $fps
        $steadyRows = @($rows | Where-Object { $_.receivedMs -ge 1000.0 })
        if ($steadyRows.Count -lt [math]::Max(10, ($CaptureSeconds - 2.0) * $fps * 0.8)) {
            throw "Too few steady-state packets: $($steadyRows.Count)"
        }
        foreach ($row in $steadyRows) {
            if ([math]::Abs([math]::Abs([double]$row.step) - $expectedStep) -gt 0.000002) {
                throw "Unexpected linear value step $($row.step) at $($row.receivedMs) ms"
            }
        }
        $averageInterval = ($steadyRows | Measure-Object -Property intervalMs -Average).Average
        if ([math]::Abs($averageInterval - $expectedInterval) -gt [math]::Max(1.0, 0.05 * $expectedInterval)) {
            throw "Mean steady-state interval was $averageInterval ms, expected $expectedInterval ms"
        }
    }
    Write-Output "Packets: $($rows.Count); intervals <20 ms: $shortIntervals; zero steps: $zeroSteps; steps >=0.006: $doubleSteps"
    Write-Output "Capture: $CapturePath"
}
finally {
    $env:APPDATA = $originalAppData
    if ($null -ne $receiver) { $receiver.Dispose() }
    if ($null -ne $appProcess -and -not $appProcess.HasExited) {
        Stop-Process -Id $appProcess.Id -Force
        $null = $appProcess.WaitForExit(5000)
    }
    if (Test-Path -LiteralPath $temporaryDirectory) {
        $resolvedTemporaryDirectory = (Resolve-Path -LiteralPath $temporaryDirectory).Path
        $resolvedTempRoot = (Resolve-Path -LiteralPath ([System.IO.Path]::GetTempPath())).Path.TrimEnd('\')
        if ($resolvedTemporaryDirectory.StartsWith($resolvedTempRoot + '\chataigne_timing_', [System.StringComparison]::OrdinalIgnoreCase)) {
            Remove-Item -LiteralPath $resolvedTemporaryDirectory -Recurse -Force
        }
    }
}
