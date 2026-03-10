# Xeno Build Guide - GitHub Actions + Local

## 🚀 Build za pomocą GitHub Actions (Recommended)

### Krok 1: Utwórz GitHub Repo (nie potrzebujesz lokalnie)

```bash
# Go to https://github.com/new
# Utwórz repo: XenoExecutor-xeno
# wyber: Public (jeśli chcesz by wszyscy widzieli builda)
```

### Krok 2: Push kodu na GitHub

```powershell
cd C:\Users\aio_01\Desktop\XenoExecutor-xeno

# Dodaj URL remote (zamień USER/REPO na swoje)
git remote add origin https://github.com/YOUR_USERNAME/XenoExecutor-xeno.git
git branch -M main

# Pushuj kod
git add .
git commit -m "Initial commit with auto-offset system"
git push -u origin main
```

### Krok 3: GitHub Actions Automatycznie Buduje

Po push'aniu:
1. Idź na https://github.com/YOUR_USERNAME/XenoExecutor-xeno
2. Klik "Actions" tab
3. Automatycznie zacznie się build ✨

**Czego build robi:**
- ✓ Instaluje MSVC compiler
- ✓ Pobiera vcpkg i zależności (xxhash, zstd, openssl)
- ✓ Kompiluje Xeno.dll (C++)
- ✓ Kompiluje XenoUI.exe (C#)
- ✓ Release + Debug warianty
- ✓ Uploaduje artefakty

### Krok 4: Pobierz Skompilowany .exe

```
GitHub -> Actions -> Najnowszy Build
  ↓
Artifacts section
  ↓
Pobierz "Xeno-Release-x64"
  ↓
Rozpakuj ZIP
  ↓
Masz Xeno.dll + XenoUI.exe!
```

---

## 💻 Lokalny Build (bez GitHub)

### Wymagania:
- Visual Studio 2022 Community (Free)
- .NET 8.0 SDK (dla XenoUI)
- vcpkg

### Instalacja vcpkg:

```powershell
# Pobierz vcpkg
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Build vcpkg
.\bootstrap-vcpkg.bat

# Zainstaluj zależności
.\vcpkg install xxhash:x64-windows zstd:x64-windows openssl:x64-windows

# Integrate z Project
.\vcpkg integrate install
```

### Build Xeno C++ (MSVC):

```powershell
cd C:\Users\aio_01\Desktop\XenoExecutor-xeno

# Otwórz sln w Visual Studio 2022
# LUB buduj z terminala:

msbuild Xeno.sln /p:Configuration=Release /p:Platform=x64

# Output: Xeno\bin\Release\x64\Xeno.dll
```

### Build XenoUI C#:

```powershell
cd C:\Users\aio_01\Desktop\XenoExecutor-xeno\XenoUI

# Albo w Visual Studio klik Build
# Albo z terminala:

dotnet build XenoUI.csproj -c Release

# Output: XenoUI\bin\Release\*\XenoUI.exe
```

---

## 📋 GitHub Actions Workflow Status

Plik: `.github/workflows/build.yml`

**Triggery (kiedy się builduje):**
- ✓ Push na `main` branch
- ✓ Push na `develop` branch
- ✓ Pull Requests
- ✓ Manual trigger (Workflow Dispatch)

**Artefakty:**
- `Xeno-Debug-x64` (30 dni retencji)
- `Xeno-Release-x64` (30 dni retencji)

**Releases (auto-create):**
- Release build on push to main → Automatycznie tworzy Release

---

## 🔧 Build Output Struktura

```
Po buildie:
├── Xeno/
│   └── bin/
│       ├── Debug/x64/
│       │   ├── Xeno.dll
│       │   └── Xeno.pdb
│       └── Release/x64/
│           ├── Xeno.dll
│           └── Xeno.pdb
│
└── XenoUI/
    └── bin/
        ├── Debug/
        │   └── XenoUI.exe
        └── Release/
            └── XenoUI.exe
```

---

## ✅ Checklist Przed Push'em

- [ ] `.github/workflows/build.yml` istnieje
- [ ] `Xeno.sln` jest w root
- [ ] `vcpkg.json` jest w root
- [ ] `.gitignore` ma C++/C# excludes
- [ ] Nie ma `bin/` i `obj/` folderów (w .gitignore)
- [ ] Kod kompiluje się lokalnie (opcjonalnie)

---

## 🚢 Deployment

### Opcja 1: Download ze GitHub Actions (EASIEST)
```
GitHub Actions → Download artifact → Koniec
```

### Opcja 2: Download ze Releases
```
GitHub → Releases → Pobierz .zip → Koniec
```

### Opcja 3: GitHub Releases API
```powershell
# Pobierz najnowszy release
$release = Invoke-RestMethod `
  -Uri "https://api.github.com/repos/YOUR_USERNAME/XenoExecutor-xeno/releases/latest"
  
Invoke-WebRequest -Uri $release.assets[0].browser_download_url `
  -OutFile "Xeno-Latest.zip"
```

---

## 🐛 Troubleshooting

### Build fails: "vcpkg not found"
```
→ GitHub Actions ma vcpkg w workflow, powinno działać
→ Lokalnie: Zainstaluj vcpkg i integratuj
```

### Build fails: "msbuild not found"
```
→ Zainstaluj Visual Studio 2022 Build Tools
→ LUB: Używaj GitHub Actions (automatycznie)
```

### Build fails: "xxhash/zstd/openssl not found"
```
→ vcpkg install xxhash:x64-windows zstd:x64-windows openssl:x64-windows
→ vcpkg integrate install
```

### Artifacts nie uploadsują się
```
→ Check Actions tab dla job logs
→ Sprawdź paths: Xeno/bin/Release/x64/ istnieje?
```

---

## 📊 CI/CD Status Badge

Dodaj do README.md:

```markdown
[![Build Status](https://github.com/YOUR_USERNAME/XenoExecutor-xeno/workflows/Build%20Xeno/badge.svg)](https://github.com/YOUR_USERNAME/XenoExecutor-xeno/actions)
```

Output: ![Build Status Badge]

---

## 🎯 Następne Kroki

1. **Push code na GitHub**
   ```powershell
   git add .
   git commit -m "Add GitHub Actions workflow"
   git push -u origin main
   ```

2. **Czekaj na build** (~5 minut)

3. **Download artifacts** lub używaj Release

4. **Testuj Xeno.dll + XenoUI.exe**

---

## ✨ Bonus: Automatyczne Release Notes

Workflow auto-generuje Release Notes z:
- Commit hash
- Timestamp
- Debug/Release warianty

Wszystko w zakładce "Releases"!

---

**Pytania?** Sprawdź GitHub Actions logs dla szczegółów lub pkomenduj się!
