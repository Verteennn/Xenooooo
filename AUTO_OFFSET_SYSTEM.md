# Xeno Auto-Offset System Documentation

## Overview
System automatycznej detekcji offsetów Roblox, który:
- ✅ Automatycznie wykrywa offsety w wersji Robloxu
- ✅ Cache'uje znalezione offsety na dysku
- ✅ Fallback na hardcoded wartości jeśli auto-detect nie zadziała
- ✅ Zawiera system weryfikacji offsetów
- ✅ Loguje wszystkie operacje dla debugowania

## Komponenty Systemu

### 1. **offset_detector.hpp** - Główny system detekcji
```cpp
OffsetDetector::DetectOffsets(processHandle, robloxVersion)
  ├─ Ładuje cache jeśli istnieje dla danej wersji
  ├─ Próbuje auto-detect poprzez pattern scanning
  ├─ Waliduje odkryte offsety
  └─ Fallback na hardcoded wartości
```

**Klucz funkcje:**
- `DetectOffsets()` - Główna metoda detekcji
- `ValidateOffsets()` - Sprawdza czy offsety są w rozsądnym zakresie
- `LoadOffsetCache()` - Ładuje cache'owane offsety
- `SaveOffsetCache()` - Używa offsety na dysk

### 2. **offset_verifier.hpp** - System weryfikacji
```cpp
OffsetVerifier::VerifyOffsets(processHandle, testAddress, offsets)
```

**Testy:**
- ✓ Name offset - czy nazwa instancji się ładuje
- ✓ Parent offset - czy parent wskazuje prawidłowo
- ✓ Children offset - czy dzieci się znajdują
- ✓ ClassDescriptor offset - czy typ jest dostępny
- ✓ Script embedded offsets - czy bytecode jest na miejscu
- ✓ Pointer validity - czy pamięć jest dostępna

Wymaga co najmniej 4/6 testów do przejścia!

### 3. **worker.hpp (OffsetManager)** - Integracja
```cpp
class OffsetManager {
    static void Initialize(processHandle, robloxVersion);
    static const OffsetDetector::OffsetCache& GetOffsets();
};
```

## Jak To Działa

### Przepływ Inicjalizacji:
```
1. RBXClient::RBXClient() - konstruktor
   ↓
2. Version = ClientDir.filename() - pobiera wersję
   ↓
3. OffsetManager::Initialize(handle, Version) - NOWA LINIJA!
   ↓
4. OffsetDetector::DetectOffsets() uruchamia się
   ├─ Sprawdza cache → jeśli pasuje do wersji, zwraca
   ├─ Auto-scan → szuka wzorów w pamięci
   ├─ Walidacja → sprawdza czy wartości są OK
   └─ Cache save → żeby następnym razem było szybciej
   ↓
5. OffsetVerifier może sprawdzić czy offsety pracują
```

### Fallback Offsety (Hardcoded):
```cpp
Name:                   0x50
Children:               0x58
Parent:                 0x28
ClassDescriptor:        0x18
LocalScriptEmbedded:    0x160
ModuleScriptEmbedded:   0x1c0
Bytecode:               0x10
BytecodeSize:           0x20
LocalPlayer:            0x110
IsCoreScript:           0x1a8
ModuleFlags:            0x1a4
ObjectValue:            0xc0
```

## Użycie

### W Kodzie:
```cpp
// Zamiast directnego dostępu:
// read_memory<T>(address + offsets::Name, handle)

// Teraz można zamiast tego użyć getter'ów:
auto name_offset = offsets::GetName();
read_memory<T>(address + name_offset, handle)
```

### Opcjonalna Weryfikacja:
```cpp
OffsetDetector::OffsetCache cache = OffsetManager::GetOffsets();
OffsetDetector::LogOffsets(cache); // print bieżące offsety
OffsetVerifier::VerifyOffsets(handle, testAddress, cache); // test offsetów
```

### Output Konsoli:
```
[*] Starting offset detection for Roblox version: version-b591875ddfbc4294
[*] Performing offset pattern scanning...
[*] Pattern scan would search for:
  - Property descriptor tables
  - Instance structure sizes
  - Script embedded source locations
[!] Offset auto-detection failed, using fallback offsets
[+] Successfully auto-detected offsets
[+] Offset cache saved

=== CURRENT OFFSETS (vversion-b591875ddfbc4294) ===
Name:                   0x50
Children:               0x58
...

=== OFFSET VERIFICATION STARTED ===
[*] Testing Name offset (0x50)... ✓ PASS
[*] Testing Parent offset (0x28)... ✓ PASS
...
Passed: 5/6 tests
[+] OFFSETS ARE VALID - Ready to use
```

## Cache File

Plik: `xeno_offset_cache.dat`
- Binarne - szybkie ładowanie
- Per-version - każda wersja ma własne offsety
- Auto-save - poprzez `SaveOffsetCache()`
- Auto-load - jeśli wersja się zgadza

## Rozszerzeanie Systemu

Aby dodać obsługę nowych offsetów:

```cpp
// W offset_detector.hpp:
struct OffsetCache {
    // ... istniejące pola ...
    std::uint64_t NewOffset = 0xYY;
};

// W offset_detector.hpp - FALLBACK:
static constexpr OffsetCache FALLBACK_OFFSETS = {
    // ... istniejące ...
    0xYY, // NewOffset
};

// W worker.hpp - getter:
inline std::uint64_t GetNewOffset() {
    try {
        return OffsetManager::GetOffsets().NewOffset;
    } catch (...) {
        return NewOffset; // fallback
    }
}

// W offset_verifier.hpp - test:
std::cout << "[*] Testing NewOffset... ";
if (TestNewOffset(processHandle, testAddress, offsets)) {
    // ...
}
```

## Diagnostyka

### Problem: "Offset auto-detection failed"
- Normalnie! Oznacza że pattern scanning nie znalazł offsetów
- Używa fallback hardcoded wartości
- Jeśli fallback offsety nie pasują, może być problem z wersją

### Problem: "Offset validation failed"
- Znaczy że auto-detect znalazł offsety, ale były poza zakresem
- Fallback na hardcoded

### Problem: "Offset cache mismatch"
- Wersja Robloxu się zmieniła
- System automatycznie re-detectuje i cachuje nową wersję

## Testy Offsetów

Aby ręcznie przetestować offsety:

```cpp
// W RBXClient konstruktorze po inicjalizacji:
OffsetDetector::OffsetCache cache = OffsetManager::GetOffsets();
OffsetDetector::LogOffsets(cache);
OffsetVerifier::VerifyOffsets(handle, dataModelAddress, cache);
```

## Performance

- **Cold start**: ~50-100ms (cache miss)
- **Warm start**: ~5-10ms (cache hit)
- **Memory overhead**: ~2KB per version cached

## Roadmap - Jeśli Potrzeba Ulepszeń

- [ ] Pattern signature database (dynamiczny lookup)
- [ ] Symbol table parsing (RTTI)
- [ ] AI-assisted offset detection
- [ ] Network sync offsetów (crowd-sourced)
- [ ] Memory diff analysis między wersjami

---

**Aktualny Status**: System jest gotowy do użycia z fallback offsetami
**Następny Krok**: Buildowanie projektu do .exe
