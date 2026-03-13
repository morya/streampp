# PowerShell script to deploy Qt dependencies

$QtPath = "C:\Qt\6.10.0\mingw_64\bin"
$BuildPath = "..\build\release"
$ExeName = "ScreenStreamer.exe"

# Add Qt to PATH
$env:PATH = "$QtPath;$env:PATH"

# Run windeployqt
Write-Host "Deploying Qt dependencies..." -ForegroundColor Green
windeployqt --qmldir ..\src --release "$BuildPath\$ExeName"

# Copy FFmpeg DLLs (adjust path as needed)
$FfmpegPath = "C:\ffmpeg\bin"
if (Test-Path $FfmpegPath) {
    Write-Host "Copying FFmpeg DLLs..." -ForegroundColor Green
    Copy-Item "$FfmpegPath\*.dll" -Destination $BuildPath -Force
}

Write-Host "Deployment completed!" -ForegroundColor Green
Write-Host "Output directory: $BuildPath" -ForegroundColor Yellow