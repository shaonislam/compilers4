//
//  scanner.hpp
//  islam.p4
//
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#ifndef scanner_hpp
#define scanner_hpp

#include "token.hpp"
#include <vector>

using namespace std;

//vector for input
extern vector<string> file_string;

//The FSA table is 4x6
const int STATES = 4;
const int COLUMNS = 6;

//States in the FSA table
const int STATE_ZERO = 0;
const int STATE_ONE = 1;
const int STATE_TWO = 2;
const int STATE_THREE = 3;
const int FINAL_STATES = 1000;
const int IDENTIFIER_FINAL_STATE = 1001;
const int INTEGER_FINAL_STATE = 1003;
const int OPERATOR_FINAL_STATE = 1004;
const int EOF_FINAL_STATE = 1005;
const int ERROR_STATE_UPPERCASE = -1;
const int ERROR_STATE_INTEGER = -2;


const int FSA_TABLE[STATES][COLUMNS] =
{
    {STATE_TWO, ERROR_STATE_UPPERCASE, STATE_THREE, STATE_ZERO,
        EOF_FINAL_STATE, STATE_ONE},
    
    {OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE,
        OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE},
    
    {STATE_TWO, STATE_TWO, STATE_TWO, IDENTIFIER_FINAL_STATE,
        IDENTIFIER_FINAL_STATE, IDENTIFIER_FINAL_STATE},
    
    {ERROR_STATE_INTEGER, ERROR_STATE_INTEGER, STATE_THREE,
        INTEGER_FINAL_STATE, INTEGER_FINAL_STATE, INTEGER_FINAL_STATE}
};

void read_file(istream&);
int filter(string&);
int is_valid_ch(char);
string get_string();
int scanner(Token &);
int get_column(char);
void error_output(int, string);

#endif /* scanner_hpp */
