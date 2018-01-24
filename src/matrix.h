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

#ifndef MATRIX_H
#define MATRIX_H

std::string take_input(std::string& input);
bool string_to_matrix(const std::string& input, std::vector<std::vector<double>>& new_matrix);
int populate_matrix_row_from_string(std::vector<std::vector<double>>& matrixIn, 
        const std::string& input);
std::string print_matrix(const std::vector<std::vector<double>>& matrix);
void operate_matrices(const char& op, const std::vector<std::vector<double>>& A, 
        const std::vector<std::vector<double>>& B, std::vector<std::vector<double>>& C);

#endif /* MATRIX_H */
