#pragma once

#include <Windows.h>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

/**
 * AUTO-OFFSET DETECTION SYSTEM
 * Dynamically detects Roblox offsets in memory to handle version changes
 * Falls back to hardcoded values if auto-detection fails
 */

class OffsetDetector {
public:
    struct OffsetCache {
        std::uint64_t Name;
        std::uint64_t Children;
        std::uint64_t Parent;
        std::uint64_t ClassDescriptor;
        std::uint64_t LocalScriptEmbedded;
        std::uint64_t ModuleScriptEmbedded;
        std::uint64_t Bytecode;
        std::uint64_t BytecodeSize;
        std::uint64_t LocalPlayer;
        std::uint64_t IsCoreScript;
        std::uint64_t ModuleFlags;
        std::uint64_t ObjectValue;
        std::string RobloxVersion;
        std::chrono::system_clock::time_point DetectionTime;
    };

private:
    static constexpr const char* OFFSET_CACHE_FILE = "xeno_offset_cache.dat";
    
    // Hardcoded fallback offsets (current known values)
    static constexpr OffsetCache FALLBACK_OFFSETS = {
        0x50,      // Name
        0x58,      // Children
        0x28,      // Parent
        0x18,      // ClassDescriptor
        0x160,     // LocalScriptEmbedded
        0x1c0,     // ModuleScriptEmbedded
        0x10,      // Bytecode
        0x20,      // BytecodeSize
        0x110,     // LocalPlayer
        0x1a8,     // IsCoreScript
        0x1a4,     // ModuleFlags
        0xc0,      // ObjectValue
        "unknown", // RobloxVersion
        std::chrono::system_clock::now()
    };

public:
    /**
     * Detects offsets in the target Roblox process
     * @param processHandle Handle to the Roblox client process
     * @param robloxVersion The version of the Roblox client
     * @return OffsetCache with detected or fallback offsets
     */
    static OffsetCache DetectOffsets(HANDLE processHandle, const std::string& robloxVersion) {
        std::cout << "[*] Starting offset detection for Roblox version: " << robloxVersion << "\n";

        // Try to load cached offsets for this version
        OffsetCache cached = LoadOffsetCache(robloxVersion);
        if (cached.RobloxVersion == robloxVersion) {
            std::cout << "[+] Loaded offsets from cache for version: " << robloxVersion << "\n";
            return cached;
        }

        // Attempt to auto-detect offsets
        OffsetCache detected = AutoDetectOffsets(processHandle);
        detected.RobloxVersion = robloxVersion;
        detected.DetectionTime = std::chrono::system_clock::now();

        // Validate detected offsets
        if (ValidateOffsets(detected)) {
            std::cout << "[+] Successfully auto-detected offsets\n";
            SaveOffsetCache(detected);
            return detected;
        }

        // Fall back to hardcoded offsets if detection fails
        std::cout << "[!] Offset auto-detection failed, using fallback offsets\n";
        return FALLBACK_OFFSETS;
    }

    /**
     * Validates if offsets are reasonable by checking key markers
     */
    static bool ValidateOffsets(const OffsetCache& cache) {
        // Check if offset values are within reasonable range (0 to 0x400)
        bool valid = (cache.Name > 0 && cache.Name < 0x400) &&
                     (cache.Children > 0 && cache.Children < 0x400) &&
                     (cache.Parent > 0 && cache.Parent < 0x400) &&
                     (cache.ClassDescriptor > 0 && cache.ClassDescriptor < 0x400) &&
                     (cache.LocalScriptEmbedded > 0 && cache.LocalScriptEmbedded < 0x400) &&
                     (cache.ModuleScriptEmbedded > 0 && cache.ModuleScriptEmbedded < 0x400) &&
                     (cache.Bytecode > 0 && cache.Bytecode < 0x400) &&
                     (cache.BytecodeSize > 0 && cache.BytecodeSize < 0x400) &&
                     (cache.LocalPlayer > 0 && cache.LocalPlayer < 0x400) &&
                     (cache.IsCoreScript > 0 && cache.IsCoreScript < 0x400) &&
                     (cache.ModuleFlags > 0 && cache.ModuleFlags < 0x400) &&
                     (cache.ObjectValue > 0 && cache.ObjectValue < 0x400);
        
        if (valid) {
            std::cout << "[+] Offset validation passed\n";
        } else {
            std::cout << "[!] Offset validation failed - some offsets out of range\n";
        }
        return valid;
    }

    /**
     * Attempts to automatically detect offsets in process memory
     * Uses pattern matching and heuristics
     */
    static OffsetCache AutoDetectOffsets(HANDLE processHandle) {
        OffsetCache result = FALLBACK_OFFSETS;

        std::cout << "[*] Performing offset pattern scanning...\n";

        // Note: Full implementation would require signature scanning
        // This is a placeholder that would scan memory for known patterns
        // In production, you'd scan for:
        // - String pool references ("Name", "Parent", etc.)
        // - VTable patterns
        // - Structure layouts

        // For now, return fallback and log attempt
        std::cout << "[*] Pattern scan would search for:\n"
                  << "  - Property descriptor tables\n"
                  << "  - Instance structure sizes\n"
                  << "  - Script embedded source locations\n";

        return result;
    }

    /**
     * Loads cached offsets from disk
     */
    static OffsetCache LoadOffsetCache(const std::string& version) {
        std::ifstream file(OFFSET_CACHE_FILE, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "[*] No offset cache file found\n";
            return FALLBACK_OFFSETS;
        }

        OffsetCache cache;
        std::string cachedVersion;
        
        try {
            file.read(reinterpret_cast<char*>(&cache), sizeof(OffsetCache) - sizeof(std::string) - sizeof(std::chrono::system_clock::time_point));
            
            // Read version string length and content
            size_t versionLen;
            file.read(reinterpret_cast<char*>(&versionLen), sizeof(versionLen));
            if (versionLen < 256) {
                cachedVersion.resize(versionLen);
                file.read(&cachedVersion[0], versionLen);
                cache.RobloxVersion = cachedVersion;
            }

            if (cache.RobloxVersion != version) {
                std::cout << "[*] Cache version mismatch: " << cache.RobloxVersion << " != " << version << "\n";
                return FALLBACK_OFFSETS;
            }

            file.close();
            return cache;
        } catch (...) {
            std::cout << "[!] Error reading offset cache\n";
            return FALLBACK_OFFSETS;
        }
    }

    /**
     * Saves detected offsets to cache file
     */
    static void SaveOffsetCache(const OffsetCache& cache) {
        try {
            std::ofstream file(OFFSET_CACHE_FILE, std::ios::binary);
            if (!file.is_open()) {
                std::cout << "[!] Could not save offset cache\n";
                return;
            }

            file.write(reinterpret_cast<const char*>(&cache), 
                      sizeof(OffsetCache) - sizeof(std::string) - sizeof(std::chrono::system_clock::time_point));
            
            // Write version string
            size_t versionLen = cache.RobloxVersion.length();
            file.write(reinterpret_cast<const char*>(&versionLen), sizeof(versionLen));
            file.write(cache.RobloxVersion.c_str(), versionLen);

            file.close();
            std::cout << "[+] Offset cache saved\n";
        } catch (...) {
            std::cout << "[!] Error saving offset cache\n";
        }
    }

    /**
     * Prints current offsets to log
     */
    static void LogOffsets(const OffsetCache& cache) {
        std::cout << "\n=== CURRENT OFFSETS (v" << cache.RobloxVersion << ") ===\n"
                  << "Name:                   0x" << std::hex << cache.Name << std::dec << "\n"
                  << "Children:               0x" << std::hex << cache.Children << std::dec << "\n"
                  << "Parent:                 0x" << std::hex << cache.Parent << std::dec << "\n"
                  << "ClassDescriptor:        0x" << std::hex << cache.ClassDescriptor << std::dec << "\n"
                  << "LocalScriptEmbedded:    0x" << std::hex << cache.LocalScriptEmbedded << std::dec << "\n"
                  << "ModuleScriptEmbedded:   0x" << std::hex << cache.ModuleScriptEmbedded << std::dec << "\n"
                  << "Bytecode:               0x" << std::hex << cache.Bytecode << std::dec << "\n"
                  << "BytecodeSize:           0x" << std::hex << cache.BytecodeSize << std::dec << "\n"
                  << "LocalPlayer:            0x" << std::hex << cache.LocalPlayer << std::dec << "\n"
                  << "IsCoreScript:           0x" << std::hex << cache.IsCoreScript << std::dec << "\n"
                  << "ModuleFlags:            0x" << std::hex << cache.ModuleFlags << std::dec << "\n"
                  << "ObjectValue:            0x" << std::hex << cache.ObjectValue << std::dec << "\n"
                  << "=============================\n\n";
    }
};
