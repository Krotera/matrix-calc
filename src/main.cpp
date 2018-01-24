/*
 * Matrix Calculator
 * 2018-01-23
 * Copyright (C) 2018 Krotera
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <vector>
#include <string>
#include <regex>

#include "matrix.h"

int main(int argc, char *argv[]) {
    std::string input;

    if (argc > 1) { // One-off process
        if (argc > 2) {
            std::cout << "WARNING: Wrap your matrix expression in quotes; only \
the first argument, \"" << argv[1] << "\", is taken.\n";
        }
        input = argv[1];
        std::cout << take_input(input);
    } else { // Continous process
        // Greet
        std::cout << " __                   _        _                  _        __\n\
| _|  _ __ ___   __ _| |_ _ __(_)_  __   ___ __ _| | ___  |_ |\n\
| |  | '_ ` _ \\ / _` | __| '__| \\ \\/ /  / __/ _` | |/ __|  | |\n\
| |  | | | | | | (_| | |_| |  | |>  <  | (_| (_| | | (__   | |\n\
| |  |_| |_| |_|\\__,_|\\__|_|  |_/_/\\_\\  \\___\\__,_|_|\\___|  | |\n\
|__|                                                      |__|\n\n\
Enter two matrix strings with an operator (+,-,*).\nFor example: \n\
                                                  [ 1 2 3 ]   [ 0 0 1 ]\n\
\"1 2 3,4 5 6,7 8 9 * 0 0 1,0 1 0,1 0 0\"  becomes  [ 4 5 6 ] * [ 0 1 0 ]\n\
                                                  [ 7 8 9 ]   [ 1 0 0 ]\n\
(The quotes are optional here \nbut necessary when calling this executable\
 with arguments in the terminal.)\n\n> ";
        do {
            std::getline(std::cin, input);
            std::cout << take_input(input);

            std::cout << "\nEnter another matrix equation or hit Ctrl+C to quit.\n\n> ";
        } while (1);
    }

    return 0;
}
