// main.cpp
#include <iostream>
#include <string>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define byte win_byte_override
#include <windows.h>
#undef byte

#include "uix.h"
#include "Color.h"
#include "Keybinds.h"

#include <sstream>
#include <cctype>
#include <algorithm>
#include <cctype>
#include <limits>
#include <regex>
#include <cmath>

using namespace Color;

int main() {
    set_terminal_size();
}
