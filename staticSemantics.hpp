//
//  staticSemantics.hpp
//  islam.p4
//
//  Created by Shaon Islam on 12/17/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#ifndef staticSemantics_hpp
#define staticSemantics_hpp

#include <iostream>
#include <fstream>
#include <vector>

#include "token.hpp"
#include "node.hpp"

using namespace std;

extern ofstream target_file;

struct stack_tt
{
    Token token;
    int scope;
    int line_number;
};

//vector stack
extern vector<stack_tt> stackk;


//traverse tree and parse tree functions
void staticSemantics(node_t*);
void check_var(stack_tt);

int check_var_exists(stack_tt);
int find_var(stack_tt);
int find_on_stack(stack_tt);
int compare_scope(stack_tt);


void remove_local_vars(int);



void print_stack();


#endif /* staticSemantics_hpp */
