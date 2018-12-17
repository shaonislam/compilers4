//
//  token.hpp
//  islam.p4
//
//  Created by Shaon Islam on 12/16/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#ifndef token_hpp
#define token_hpp

#include <string>
#include <map>

using namespace std;

const int NUM_TOKENS = 5;
const int NUM_KEYWORDS = 12;
const int NUM_OPERATORS = 19;

extern int current_index;
extern int token_index;
extern int line_index;

extern map<string, string> operator_map;
extern map<string, string> keyword_map;

enum token_ID {ID_tk, KEYWORDtk, OPtk, INT_tk, EOFtk};

//types of tokens
const string token_names[NUM_TOKENS] = {"Identifier", "Keyword",
    "Operator", "Integer", "End of File"};

//all the keywords
const string keywords[NUM_KEYWORDS] = {"start", "stop", "loop",
    "void", "var", "return", "read", "out", "program", "if",
    "scan", "let",};

//type of operators
const char operators[] = {'#', '=', '<', '>', ':', '+','-', '*', '/', '%', '.', '(', ')', ',', '{', '}', ';','[',']'};

//The comment symbol
const char COMMENT_DELIM = '#';

//token struct
struct Token
{
    token_ID ID;
    string desc;
    int line_number;
};

void populate_operator_map();
void populate_keyword_map();
void display_token(Token);
void get_operator(Token &);
int is_operator(char);
int is_keyword(Token &);
string get_token_desc(Token);
string get_token_int(Token);
#endif /* token_hpp */
