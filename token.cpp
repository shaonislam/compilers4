//
//  token.cpp
//  islam.p4
//
//  Created by Shaon Islam on 12/16/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#include "token.hpp"

//token.cpp

#include <iostream>
#include <string>
#include <map>

using namespace std;

//Extern map from token.h
map<string, string> operator_map;

//Extern var from token.h
map<string, string> keyword_map;

void populate_operator_map()
{
    operator_map.insert(make_pair("=", "EQUALS_tk"));
    operator_map.insert(make_pair("<", "LESS_tk"));
    operator_map.insert(make_pair(":", "colon_tk"));
    operator_map.insert(make_pair(">", "GREAT_tk"));
    operator_map.insert(make_pair("+", "PLUS_tk"));
    operator_map.insert(make_pair("-", "MINUS_tk"));
    operator_map.insert(make_pair("*", "ASTERIK_tk"));
    operator_map.insert(make_pair("/", "SLASH_tk"));
    operator_map.insert(make_pair("%", "PCT_tk"));
    operator_map.insert(make_pair(".", "period_tk"));
    operator_map.insert(make_pair("(", "OPAR_tk"));
    operator_map.insert(make_pair(")", "CPAR_tk"));
    operator_map.insert(make_pair(",", "COMMA_tk"));
    operator_map.insert(make_pair("{", "OBRACE_tk"));
    operator_map.insert(make_pair("}", "CBRACE_tk"));
    operator_map.insert(make_pair(";", "SCOLON_tk"));
    operator_map.insert(make_pair("[", "OBRACKET_tk"));
    operator_map.insert(make_pair("]", "CBRACKET_tk"));
}

void populate_keyword_map()
{
    keyword_map.insert(make_pair("start", "start_tk"));
    keyword_map.insert(make_pair("stop", "stop_tk"));
    keyword_map.insert(make_pair("loop", "loop_tk"));
    keyword_map.insert(make_pair("void", "VOID_tk"));
    keyword_map.insert(make_pair("var", "var_tk"));
    keyword_map.insert(make_pair("return", "RETURN_tk"));
    keyword_map.insert(make_pair("scan", "scan_tk"));
    keyword_map.insert(make_pair("out", "out_tk"));
    keyword_map.insert(make_pair("program", "PROGRAM_tk"));
    keyword_map.insert(make_pair("if", "if_tk"));
    keyword_map.insert(make_pair("then", "THEN_tk"));
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
