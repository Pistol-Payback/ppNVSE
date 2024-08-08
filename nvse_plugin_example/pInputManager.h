#pragma once
#include "ppNVSE.h"
#include "EventHandlers.h"
#include <string>
#include <unordered_map>
#include <set>

enum ControlType {
    DirectXKey,
    GamepadButton,
    GameControlCode
};

// Control identifier
struct ControlID {

    ControlType type;
    int code;

    bool operator==(const ControlID& other) const {
        return type == other.type && code == other.code;
    }

    struct Hash {
        size_t operator()(const ControlID& cid) const {
            return std::hash<int>()(cid.type) ^ std::hash<int>()(cid.code);
        }
    };

};

struct ControlSet {

    std::unordered_set<ControlID, ControlID::Hash> keys;
    bool isMenuMode;
    std::vector<Script*> onActivate;
    std::vector<Script*> onDeactivate;

    void activateScripts() const {
        for (const auto& script : onActivate) {
            //script->run();
        }
    }

    void deactivateScripts() const {
        for (const auto& script : onDeactivate) {
            //script->run();
        }
    }
};

class ControlManager {

public:

    std::unordered_map<std::string, ControlSet> modes;
    std::unordered_map<ControlID, int, ControlID::Hash> keyUsageCount;
    std::unordered_set<std::string> activeMenuModes;

    void enableMode(const std::string& modeName, const std::unordered_set<ControlID, ControlID::Hash>& controls, bool isMenuMode = false,
        const std::vector<Script*>& activateScripts = {}, const std::vector<Script*>& deactivateScripts = {}) {
        modes[modeName] = { controls, isMenuMode, activateScripts, deactivateScripts };
        for (const ControlID& control : controls) {
            keyUsageCount[control]++;
        }
        if (isMenuMode) {
            activeMenuModes.insert(modeName);
        }
        modes[modeName].activateScripts();  // Execute activation scripts
    }

    void disableMode(const std::string& modeName) {
        auto it = modes.find(modeName);

        if (it != modes.end()) {

            it->second.deactivateScripts();  // Execute deactivation scripts

            for (const ControlID& control : it->second.keys) {
                if (--keyUsageCount[control] == 0) {
                    keyUsageCount.erase(control);
                }
            }
            if (it->second.isMenuMode) {
                activeMenuModes.erase(modeName);
            }

            modes.erase(it);

        }
    }

    bool isControlDisabled(const ControlID& control) const {
        return keyUsageCount.find(control) != keyUsageCount.end();
    }
};
