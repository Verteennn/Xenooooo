#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Xeno GitHub Push Helper - Automated setup for GitHub Actions CI/CD

.DESCRIPTION
    Helps you push Xeno project to GitHub with proper configuration

.EXAMPLE
    .\github_setup.ps1
#>

param(
    [string]$Username = "",
    [string]$RepoName = "XenoExecutor-xeno",
    [string]$Branch = "main"
)

# Colors for output
$colors = @{
    Success = "Green"
    Error   = "Red"
    Info    = "Cyan"
    Warning = "Yellow"
}

function Write-Info {
    param([string]$Message)
    Write-Host "[*] $Message" -ForegroundColor $colors.Info
}

function Write-Success {
    param([string]$Message)
    Write-Host "[+] $Message" -ForegroundColor $colors.Success
}

function Write-Error {
    param([string]$Message)
    Write-Host "[-] $Message" -ForegroundColor $colors.Error
}

function Write-Warning {
    param([string]$Message)
    Write-Host "[!] $Message" -ForegroundColor $colors.Warning
}

function Show-Header {
    Clear-Host
    Write-Host "=========================================" -ForegroundColor Cyan
    Write-Host "   Xeno GitHub Push Helper" -ForegroundColor Cyan
    Write-Host "=========================================" -ForegroundColor Cyan
    Write-Host ""
}

function Check-Prerequisites {
    Write-Info "Checking prerequisites..."
    
    # Check Git
    $gitExists = Get-Command git -ErrorAction SilentlyContinue
    if (-not $gitExists) {
        Write-Error "Git not found! Install from https://git-scm.com/download/win"
        return $false
    }
    Write-Success "Git found"
    
    # Check if in repository
    $gitStatus = git status 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Not in a Git repository!"
        return $false
    }
    Write-Success "Git repository detected"
    
    return $true
}

function Get-GitHub-Credentials {
    Write-Host ""
    Write-Info "GitHub Setup Information"
    
    if ([string]::IsNullOrEmpty($Username)) {
        $Username = Read-Host "GitHub Username"
    }
    
    Write-Host ""
    Write-Host "Repository name: $RepoName"
    $changeRepo = Read-Host "Change repository name? (y/n)" 
    if ($changeRepo -eq "y") {
        $RepoName = Read-Host "New repository name"
    }
    
    return @{
        Username = $Username
        RepoName = $RepoName
        RepoUrl = "https://github.com/$Username/$RepoName.git"
        Branch = $Branch
    }
}

function Setup-Git-Config {
    param($Creds)
    
    Write-Info "Configuring Git repository..."
    
    # Check if remote exists
    $remotes = git remote
    if ($remotes -contains "origin") {
        Write-Warning "Remote 'origin' already exists"
        $replace = Read-Host "Replace it? (y/n)"
        if ($replace -eq "y") {
            git remote remove origin
            Write-Success "Removed old remote"
        } else {
            Write-Warning "Skipping remote setup"
            return
        }
    }
    
    # Add new remote
    Write-Info "Adding remote: $($Creds.RepoUrl)"
    git remote add origin $Creds.RepoUrl
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "Remote 'origin' added"
    } else {
        Write-Error "Failed to add remote"
        return $false
    }
    
    # Check/create branch
    $currentBranch = git rev-parse --abbrev-ref HEAD
    if ($currentBranch -ne $Creds.Branch) {
        Write-Info "Switching to branch '$($Creds.Branch)'..."
        git branch -M $Creds.Branch
        Write-Success "Branch renamed to '$($Creds.Branch)'"
    }
    
    return $true
}

function Show-Status {
    Write-Info "Git status:"
    Write-Host ""
    git status --short | ForEach-Object {
        Write-Host "  $_"
    }
    Write-Host ""
}

function Confirm-Push {
    Write-Warning "Review changes above before pushing!"
    Write-Host ""
    
    $files = git status --short | Measure-Object
    Write-Info "Files to commit: $($files.Count)"
    
    $confirm = Read-Host "Proceed with commit and push? (y/n)"
    return ($confirm -eq "y")
}

function Perform-Commit {
    Write-Info "Creating commit..."
    
    git add .
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Failed to stage files"
        return $false
    }
    
    $message = "Initial commit: Xeno with auto-offset system and GitHub Actions CI/CD"
    Write-Info "Commit message: $message"
    
    git commit -m $message
    if ($LASTEXITCODE -eq 0) {
        Write-Success "Commit created"
        return $true
    } else {
        Write-Error "Commit failed"
        return $false
    }
}

function Perform-Push {
    param($Creds)
    
    Write-Info "Pushing to GitHub..."
    Write-Host ""
    
    git push -u origin $Creds.Branch
    
    if ($LASTEXITCODE -eq 0) {
        Write-Success "Pushed successfully!"
        Write-Host ""
        Write-Success "Repository: https://github.com/$($Creds.Username)/$($Creds.RepoName)"
        Write-Success "Branch: $($Creds.Branch)"
        return $true
    } else {
        Write-Error "Push failed!"
        Write-Warning "Common solutions:"
        Write-Warning "1. Create repo at: https://github.com/new"
        Write-Warning "2. Setup SSH keys: https://github.com/settings/keys"
        Write-Warning "3. Use personal token instead of password"
        return $false
    }
}

function Show-Next-Steps {
    param($Creds)
    
    Write-Host ""
    Write-Host "=========================================" -ForegroundColor Green
    Write-Host "   ✓ Setup Complete!" -ForegroundColor Green
    Write-Host "=========================================" -ForegroundColor Green
    Write-Host ""
    
    Write-Info "Next steps:"
    Write-Host ""
    Write-Host "1. GitHub Actions auto-builds your code"
    Write-Host "   • Wait 5 minutes for build to complete"
    Write-Host "   • Monitor at: GitHub Actions tab"
    Write-Host ""
    
    Write-Host "2. Download compiled files"
    Write-Host "   • Go to: Actions → Latest Build"
    Write-Host "   • Download 'Xeno-Release-x64' artifact"
    Write-Host "   • Extract ZIP → Use Xeno.dll + XenoUI.exe"
    Write-Host ""
    
    Write-Host "3. Documentation"
    Write-Host "   • GITHUB_QUICKSTART.md - Quick reference"
    Write-Host "   • BUILD_GUIDE.md - Full documentation"
    Write-Host "   • AUTO_OFFSET_SYSTEM.md - Offset system"
    Write-Host ""
    
    Write-Host "Your repository:"
    Write-Host "   $($Creds.RepoUrl)" -ForegroundColor Cyan
    Write-Host ""
}

# Main execution
function Main {
    Show-Header
    
    # Check prerequisites
    if (-not (Check-Prerequisites)) {
        Write-Error "Prerequisites check failed!"
        Read-Host "Press Enter to exit"
        return
    }
    
    # Get credentials
    $creds = Get-GitHub-Credentials
    Write-Host ""
    
    # Setup Git config
    if (-not (Setup-Git-Config $creds)) {
        Write-Error "Git configuration failed!"
        Read-Host "Press Enter to exit"
        return
    }
    
    # Show what will be committed
    Write-Host ""
    Show-Status
    
    # Confirm before push
    if (-not (Confirm-Push)) {
        Write-Warning "Push cancelled"
        Read-Host "Press Enter to exit"
        return
    }
    
    # Perform commit
    Write-Host ""
    if (-not (Perform-Commit)) {
        Write-Error "Commit failed!"
        Read-Host "Press Enter to exit"
        return
    }
    
    # Perform push
    Write-Host ""
    if (-not (Perform-Push $creds)) {
        Write-Error "Push failed!"
        Read-Host "Press Enter to exit"
        return
    }
    
    # Show next steps
    Show-Next-Steps $creds
    
    Write-Warning "Press Enter to exit"
    Read-Host ""
}

Main
