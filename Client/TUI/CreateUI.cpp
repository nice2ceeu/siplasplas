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

//? ________________________________ FUNCTION  ________________________________ 

// Line with Text
void line_title(const string text, const char symbol, const string line_color, const string text_color){
    string decorated_text = "  " + text + "  ";  // Text with padding
    int text_length = decorated_text.length();
    int line_length = term_width;

    // Calculate how many symbols go on each side
    int remaining = line_length - text_length;
    if (remaining < 0) remaining = 0;

    int left_len = remaining / 2;
    int right_len = remaining - left_len;

    // Create the left and right parts of the line
    string left = string(left_len, symbol);
    string right = string(right_len, symbol);

    // Combine and print with colors
    cout << line_color << left << text_color << decorated_text << line_color << right << "\033[0m" << endl;
}

// ADDS ENDLINE
void space(const int count){
    for(int i = 0; i < count; i++){
        cout << "\n";
    }
}

// TERMINAL SIZE
void set_terminal_size(){

    ostringstream console;
    console << "mode con: cols=" << term_width << " lines=" <<term_height;
    string command = console.str();

    system(command.c_str());

    return;
}

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

// PRINT TO RIGHT
void print_right(const string &text, const int side, const string color) {
    string fulltext = text;

    int effective_length = 0; 
    for (char ch : fulltext) { 
        if (ch == '\n' || ch == '\t') continue; 
        effective_length += 1; 
    } 

    // Quarter-width centering (right side)
    int quarter_width = term_width / 2;
    int spaces = ((quarter_width - effective_length) / 2) + side + quarter_width;

    cout << string(max(0, spaces), ' ') << color << fulltext << (color.empty() ? "" : "\033[0m");
}

// PRINT TO LEFT
void print_left(const string &text, const int side, const string color) {
    // Move cursor up 1 line and to column 1 (start of line)
    cout << "\033[1A\r" << flush;

    // Calculate effective text length (skip \n, \t)
    int effective_length = 0;
    for (char ch : text) {
        if (ch != '\n' && ch != '\t') effective_length++;
    }

    int half_width = term_width / 2;
    int spaces = ((half_width - effective_length) / 2) + side;

    // Pad right side to clear leftovers in left half
    int total_fill = half_width;

    string output = string(max(0, spaces), ' ') + color + text + (color.empty() ? "" : "\033[0m");
    int fill = max(0, (int)total_fill - (int)output.length());
    output += string(fill, ' ');

    cout << output << flush;
}

#include <iostream>
#include <string>
using namespace std;


void print_sides(const string &left_text, const string &right_text, int side, const string &left_color, const string &right_color) {

    int half_width = term_width / 2;

    // LEFT SIDE
    int left_spaces = (half_width / 2) + side;
    string left_output = string(max(0, left_spaces), ' ') + left_color + left_text + (left_color.empty() ? "" : "\033[0m");

    // RIGHT SIDE
    int right_spaces = (half_width / 2) + side;
    string right_output = string(max(0, right_spaces), ' ') + right_color + right_text + (right_color.empty() ? "" : "\033[0m");

    // Move cursor up and return to start to overwrite same line
    cout << "\033[1A\r" << flush;

    // Combine and print
    cout << left_output;

    // Pad if needed to reach middle of screen before right text
    int pad_to_middle = max(0, term_width / 2 - (int)left_output.length());
    cout << string(pad_to_middle, ' ');

    cout << right_output << endl;
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