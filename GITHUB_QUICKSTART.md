# 🚀 Xeno GitHub Auto-Build - QUICKSTART

## ⚡ 5 Minut Setup

### 1️⃣ Utwórz GitHub Repo (30 sekund)
```
https://github.com/new
Nazwa: XenoExecutor-xeno
Visibility: Public/Private (Twój wybór)
Klik "Create repository"
```

### 2️⃣ Push kod (2 minuty)

```powershell
cd C:\Users\aio_01\Desktop\XenoExecutor-xeno

git remote add origin https://github.com/YOUR_USERNAME/XenoExecutor-xeno.git
git branch -M main

git add .
git commit -m "Initial commit with auto-offset system + CI/CD"
git push -u origin main
```

### 3️⃣ Czekaj na Build (2 minuty)

Idź na: https://github.com/YOUR_USERNAME/XenoExecutor-xeno/actions

Czekaj aż build się skończy ✅

### 4️⃣ Pobierz .exe (1 minuta)

```
Actions → Build Xeno
  ↓
Pobierz "Xeno-Release-x64" artifact
  ↓
Rozpakuj ZIP
  ↓
Masz Xeno.dll!
```

---

## 📦 Co Się Buduje

| Artefakt | Plik | Opis |
|----------|------|------|
| **Xeno.dll** | `Xeno\bin\Release\x64\` | Core executor |
| **Xeno.pdb** | `Xeno\bin\Release\x64\` | Debug symbols |
| **XenoUI.exe** | `XenoUI\bin\Release\` | GUI aplikacja |

---

## 🔄 Automatyczne Builde

Projekt buduje się **automatycznie** gdy:
- ✅ Push'nniesz kod na `main` branch
- ✅ Otwiersz Pull Request
- ✅ Manualne trigger (Actions tab → "Run workflow")

---

## 🛠️ Troubleshooting

### ❌ "Build Failed"
```
1. Klik Build job w GitHub Actions
2. Sprawdź ostatnią linię w logs
3. Zwykle to brakujące zależności
4. Fix lokalnie, potem push'uj ponownie
```

### ❌ "Artifacts not found"
```
→ BUILD się nie skończyć
→ Czekaj, potem spróbuj ponownie
```

### ❌ "403 Unauthorized"
```
→ SSH key issue
→ Albo użyj personal access token:
git remote set-url origin https://TOKEN@github.com/USER/REPO.git
```

---

## 📝 Workflow Files

| Plik | Opis |
|------|------|
| `.github/workflows/build.yml` | GitHub Actions config |
| `BUILD_GUIDE.md` | Pełna dokumentacja |
| `AUTO_OFFSET_SYSTEM.md` | Offset detekcja |

---

## ✨ Status Badge

Dodaj do README.md top:

```markdown
# Xeno Executor

[![Build Status](https://github.com/YOUR_USERNAME/XenoExecutor-xeno/workflows/Build%20Xeno/badge.svg)](https://github.com/YOUR_USERNAME/XenoExecutor-xeno/actions)

Roblox executor z GitHub CI/CD...
```

---

## 🎯 Teraz Możesz:

- ✅ Pushować kod → auto-builds
- ✅ Pobierać compiled .exe
- ✅ Robić Release builds 
- ✅ Track build history
- ✅ Share builds z zespołem

---

**Ready? Start z Step 1! 🚀**
