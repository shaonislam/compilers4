//
//  main.cpp
//  islam.p4
//
//main.cpp

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>


#include "token.hpp"
#include "node.hpp"
#include "scanner.hpp"
#include "parser.hpp"
#include "testTree.hpp"
#include "staticSemantics.hpp"


using namespace std;

ofstream target_file;

int main(int argc, char* argv[])
{
    //input file
    ifstream input_file;
    
    //input file with ext
    char * input_file_name;
    
    // error if more than 2 arguments
    if (argc > 2)
    {
        cout << "Error: Exceeded Argument limit \n";
        exit(EXIT_FAILURE);
    }
    else if (argc == 2)
    {
        //get input file
        string file_name = argv[1];
        
        size_t index = file_name.find(".fs182");
        string input_file_str = file_name;
        string output_file_str = file_name;
        
        //add extension
        if (index == string::npos)
        {
            input_file_str.append(".fs182");
            output_file_str.append(".asm");
        }
        else
        {
            output_file_str = file_name.substr(0, index);
            output_file_str.append(".asm");
        }
        input_file_name = new char[input_file_str.length() + 1];
        char *output_file_name = new char[output_file_str.length() + 1];
        
        strcpy(input_file_name, input_file_str.c_str());
        strcpy(output_file_name, output_file_str.c_str());
        
        input_file.open(input_file_name, ifstream::in);
        
        if (!input_file.is_open())
        {
            cout << "Error: Unable To Open \n" << file_name;
            
            return -1;
        }
        else
        {
            target_file.open(output_file_name, ofstream::out);
            read_file(input_file);
        }
    }
    //no arguments read in from keyboard
    else if (argc == 1)
    {
        
        target_file.open("out.asm", ofstream::out);
        read_file(cin);
    }
    
    //Close the input file
    input_file.close();
    Node *root = parser();
    staticSemantics(root);
    //preorder(root, root->level);
    target_file.close();
    
    return 0; //main
} //main
