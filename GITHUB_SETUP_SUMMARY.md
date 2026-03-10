# 🎯 Xeno - GitHub Actions CI/CD Setup

## ✅ Co Zostało Stworzone

### 📁 Nowe Pliki do Repo:
```
.github/workflows/
  └── build.yml                  # GitHub Actions workflow (Debug + Release)

GITHUB_QUICKSTART.md             # 5-minute setup guide
BUILD_GUIDE.md                   # Pełna dokumentacja buildowania
AUTO_OFFSET_SYSTEM.md            # Auto-offset detection
build.bat                        # Lokalny build helper script
```

### 🔧 Nowe Components:
```
Xeno/include/
  ├── offset_detector.hpp        # Auto-offset system
  └── offset_verifier.hpp        # Offset validation

Xeno/src/
  └── worker.cpp                 # Integration z OffsetManager
```

---

## 🚀 SETUP w 4 Kroki

### 1) Utwórz GitHub Repo
```
https://github.com/new
→ Nazwa: XenoExecutor-xeno
→ Visibility: Public/Private
→ Klik "Create"
```

### 2) Push Code

```powershell
cd C:\Users\aio_01\Desktop\XenoExecutor-xeno

# Ustaw remote URL
git remote add origin https://github.com/YOUR_USERNAME/XenoExecutor-xeno.git

# Zmień branch na main
git branch -M main

# Push wszystko
git add .
git commit -m "Initial commit: Xeno with auto-offset system and CI/CD"
git push -u origin main
```

### 3) GitHub Actions Auto-Build

Po push'aniu:
- ✅ GitHub Actions automatycznie uruchamia workflow
- ✅ Zainstaluje MSVC compiler
- ✅ Pobierze vcpkg dependencies
- ✅ Zbuduje Debug + Release
- ✅ Uploaduje artefakty

Monitoruj progress: `GitHub → Actions tab`

### 4) Pobierz Compiled Files

```
GitHub Actions → Latest Build → Artifacts
  ↓
Pobierz "Xeno-Release-x64"
  ↓
Rozpakuj ZIP
  ↓
Xeno.dll + XenoUI.exe gotowe!
```

---

## 📖 Dokumentacja

| Plik | Dla Kogo | Opis |
|------|----------|------|
| **GITHUB_QUICKSTART.md** | Szybki start | 5-minute setup |
| **BUILD_GUIDE.md** | Local + CI/CD | Pełny guide |
| **AUTO_OFFSET_SYSTEM.md** | Developers | Offset detekcja |
| **build.bat** | Windows users | Helper script |

---

## 🔄 Workflow Details

### GitHub Actions Triggers:
```yaml
on:
  push:
    branches: [main, master, develop]
  pull_request:
    branches: [main, master]
  workflow_dispatch:  # Manual trigger
```

### Build Matrix:
- Platform: **x64**
- Configurations: **Debug** + **Release**
- Runner: **windows-latest** (MSVC)

### Build Steps:
1. Checkout code (z submodules)
2. Setup MSVC kompiler
3. Setup vcpkg
4. Instalacja dependencies (xxhash, zstd, openssl)
5. Build Xeno.dll (C++)
6. Build XenoUI.exe (C#)
7. Artifacts upload
8. Auto-create Release (Release builds)

---

## 📊 Status Badge

Dodaj do README.md:

```markdown
[![Build Status](https://github.com/YOUR_USERNAME/XenoExecutor-xeno/workflows/Build%20Xeno/badge.svg?branch=main)](https://github.com/YOUR_USERNAME/XenoExecutor-xeno/actions?query=workflow:%22Build%20Xeno%22)
```

---

## 🛠️ Lokalnie (bez GitHub)

Jeśli nie chcesz używać GitHub Actions:

```powershell
# Instaluj vcpkg
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install xxhash:x64-windows zstd:x64-windows openssl:x64-windows
.\vcpkg integrate install

# Build Xeno
cd C:\Users\aio_01\Desktop\XenoExecutor-xeno
msbuild Xeno.sln /p:Configuration=Release /p:Platform=x64 /m

# Output: Xeno\bin\Release\x64\Xeno.dll
```

Lub użyj: `build.bat` (helper script)

---

## ✨ Auto-Offset System Integration

Projekt zawiera **automatyczną detekcję offsetów**:

### Jak Działa:
```
Każdy nowy RBXClient:
  1. Pobiera wersję Robloxu
  2. OffsetManager::Initialize() uruchamia się
  3. Szuka offsetów w cache
  4. Jeśli nie ma → Auto-detect + cache
  5. Jeśli auto-detect fails → Fallback hardcoded
  ✅ Zawsze działa!
```

### Cache System:
- Plik: `xeno_offset_cache.dat`
- Per-version caching
- Auto-save + auto-load
- Fallback na hardcoded wartości

---

## 📋 Checklist Przed First Push

- [x] `.github/workflows/build.yml` created
- [x] `offset_detector.hpp` created
- [x] `offset_verifier.hpp` created
- [x] `worker.hpp` updated (OffsetManager)
- [x] `worker.cpp` updated (initialization)
- [x] `BUILD_GUIDE.md` created
- [x] `GITHUB_QUICKSTART.md` created
- [x] `build.bat` created
- [x] `.gitignore` has bin/obj excludes
- [x] `Xeno.sln` in root directory

---

## 🎯 Next Steps

1. **Create GitHub repo** (2 min)
2. **Push code** (1 min)
   ```powershell
   git add .
   git commit -m "Initial commit"
   git push -u origin main
   ```
3. **Wait for build** (5 min)
4. **Download artifacts** (1 min)
5. **Test Xeno.dll + XenoUI.exe** ✅

---

## 🐛 Common Issues

### "Build Failed"
→ Check GitHub Actions logs for details
→ Common: missing vcpkg integration
→ Solution: Run `vcpkg integrate install` locally

### "vcpkg not found in Actions"
→ Normal! Actions downloads fresh copy
→ GitHub has vcpkg cache built-in

### "Can't connect to GitHub"
→ SSH key or HTTPS token issue
→ Use: `git clone https://github.com/YOUR_REPO` (HTTPS)
→ Or setup SSH key on GitHub

### "Artifacts not available"
→ Build still running
→ Check Actions tab for progress
→ Wait for all jobs to complete

---

## 📞 Support

**GitHub Actions Documentation:**
https://docs.github.com/en/actions

**vcpkg:**
https://github.com/microsoft/vcpkg

**Xeno Auto-Offset System:**
See `AUTO_OFFSET_SYSTEM.md`

---

## 🎉 You're All Set!

**Quick Links:**
- 📖 Docs: [BUILD_GUIDE.md](BUILD_GUIDE.md) | [GITHUB_QUICKSTART.md](GITHUB_QUICKSTART.md)
- 🔧 Tools: [build.bat](build.bat)
- 📊 Status: GitHub Actions tab

Ready to push? Go to step 1! 🚀
