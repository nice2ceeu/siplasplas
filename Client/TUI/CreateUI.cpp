// IMPORTS
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <cctype>
#include <algorithm>
#include <limits>
#include <regex>
#include <cmath>
#include "uix.h"
#include "../Headers/Color.h"

// NAMESPACES
using namespace std;
using namespace Color;

// GLOBAL VARIABLE
int term_width = 80; 
int term_height = 20;


// CONFIGURATION PURPOSES
int main1() {
    // UI INITIAL TEST
    print("Sample Center\n", 0, Color::red);
    print_line();
    print_error("ERROR!\n");
    print_gradient("HELLO THERE EVERYONE\n");
    print(convert_case("ThIS wIll BE FiX\n"));

    return 0;
}

//? ________________________________ FUNCTION  ________________________________ 

// CENTER ALL TEXT
void print(const string &text, const int side, const string color){ 
    string fulltext = text;

    int effective_length = 0; 
    for (char ch : fulltext){ 
        if (ch == '\n' || ch == '\t'){ 
            continue; 
        } 
        effective_length += 1; 
    } 

    int spaces = ((term_width - effective_length) / 2) + side; 
    cout << string(spaces, ' ') << color << fulltext << "\e[0m";
}

// ERROR PANEL
void print_error(const string &text, const int side){
    string fulltext = text;

    int effective_length = 0; 
    for (char ch : fulltext){ 
        if (ch == '\n' || ch == '\t'){ 
            continue; 
        } 
        effective_length += 1; 
    } 

    int spaces = ((term_width - effective_length) / 2) + side; 
    cout << string(spaces, ' ') << "\x1B[31m" << fulltext << "\e[0m";
}

// ADD A LINE TO THE TERMINAL
void print_line(const char symbol, const string &color){
    cout << color << string(term_width, symbol) << "\n\e[0m" << endl;
}

// ADD A GRADIENT TEXT
void print_gradient(const string &text, const int &color_start, const int &color_end, bool background, const int side) {
    int color_range = color_end - color_start;
    int text_length = text.length();

    // Calculate the effective length (ignore escape codes for centering)
    int effective_length = 0;
    for (char ch : text) {
        if (ch == '\n' || ch == '\t') {
            continue;
        }
        effective_length += 1;
    }

    // Calculate the spaces to center the text
    int spaces = ((term_width - effective_length) / 2) + side;
    
    // Print leading spaces
    cout << string(spaces, ' ');

    // Print the gradient text with colors
    for (int i = 0; i < text_length; ++i) {
        int color_code = color_start + (color_range * i / text_length);
        if (background) {
            cout << "\033[48;5;" << color_code << "m\033[38;5;" << color_code << "m" << text[i];
        } else {
            cout << "\033[38;5;" << color_code << "m" << text[i];
        }
    }
    
    // Reset color at the end
    cout << "\033[0m"; 
}



// RETURN A CENTERED STRING
string centered_str(const string &text, const int side){ 
    string fulltext = text;

    int effective_length = 0; 
    for (char ch : fulltext){ 
        if (ch == '\n' || ch == '\t'){ 
            continue; 
        } 
        effective_length += 1; 
    } 

    int spaces = ((term_width - effective_length) / 2) + side; 
    return string(spaces, ' ') + fulltext;
}

// RETURN A LINE STRING FOR MODIFICATION
string line_str(const char symbol){
    return string(term_width, symbol);
}

// CHANGE A STRING'S CASE
string convert_case(const string &subject, const string &option) {
    string result = subject;

    if (option == "lower") {
        transform(result.begin(), result.end(), result.begin(), ::tolower);
    } else if (option == "upper") {
        transform(result.begin(), result.end(), result.begin(), ::toupper);
    }else{
        bool capitalize = true;
        for (char &ch : result) {
            if (isspace(ch)) {
                capitalize = true;
            } else if (capitalize) {
                ch = toupper(ch);
                capitalize = false;
            } else {
                ch = tolower(ch);
            }
        }
    }

    return result;
}