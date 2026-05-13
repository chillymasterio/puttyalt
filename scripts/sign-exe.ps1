# PuttyAlt Code Signing Script
# This script signs puttyalt.exe with a code signing certificate
# to bypass Windows SmartScreen filter.
#
# Usage:
#   .\sign-exe.ps1 -ExePath ".\puttyalt.exe" -CertPath ".\cert.pfx" -Password "pass"
#   .\sign-exe.ps1 -ExePath ".\puttyalt.exe" -Thumbprint "CERT_THUMBPRINT"
#
# To create a self-signed certificate for testing:
#   $cert = New-SelfSignedCertificate -Type CodeSigningCert -Subject "CN=PuttyAlt Project" -CertStoreLocation Cert:\CurrentUser\My
#   Export-PfxCertificate -Cert $cert -FilePath .\puttyalt-sign.pfx -Password (ConvertTo-SecureString -String "password" -Force -AsPlainText)

param(
    [Parameter(Mandatory=$true)]
    [string]$ExePath,

    [string]$CertPath,
    [string]$Password,
    [string]$Thumbprint,
    [string]$TimestampServer = "http://timestamp.digicert.com"
)

$ErrorActionPreference = "Stop"

Write-Host "PuttyAlt Code Signing" -ForegroundColor Cyan
Write-Host "=====================" -ForegroundColor Cyan

if (-not (Test-Path $ExePath)) {
    Write-Error "File not found: $ExePath"
    exit 1
}

$cert = $null

if ($CertPath) {
    if (-not (Test-Path $CertPath)) {
        Write-Error "Certificate not found: $CertPath"
        exit 1
    }
    $secPass = ConvertTo-SecureString -String $Password -Force -AsPlainText
    $cert = Get-PfxCertificate -FilePath $CertPath -Password $secPass
    Write-Host "Using certificate from file: $CertPath" -ForegroundColor Green
}
elseif ($Thumbprint) {
    $cert = Get-ChildItem -Path Cert:\CurrentUser\My\$Thumbprint -ErrorAction SilentlyContinue
    if (-not $cert) {
        $cert = Get-ChildItem -Path Cert:\LocalMachine\My\$Thumbprint -ErrorAction SilentlyContinue
    }
    if (-not $cert) {
        Write-Error "Certificate with thumbprint $Thumbprint not found"
        exit 1
    }
    Write-Host "Using certificate: $($cert.Subject)" -ForegroundColor Green
}
else {
    Write-Error "Specify either -CertPath or -Thumbprint"
    exit 1
}

Write-Host "Signing: $ExePath" -ForegroundColor Yellow
Write-Host "Timestamp: $TimestampServer" -ForegroundColor Yellow

try {
    $result = Set-AuthenticodeSignature -FilePath $ExePath -Certificate $cert `
        -TimestampServer $TimestampServer -HashAlgorithm SHA256

    if ($result.Status -eq "Valid") {
        Write-Host "`nSigning successful!" -ForegroundColor Green
        Write-Host "Status: $($result.Status)"
        Write-Host "Signer: $($result.SignerCertificate.Subject)"

        # Remove Zone.Identifier if present
        $adsPath = "${ExePath}:Zone.Identifier"
        if (Test-Path $adsPath -ErrorAction SilentlyContinue) {
            Remove-Item $adsPath -Force
            Write-Host "Removed Zone.Identifier ADS" -ForegroundColor Green
        }

        Write-Host "`nSmartScreen should no longer block this executable." -ForegroundColor Cyan
    }
    else {
        Write-Warning "Signing completed with status: $($result.Status)"
        Write-Warning $result.StatusMessage
    }
}
catch {
    Write-Error "Signing failed: $_"
    exit 1
}
