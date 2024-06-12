#include <linux_parser.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

#include "ncurses_display.h"
#include "system.h"

int main() {

  System system;
  NCursesDisplay::Display(system);
}
