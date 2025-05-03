// utils.h
#ifndef KEYBINDS_H
#define KEYBINDS_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Helper function to convert string to lowercase
inline std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// Login keybinds
inline std::vector<std::string> login_keybinds() {
    return {"login", "l", "log"};
}

inline bool login_key(std::string input) {
    input = to_lower(input);
    const auto keybinds = login_keybinds();
    
    for(const auto& key : keybinds) {
        if(input == key) {
            return true;
        }
    }
    return false;
}

// Register keybinds
inline std::vector<std::string> register_keybinds() {
    return {"reg", "r", "register"};
}

inline bool register_key(std::string input) {
    input = to_lower(input);
    const auto keybinds = register_keybinds();
    
    for(const auto& key : keybinds) {
        if(input == key) {
            return true;
        }
    }
    return false;
}

// Settings keybinds
inline std::vector<std::string> setting_keybinds() {
    return {"s", "set", "settings"};
}

inline bool setting_key(std::string input) {
    input = to_lower(input);
    const auto keybinds = setting_keybinds();
    
    for(const auto& key : keybinds) {
        if(input == key) {
            return true;
        }
    }
    return false;
}

// Back keybinds
inline std::vector<std::string> back_keybinds() {
    return {"b", "back", "z", "0"};
}

inline bool back_key(std::string input) {
    input = to_lower(input);
    const auto keybinds = back_keybinds();
    
    for(const auto& key : keybinds) {
        if(input == key) {
            return true;
        }
    }
    return false;
}

// Exit keybinds
inline std::vector<std::string> exit_keybinds() {
    return {"exit", "x", "quit", "q", "e"};
}

inline bool exit_key(std::string input) {
    input = to_lower(input);
    const auto keybinds = exit_keybinds();
    
    for(const auto& key : keybinds) {
        if(input == key) {
            return true;
        }
    }
    return false;
}

// Left keybinds
inline std::vector<std::string> left_keybinds() {
    return {"<", "{", "a", "4"};
}

inline bool left_key(std::string input) {
    input = to_lower(input);
    const auto keybinds = left_keybinds();
    
    for(const auto& key : keybinds) {
        if(input == key) {
            return true;
        }
    }
    return false;
}

// Right keybinds
inline std::vector<std::string> right_keybinds() {
    return {">", "}", "d", "6"};
}

inline bool right_key(std::string input) {
    input = to_lower(input);
    const auto keybinds = right_keybinds();
    
    for(const auto& key : keybinds) {
        if(input == key) {
            return true;
        }
    }
    return false;
}


#endif
