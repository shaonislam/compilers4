//
//  token.cpp
//  islam.p4
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#include "token.hpp"
#include <iostream>
#include <string>
#include <map>

using namespace std;
map<string, string> operator_map;
map<string, string> keyword_map;

void setupOperators()
{
    operator_map.insert(make_pair("=", "eq_tk"));
    operator_map.insert(make_pair("<", "lessthan_tk"));
    operator_map.insert(make_pair(":", "colon_tk"));
    operator_map.insert(make_pair(">", "greaterthan_tk"));
    operator_map.insert(make_pair("+", "plus_tk"));
    operator_map.insert(make_pair("-", "minus_tk"));
    operator_map.insert(make_pair("*", "asterisk_tk"));
    operator_map.insert(make_pair("/", "slash_tk"));
    operator_map.insert(make_pair("%", "percent_tk"));
    operator_map.insert(make_pair(".", "period_tk"));
    operator_map.insert(make_pair("(", "leftparenthesis_tk"));
    operator_map.insert(make_pair(")", "rightparenthesis_tk"));
    operator_map.insert(make_pair(",", "comma_tk"));
    operator_map.insert(make_pair("{", "leftbrace_tk"));
    operator_map.insert(make_pair("}", "rightbrace_tk"));
    operator_map.insert(make_pair(";", "semicolon_tk"));
    operator_map.insert(make_pair("[", "leftbracket_tk"));
    operator_map.insert(make_pair("]", "rightbracket_tk"));
}

void setupKeywords()
{
    keyword_map.insert(make_pair("start", "start_tk"));
    keyword_map.insert(make_pair("stop", "stop_tk"));
    keyword_map.insert(make_pair("loop", "loop_tk"));
    keyword_map.insert(make_pair("void", "void_tk"));
    keyword_map.insert(make_pair("var", "var_tk"));
    keyword_map.insert(make_pair("scan", "scan_tk"));
    keyword_map.insert(make_pair("out", "out_tk"));
    keyword_map.insert(make_pair("program", "program_tk"));
    keyword_map.insert(make_pair("if", "if_tk"));
    keyword_map.insert(make_pair("var", "var_tk"));
    keyword_map.insert(make_pair("let", "let_tk"));
}

//line number token error
void display_token(Token token)
{
    cout << "Line number: " << token.line_number << " " << token_names[token.ID]
    << " " << token.desc << "\n";
}


void get_operator(Token &token)
{
    token.desc.assign(operator_map[token.desc]);
}

int is_operator(char ch)
{
    for (int counter = 0; counter < NUM_OPERATORS; counter++)
    {
        //get operators
        if (ch == operators[counter])
            return 1;
    }
    return 0;
}

//check for keyword function
int is_keyword(Token &token)
{
    
    for (int counter = 0; counter < NUM_KEYWORDS; counter++)
    {
        if (token.desc == keywords[counter])
        {
            token.desc = keyword_map[token.desc];
            return counter;
        }
    }
    return -1;
}

//get token ID
string get_token_desc(Token token)
{
    size_t found = token.desc.find("ID_tk");
    string desc = token.desc;
    
    if (found != string::npos)
        desc.erase(found, 5);
    
    return desc;
}

//get token INT
string get_token_int(Token token)
{
    size_t found = token.desc.find("INT_tk");
    string desc = token.desc;
    
    if (found != string::npos)
        desc.erase(found, 6);
    
    return desc;
}
