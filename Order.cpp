/***
 * Tanvi Herwadkar
 * therwadk
 * 2023 Fall CSE101 PA7
 * Order.cpp
 * Main Client for Dictionary ADT
***/ 

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "Dictionary.h"

using namespace std;

int main (int argc, char* argv[]) {
    if (argc != 3) {
        throw invalid_argument("Only one command line argument\nUsage: $ Order <input> <output>");
    }
    ifstream in;
    ofstream out;
    in.open(argv[1]);
    if (!in.is_open()) {
        throw invalid_argument("Unable to open input file");
    }
    out.open(argv[2]);
    if (!out.is_open()) {
        throw invalid_argument("Unable to open output file");
    }

    Dictionary A;
    string s;
    int line_counter = 1;
    while (getline(in, s)) {
        A.setValue(s, line_counter);
        line_counter += 1;
    }

    out << A.to_string() << endl;
    out << A.pre_string() << endl;

    in.close();
    out.close();
    return 0;
}