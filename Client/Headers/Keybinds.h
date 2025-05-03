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

// Login
inline bool login_key(std::string input) {
    input = to_lower(input);
    const std::vector<std::string> keybinds = {"login", "l", "log"};
    
    for(const auto& key : keybinds ){
        if(input == key){
            return true;
        }
    }
    return false;
}

// Register
inline bool register_key(std::string input) {
    input = to_lower(input);
    const std::vector<std::string> keybinds = {"reg", "r", "register"};
    
    for(const auto& key : keybinds ){
        if(input == key){
            return true;
        }
    }
    return false;
}
// Settings
inline bool setting_key(std::string input) {
    input = to_lower(input);
    const std::vector<std::string> keybinds = {"s", "set", "settings"};
    
    for(const auto& key : keybinds ){
        if(input == key){
            return true;
        }
    }
    return false;
}

// Back
inline bool back_key(std::string input) {
    input = to_lower(input);
    const std::vector<std::string> keybinds = {"b", "back", "z", "0"};
    
    for(const auto& key : keybinds ){
        if(input == key){
            return true;
        }
    }
    return false;
}

// Exit
inline bool exit_key(std::string input) {
    input = to_lower(input);
    const std::vector<std::string> keybinds = {"exit", "x", "quit", "q"};
    
    for(const auto& key : keybinds ){
        if(input == key){
            return true;
        }
    }
    return false;
}

// Left
inline bool left_key(std::string input) {
    input = to_lower(input);
    const std::vector<std::string> keybinds = {"<", "{", "a", "4"};
    
    for(const auto& key : keybinds ){
        if(input == key){
            return true;
        }
    }
    return false;
}

// Right
inline bool right_key(std::string input) {
    input = to_lower(input);
    const std::vector<std::string> keybinds = {">", "}", "d", "6"};
    
    for(const auto& key : keybinds ){
        if(input == key){
            return true;
        }
    }
    return false;
}

#endif
