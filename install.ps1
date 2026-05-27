# PuttyAlt Installer - downloads without SmartScreen warning
# Usage: irm https://raw.githubusercontent.com/chillymasterio/puttyalt/main/install.ps1 | iex

[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

$Repo = "chillymasterio/puttyalt"
$Tag  = "v2.0.1"
$Url  = "https://github.com/$Repo/releases/download/$Tag/puttyalt.exe"
$Dir  = "$env:LOCALAPPDATA\PuttyAlt"
$Exe  = "$Dir\puttyalt.exe"

Write-Host ""
Write-Host "  PuttyAlt Installer" -ForegroundColor Cyan
Write-Host ""

# Create directory
if (!(Test-Path $Dir)) { New-Item -ItemType Directory -Path $Dir -Force | Out-Null }

# Download without MOTW
Write-Host "  Downloading..." -ForegroundColor Yellow
$wc = New-Object System.Net.WebClient
$wc.DownloadFile($Url, $Exe)
$wc.Dispose()

# Remove MOTW if somehow present
$zoneFile = "$Exe`:Zone.Identifier"
if (Test-Path $zoneFile) { Remove-Item $zoneFile -Force -ErrorAction SilentlyContinue }

$size = [math]::Round((Get-Item $Exe).Length / 1024)
Write-Host "  Downloaded: $Exe ($size KB)" -ForegroundColor Green

# Add to PATH for current user
$userPath = [Environment]::GetEnvironmentVariable("Path", "User")
if ($userPath -notlike "*$Dir*") {
    [Environment]::SetEnvironmentVariable("Path", "$userPath;$Dir", "User")
    Write-Host "  Added to PATH" -ForegroundColor Green
}

# Create Start Menu shortcut
$startMenu = "$env:APPDATA\Microsoft\Windows\Start Menu\Programs"
$lnk = "$startMenu\PuttyAlt.lnk"
try {
    $shell = New-Object -ComObject WScript.Shell
    $sc = $shell.CreateShortcut($lnk)
    $sc.TargetPath = $Exe
    $sc.Description = "PuttyAlt - Modern SSH Terminal"
    $sc.Save()
    Write-Host "  Start Menu shortcut created" -ForegroundColor Green
} catch {}

# Create Desktop shortcut
$desktop = [Environment]::GetFolderPath("Desktop")
$dlnk = "$desktop\PuttyAlt.lnk"
try {
    $sc2 = $shell.CreateShortcut($dlnk)
    $sc2.TargetPath = $Exe
    $sc2.Description = "PuttyAlt - Modern SSH Terminal"
    $sc2.Save()
    Write-Host "  Desktop shortcut created" -ForegroundColor Green
} catch {}

Write-Host ""
Write-Host "  Done! Run: puttyalt [user@]host[:port]" -ForegroundColor Cyan
Write-Host ""
