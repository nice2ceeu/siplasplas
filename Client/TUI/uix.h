#ifndef UIX_H
#define UIX_H

#include <string>
using namespace std;

void print(const string &text, const int side = 0, const string color = "\e[0m");
void print_error(const string &text, const int side = 0);
void print_line(const char symbol = '_', const string &color = "\e[1;30m");
void print_gradient(const string &text, const int &color_start = 32, const int &color_end = 45, bool background = false, const int side = 0);
string centered_str(const string &text, const int side = 0);
string line_str(const char symbol);
string convert_case(const string &subject, const string &option = "");

extern int term_width;
extern int term_height;

#endif