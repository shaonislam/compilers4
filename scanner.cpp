//
//  scanner.cpp
//  islam.p4
//
//  Created by Shaon Islam on 12/16/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#include "scanner.hpp"
//scanner.cpp

#include <string>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <cstdlib>

#include "token.hpp"

using namespace std;

//From token.h
int token_index;

int current_index;
int line_index;
vector <string> file_string;
static int comment_flag;

//function to read in from file input
void read_file(istream &in)
{
    populate_operator_map();
    populate_keyword_map();
    
    string input_line;
    current_index = 0;
    Token token;
    
    int counter = 0;
    
    //get line until EOF
    while (getline(in, input_line))
    {
        current_index = 0;
        filter(input_line);
        
        if (input_line.length() > 0)
        {
            file_string.push_back(input_line);
            counter++;
        }
    }
}


int filter(string &input_string)
{
    if (current_index >= input_string.length())
        return 0;
    
    string filtered_string;
    
    char current_ch;
    const char SPACE = ' ';
    char prev_ch = ' ';
    
    //go through each char
    for (int counter = current_index; counter < input_string.length(); counter++)
    {
        current_ch = input_string.at(counter);
        
        if (counter > 0)
            prev_ch = input_string.at(counter - 1);
        
        if (current_ch == COMMENT_DELIM)
        {
            filtered_string.push_back(SPACE);
            comment_flag = !comment_flag;
        }
        else if (!comment_flag)
        {
            //check for space
            if (isspace(current_ch))
            {
                if (!isspace(prev_ch))
                    filtered_string.push_back(current_ch);
            }
            else if (is_valid_ch(current_ch) == -1)
            {
                //check for char
                cout << "Error: \'" << current_ch <<
                "\' is not a valid character.\n";
                exit(EXIT_FAILURE);
            }
            else
            {
                filtered_string.push_back(current_ch);
            }
        }
        
        current_index++;
    }
    
    string whitespaces = " \t\f\v\n\r";
    
    size_t trailing_whitespace_index =
    filtered_string.find_last_not_of(whitespaces);
    
    if (trailing_whitespace_index != string::npos)
    {
        filtered_string.erase(trailing_whitespace_index + 1);
    }
    else
    {
        bool all_spaces_flag = true;
        
        for (int i = 0; i < filtered_string.length(); i++)
        {
            if (filtered_string.at(i) != SPACE)
            {
                all_spaces_flag = false;
                break;
            }
        }
        if (all_spaces_flag)
            filtered_string.assign("");
    }
    
    input_string.assign(filtered_string);
    return current_index;
}

//check if the input is not an operator or digit or char
int is_valid_ch(char ch)
{
    if (!is_operator(ch) && !isdigit(ch) & !isalpha(ch))
        return -1;
    else
        return 0;
}

//get current line
string get_string()
{
    return file_string[line_index];
}

//----------------------------------------------------
int scanner(Token &token)
{
    string input_string = get_string();
    
    if (token_index == input_string.length())
    {
        line_index++;
        token_index = 0;
        
        if (line_index < file_string.size())
            input_string = file_string[line_index];
        else
        {
            token.desc = "EOF";
            token.ID = EOFtk;
            token.line_number = (line_index - 1);
            return 1;
        }
    }
    
    token.line_number = (line_index + 1);
    int current_state = 0;
    
    int next_state;
    int next_col;
    string token_desc;
    
    char next_char;
    const char SPACE = ' ';
    
    while (token_index <= input_string.length())
    {
        if (token_index < input_string.length())
            next_char = input_string.at(token_index);
        else
            next_char = SPACE;
        next_col = get_column(next_char);
        next_state = FSA_TABLE[current_state][next_col];
        
        if (next_state < 0)
        {
            current_state = 0;
            error_output(next_state, input_string);
            cout << input_string.substr(0, token_index + 1) << "\n";
            cout << string(token_index, SPACE) << "^\n";
            
            token_index++;
            
            exit(EXIT_FAILURE);
        }
        else if (next_state > FINAL_STATES)
        {
            token.desc = token_desc;
            
            switch (next_state)
            {
                case IDENTIFIER_FINAL_STATE:
                    if (is_keyword(token) != -1)
                    {
                        token.ID = KEYWORDtk;
                        token.desc.assign(token_desc);
                    }
                    else
                    {
                        token.ID = ID_tk;
                        token.desc.assign("ID_tk " + token_desc);
                    }
                    break;
                case INTEGER_FINAL_STATE:
                    
                    token.ID = INT_tk;
                    token.desc.assign("INT_tk " + token_desc);
                    break;
                    
                case OPERATOR_FINAL_STATE:
                    
                    token.ID = OPtk;
                    get_operator(token);
                    token.desc.assign(token_desc);
                    break;
            }
            
            return 0;
        }
        
        current_state = next_state;
        
        token_index++;
        
        if (!isspace(next_char))
            token_desc.push_back(next_char);
    }
    
    return -1;
}
//--------------------------------------------------
//get the column with the input
int get_column(char ch)
{
    if (isalpha(ch))
    {
        //check if input is lower case
        if (islower(ch))
            return 0; //return 0 for lower
        else
            return 1; //return 1 for upper
    }
    else if (isdigit(ch))
        return 2; //return 2 for digit
    else if (isspace(ch))
        return 3; //return 3 for space
    else if (is_operator(ch))
        return 5; //return 5 for operatort=
    else
        return -1; //does not exisit
}

//output error
void error_output(int state, string input_string)
{
    cout << "Scanner error: Line " << line_index << ": ";
    
    //print error
    if (state == ERROR_STATE_UPPERCASE)
    {
        cout << "All tokens must be lower case \n";
    }
    else if (state == ERROR_STATE_INTEGER)
    {
        cout << "All integer tokens must only be numbers\n";
    }
}
