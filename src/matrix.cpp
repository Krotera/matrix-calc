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
#include <iomanip>
#include <sstream>
#include <math.h>
#include <regex>
#include <string>
#include <vector>
#include <stdexcept>

#include "matrix.h"

/******************************************************************************
 * Takes a string expression, "input", with two matrices, prompting the user  *
 * to re-enter it in part or in whole if it can't be understood, uses the     *
 * other functions to carry out the expression, and returns its result or     *
 * an error message as a string                                              *
 ******************************************************************************/
std::string take_input(std::string& input) {
    std::string A_string, B_string;
    char op_char;
    std::regex input_regex("(.*)( [\\*\\+\\-] )(.*)");
    std::smatch input_match;
    std::vector<std::vector<double>> matrix_A, matrix_B, matrix_C;

    //    //DEBUG
    //    if (std::regex_search(input, input_match, input_regex)) {
    //        std::cout << "\nDEBUG:\n";
    //        std::cout << "input_match.size() = " << input_match.size() << std::endl;
    //        for (size_t k = 0; k < input_match.size(); ++k) {
    //            std::cout << "input_match.str(" << k << "): " << input_match.str(k) << std::endl;
    //        }
    //    }
    
    // Check input and prompt for re-entry if bad
    while (!(std::regex_search(input, input_match, input_regex))
            || (input_match.size() != 4)
            || ((input_match.str(2).at(1) != '+')
            && (input_match.str(2).at(1) != '-')
            && (input_match.str(2).at(1) != '*'))) {
        std::cout << "ERROR: Input does not compute! Try again or hit Ctrl+C to quit.\n> ";
        std::getline(std::cin, input);
    }

    // Split into operator and matrix strings
    A_string = input_match.str(1); // input_match.str(1) should be the first matrix substring,
    op_char = input_match.str(2).at(1); // .str(2) should be the operator char padded with one space per side,
    B_string = input_match.str(3); // and .str(3) should be the second

    // Try to generate matrices, prompting for re-entry for each bad one
    input_regex.assign("[\\d\\.\\,\\s\\-]+"); // Preemptive reassign for input matrix error
    while (!string_to_matrix(A_string, matrix_A)) {
        std::cout << "ERROR: Your first matrix, \"" << A_string << "\", is incomputable!\n\
Type your first matrix in again or hit Ctrl+C to quit.\n\n> ";
        std::getline(std::cin, input);
        if (std::regex_search(input, input_match, input_regex)) {
            A_string = input;
        }
    }
    while (!string_to_matrix(B_string, matrix_B)) {
        std::cout << "ERROR: Your second matrix, \"" << B_string << "\", is incomputable!\n\
Type your second matrix in again or hit Ctrl+C to quit.\n\n> ";
        std::getline(std::cin, input);
        if (std::regex_search(input, input_match, input_regex)) {
            B_string = input;
        }
    }

    // Do math and return result as a string
    try {
        operate_matrices(op_char, matrix_A, matrix_B, matrix_C);
        return print_matrix(matrix_C);
    } catch (const std::invalid_argument& ia) {
        std::string error = "SOMETHING HAPPENED\n";

        error += ia.what();
        error += "\n";
        return error;
    }
}

/******************************************************************************
 * Populates "new_matrix" with data from "input", where sequences of          *
 * comma-separated numbers like                                               *
 *                                                                            *
 * "1 2 3, 4 5 6, -7 88.003 -0.9"                                             *
 *                                                                            *
 * represent the rows of a matrix, prompts the user                           *
 * for re-entry for every row that doesn't match the length of the first row, *
 * and returns a bool rerpresenting whether "new_matrix" was successfully     *
 * initialized                                                                *
 ******************************************************************************/
bool string_to_matrix(const std::string& input, std::vector<std::vector<double>>&new_matrix) {
    std::regex regex("[^\\,]*[\\\"\\d\\.\\s\\-]+");
    auto s_begin = std::sregex_iterator(input.begin(), input.end(), regex);
    auto s_end = std::sregex_iterator();

    /*
     * Populate new_matrix by calling populate_matrix_row_from_string for each 
     * comma-separated number sequence, keeping track of the length of subsequent rows and...
     */
    if (std::distance(s_begin, s_end) > 0) {
        int prev_cols = -1, curr_cols;

        for (std::sregex_iterator i = s_begin; i != s_end; ++i) {
            std::string num_group = (*i).str();

            curr_cols = populate_matrix_row_from_string(new_matrix, num_group);
            if (prev_cols == -1) {
                prev_cols = curr_cols;
            }
            // ...prompt for re-entry if detecting a mismatch
            while ((prev_cols != -1) && (curr_cols != prev_cols)) {
                std::string retry_row;

                new_matrix.pop_back();
                std::cout << "ERROR: The row \"" << num_group << "\" doesn't \
match the one(s) before it!\nPlease re-enter the row with " << prev_cols << " \
numbers: ";
                std::getline(std::cin, retry_row);
                curr_cols = populate_matrix_row_from_string(new_matrix, retry_row);
            }
        }
    } else {
        return false;
    }
    return true;
}

// Go through numbers, add them to vector's ith row, and return how many of them were in input
/******************************************************************************
 * Populates "matrixIn" with a vector of doubles interpreted from "input",    *
 * where a sequence of space-separated numbers like                           *
 *                                                                            *
 * "1.03 2 -0.3"                                                              *
 *                                                                            *
 * represents a matrix row, and returns an int representing the length of the *
 * row added                                                                  *
 ******************************************************************************/
int populate_matrix_row_from_string(std::vector<std::vector<double>>&matrixIn, const std::string& input) {
    std::istringstream iss(input);
    std::string elt;
    std::vector<double> row_vec;

    while (std::getline(iss, elt, ' ')) {
        if (elt != "\0" && elt != "\t" && elt != "\n") {
            std::string::const_iterator elt_it = elt.begin();
            std::string filtered_elt = ""; // Filter "input" of non-numeric chars
            bool have_decimal = false;

            // Be excessively tolerant of bad input for no justifiable reason
            while (elt_it != elt.end()) {
                char elt_c = (*elt_it);

                // Check for a number, negative prefix, or decimal point
                if (std::isdigit(elt_c)) {
                    filtered_elt += elt_c;
                } else if (elt_c == '-') {
                    if (filtered_elt.empty()) {
                        filtered_elt += elt_c;
                    }
                } else if (elt_c == '.') {
                    if (!have_decimal) {
                        filtered_elt += elt_c;
                        have_decimal = true;
                    }
                }
                ++elt_it;
            }
            if (!filtered_elt.empty()) {
                row_vec.push_back(std::stof(filtered_elt));
            }
        }
    }
    matrixIn.push_back(row_vec);

    return (int) row_vec.size();
}

/******************************************************************************
 * Carries out an addition, subtraction, or multiplication (based on the      *
 * operator char "op") on two matrices "A" and "B" (in the order A +/-/* B)   *
 * and throws an exception if the matrices are empty or incompatible for the  *
 * given operation                                                            * 
 ******************************************************************************/
void operate_matrices(const char& op, const std::vector<std::vector<double>>&A,
        const std::vector<std::vector<double>>&B, std::vector<std::vector<double>>&C) {

    //    std::cout << "DEBUG: " << A.size() << "×" << A[0].size() << " Matrix A\n" << print_matrix(A);
    //    std::cout << op << std::endl;
    //    std::cout << "DEBUG: " << B.size() << "×" << B[0].size() << " Matrix B\n" << print_matrix(B);

    if (A.size() != 0 && B.size() != 0) {
        const int A_rows = A.size();
        const int A_cols = A[0].size();
        const int B_rows = B.size();
        const int B_cols = B[0].size();

        if ((op == '*') && (A_cols == B_rows)) { // Multiplication
            for (int C_row = 0; C_row < A_rows; ++C_row) {
                std::vector<double> row_vec;

                for (int C_col = 0; C_col < B_cols; ++C_col) {
                    double product = 0;

                    for (int i = 0; i < A_cols; ++i) {
                        product += (A[C_row][i] * B[i][C_col]);
                    }
                    row_vec.push_back(product);
                }
                C.push_back(row_vec);
            }
        } else if (((op == '+') || (op == '-')) && (A_rows == B_rows) && (A_cols == B_cols)) { // Addition or subtraction
            for (int C_row = 0; C_row < A_rows; ++C_row) {
                std::vector<double> row_vec;

                for (int C_col = 0; C_col < B_cols; ++C_col) {
                    if (op == '+') {
                        row_vec.push_back((A[C_row][C_col] + B[C_row][C_col]));
                    } else {
                        row_vec.push_back((A[C_row][C_col] - B[C_row][C_col]));
                    }
                }
                C.push_back(row_vec);
            }
        } else {
            throw std::invalid_argument("ERROR: operate_matrices() received incompatible matrices or invalid operator!");
        }
    } else {
        throw std::invalid_argument("ERROR: operate_matrices() received one or more null/empty matrices!");
    }
}

/******************************************************************************
 * Creates and returns a formatted string representation of "matrix"          *
 ******************************************************************************/
std::string print_matrix(const std::vector<std::vector<double>>&matrix) {
    std::ostringstream oss;
    int max_padding = 0;

    if (matrix.size() == 0) { // Check for empty matrix
        throw std::invalid_argument("ERROR: print_matrix() received null/empty matrix!");
    } else {
        // Probe for padding measurement
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                int curr_len = std::to_string(matrix[i][j]).size();

                if (curr_len > max_padding) {
                    max_padding = curr_len;
                }
            }
        }
        /*
         * 12 chars is the max-length presentaiton (e.g., -4.32432e+28), so
         * cap it at 12 with a one-space pad to 13.
         */
        if (max_padding > 12) {
            max_padding = 13;
        }
        oss << "\n";
        // Print
        for (size_t i = 0; i < matrix.size(); ++i) {
            oss << "[";
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                oss << std::right << std::setw(max_padding) << matrix[i][j];
            }
            oss << " ]\n";
        }
    }
    return oss.str();
}