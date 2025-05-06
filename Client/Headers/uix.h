#ifndef UIX_H
#define UIX_H

#include <string>
using namespace std;

void set_terminal_size();
void space(const int count = 1);
void print(const string &text, const int side = 0, const vector<string> &colors = {"\e[0m"});
void print_right(const string &text, const int side = 0, const string color = "\e[0m");
void print_left(const string &text, const int side = 0, const string color = "\e[0m");
void print_sides(const string &left_text, const string &right_text, int side = 0, const string &left_color = "\e[0m", const string &right_color = "\e[0m");
void print_error(const string &text, const int side = 0);
void print_line(const char symbol = '_', const string &color = "\e[1;30m");
void print_gradient(const string &text, const int &color_start = 32, const int &color_end = 45, bool background = false, const int side = 0);
void line_title(const string text, const char symbol, const string line_color, const string text_color);

void set_cursor(int x, int y);
void move_cursor(int dx, int dy);
void clear_line(int line_number);

void print_input_box(int width, const int side = 0, const string &bg_color = "\033[48;5;7m");
void print_input_box(int width, const int side, const string &bg_color, const string &label, bool side_label, const string &position = "left");

string line_title_str(const string text, const char symbol);
string centered_str(const string &text, const int side = 0);
string line_str(const char symbol);
string convert_case(const string &subject, const string &option = "");

extern int term_width;
extern int term_height;

#endif