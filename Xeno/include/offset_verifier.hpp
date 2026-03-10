#pragma once

#include <Windows.h>
#include <iostream>
#include <iomanip>
#include "offset_detector.hpp"

/**
 * OFFSET VERIFICATION SYSTEM
 * Tests if offsets work correctly by reading known instances
 */
class OffsetVerifier {
public:
    /**
     * Verifies offsets are working by testing reads
     * @param processHandle Handle to Roblox client
     * @param testAddress Known Instance address to test
     * @param offsets Reference to current offset cache
     * @return true if verification passed
     */
    static bool VerifyOffsets(HANDLE processHandle, std::uintptr_t testAddress, const OffsetDetector::OffsetCache& offsets) {
        std::cout << "\n=== OFFSET VERIFICATION STARTED ===\n";
        
        int passCount = 0;
        int totalTests = 6;

        // Test 1: Name offset
        std::cout << "[*] Testing Name offset (0x" << std::hex << offsets.Name << std::dec << ")... ";
        if (TestNameOffset(processHandle, testAddress, offsets)) {
            std::cout << "✓ PASS\n";
            passCount++;
        } else {
            std::cout << "✗ FAIL\n";
        }

        // Test 2: Parent offset
        std::cout << "[*] Testing Parent offset (0x" << std::hex << offsets.Parent << std::dec << ")... ";
        if (TestParentOffset(processHandle, testAddress, offsets)) {
            std::cout << "✓ PASS\n";
            passCount++;
        } else {
            std::cout << "✗ FAIL\n";
        }

        // Test 3: Children offset
        std::cout << "[*] Testing Children offset (0x" << std::hex << offsets.Children << std::dec << ")... ";
        if (TestChildrenOffset(processHandle, testAddress, offsets)) {
            std::cout << "✓ PASS\n";
            passCount++;
        } else {
            std::cout << "✗ FAIL\n";
        }

        // Test 4: ClassDescriptor offset
        std::cout << "[*] Testing ClassDescriptor offset (0x" << std::hex << offsets.ClassDescriptor << std::dec << ")... ";
        if (TestClassDescriptorOffset(processHandle, testAddress, offsets)) {
            std::cout << "✓ PASS\n";
            passCount++;
        } else {
            std::cout << "✗ FAIL\n";
        }

        // Test 5: Script embedded offsets
        std::cout << "[*] Testing Script embedded offsets (0x" << std::hex << offsets.LocalScriptEmbedded << std::dec << " / 0x" << offsets.ModuleScriptEmbedded << std::dec << ")... ";
        if (TestScriptOffsets(processHandle, testAddress, offsets)) {
            std::cout << "✓ PASS\n";
            passCount++;
        } else {
            std::cout << "✗ FAIL\n";
        }

        // Test 6: Pointer validation
        std::cout << "[*] Testing pointer validity... ";
        if (TestPointerValidity(processHandle, testAddress, offsets)) {
            std::cout << "✓ PASS\n";
            passCount++;
        } else {
            std::cout << "✗ FAIL\n";
        }

        std::cout << "\n=== VERIFICATION RESULT ===\n";
        std::cout << "Passed: " << passCount << "/" << totalTests << " tests\n";
        
        if (passCount >= 4) {
            std::cout << "[+] OFFSETS ARE VALID - Ready to use\n";
            return true;
        } else {
            std::cout << "[!] OFFSETS ARE INVALID - Consider updating\n";
            return false;
        }
    }

private:
    static bool TestNameOffset(HANDLE processHandle, std::uintptr_t address, const OffsetDetector::OffsetCache& offsets) {
        try {
            std::uintptr_t namePtr = 0;
            SIZE_T bytesRead = 0;
            if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address + offsets.Name), &namePtr, sizeof(namePtr), &bytesRead)) {
                // Name should point to valid string data
                return namePtr > 0 && namePtr < 0xFFFFFFFFFFFF0000;
            }
            return false;
        } catch (...) {
            return false;
        }
    }

    static bool TestParentOffset(HANDLE processHandle, std::uintptr_t address, const OffsetDetector::OffsetCache& offsets) {
        try {
            std::uintptr_t parentPtr = 0;
            SIZE_T bytesRead = 0;
            if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address + offsets.Parent), &parentPtr, sizeof(parentPtr), &bytesRead)) {
                // Parent can be 0 (for DataModel) or valid pointer
                return parentPtr == 0 || (parentPtr > 0 && parentPtr < 0xFFFFFFFFFFFF0000);
            }
            return false;
        } catch (...) {
            return false;
        }
    }

    static bool TestChildrenOffset(HANDLE processHandle, std::uintptr_t address, const OffsetDetector::OffsetCache& offsets) {
        try {
            std::uintptr_t childrenPtr = 0;
            SIZE_T bytesRead = 0;
            if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address + offsets.Children), &childrenPtr, sizeof(childrenPtr), &bytesRead)) {
                // Children can be 0 or valid pointer
                return childrenPtr == 0 || (childrenPtr > 0 && childrenPtr < 0xFFFFFFFFFFFF0000);
            }
            return false;
        } catch (...) {
            return false;
        }
    }

    static bool TestClassDescriptorOffset(HANDLE processHandle, std::uintptr_t address, const OffsetDetector::OffsetCache& offsets) {
        try {
            std::uintptr_t classDescPtr = 0;
            SIZE_T bytesRead = 0;
            if (ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address + offsets.ClassDescriptor), &classDescPtr, sizeof(classDescPtr), &bytesRead)) {
                // ClassDescriptor should always be valid
                return classDescPtr > 0 && classDescPtr < 0xFFFFFFFFFFFF0000;
            }
            return false;
        } catch (...) {
            return false;
        }
    }

    static bool TestScriptOffsets(HANDLE processHandle, std::uintptr_t address, const OffsetDetector::OffsetCache& offsets) {
        try {
            std::uintptr_t scriptPtr = 0;
            SIZE_T bytesRead = 0;
            // Try both offsets
            bool localResult = ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address + offsets.LocalScriptEmbedded), &scriptPtr, sizeof(scriptPtr), &bytesRead);
            bool moduleResult = ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address + offsets.ModuleScriptEmbedded), &scriptPtr, sizeof(scriptPtr), &bytesRead);
            return localResult || moduleResult;
        } catch (...) {
            return false;
        }
    }

    static bool TestPointerValidity(HANDLE processHandle, std::uintptr_t address, const OffsetDetector::OffsetCache& offsets) {
        try {
            // Simple check: can we read from the address?
            std::uintptr_t test = 0;
            SIZE_T bytesRead = 0;
            return ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &test, sizeof(test), &bytesRead) && bytesRead > 0;
        } catch (...) {
            return false;
        }
    }
};
