// main.cpp
#include <iostream>
#include "uix.h"
#include "Color.h"
#include <string>

using namespace std;
using namespace Color;
void auth_page();

int main() {
    set_terminal_size();
    auth_page();
}

// LOGIN / REGISTER
void auth_page(){

    space(1);
    print_gradient(line_str('='), 119, 123);
    space(5);
    print("SUPPLYSYNC");
    space(5);
    print_gradient(line_str('='), 119, 123);
    space(2);
    
    // TODO: This doesnt work, Whole line of Right is getting overwritten
    print_right("REGISTER");
    print_left("LOGIN");
}
