#ifndef UIX_H
#define UIX_H

#include <string>
using namespace std;

void set_terminal_size();
void space(const int count = 1);
void print(const string &text, const int side = 0, const string color = "\e[0m");
void print_right(const string &text, const int side = 0, const string color = "\e[0m");
void print_left(const string &text, const int side = 0, const string color = "\e[0m");
void print_error(const string &text, const int side = 0);
void print_line(const char symbol = '_', const string &color = "\e[1;30m");
void print_gradient(const string &text, const int &color_start = 32, const int &color_end = 45, bool background = false, const int side = 0);
void line_title(const string text, const char symbol, const string line_color, const string text_color);
string centered_str(const string &text, const int side = 0);
string line_str(const char symbol);
string convert_case(const string &subject, const string &option = "");

extern int term_width;
extern int term_height;

#endif