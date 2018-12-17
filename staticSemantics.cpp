//
//  staticSemantics.cpp
//  islam.p4
//
//  Created by Shaon Islam on 12/17/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "staticSemantics.hpp"
#include "token.hpp"
#include "node.hpp"
#include <iomanip>

using namespace std;

// max stack size
// const int MAX_VARS = 100;

//Var scope
static int var_scope = 0;

//temp vars
static int temp_vars = 0;

static int num_repeat = 0;

//stack temp
static stack_tt temp;

//vector stack for vars
vector<stack_tt> stackk;

//location on stack
int stack_location = 0;

//Total vars on stack
int stack_vars = 0;





// check local and global scopes
int compare_scope(stack_tt var)
{
    int var_location = check_var_exists(var);
    
    if (var_location >= 0)
    {
        if (stackk.at(var_location).scope > var.scope)
        {
            cout << "Error: staticSemantics  variable \'" << get_token_desc(var.token)
            << "\' on line " << var.token.line_number <<
            " cannot be accessed in this scope.\n";
            exit(EXIT_FAILURE);
        }
        else
            return 1;
    }
    else
    {
        
        cout << "Error: staticSemantics variable \'" << get_token_desc(var.token)
        << "\' on line " << var.token.line_number <<
        " is not on the stack, and has either not yet been defined OR "
        << "cannot be accessed in this scope.\n";
        exit(EXIT_FAILURE);
    }
}

void staticSemantics(node_t *root)
{
    if (root == NULL)
        return;
    
    // <program> -> void <vars> <block>
    if (root->label == "<program>")
    {
        staticSemantics(root->child_1);
        staticSemantics(root->child_2);
        
        remove_local_vars(var_scope);
        var_scope--;
        
        //out.asm STOP
        target_file << "STOP\n";
        
        while (temp_vars > 0)
        {
            target_file << "T" << (temp_vars - 1) << " 0\n";
            temp_vars--;
        }
        
        return;
    }
    
    // <block> -> begin <vars> <stats> end
    if (root->label == "<block>")
    {
        var_scope++;
        
        staticSemantics(root->child_1);
        staticSemantics(root->child_2);
        
        remove_local_vars(var_scope);
        var_scope--;
    }
    
    // <vars> -> empty | let Identifier = Integer <vars>
    if (root->label == "<vars>")
    {
        temp.token = root->tokens.front();
        temp.scope = var_scope;
        
        if (temp.token.desc != "EMPTY")
        {
            check_var(temp);
            stack_vars++;
            
            staticSemantics(root->child_1);
        }
    }
    
    // <expr> -> <A> / <expr> | <A> * <expr> | <A>
    if (root->label == "<expr>")
    {
        if (root->tokens.empty())
            staticSemantics(root->child_1);
        else
        {
            int local_var = temp_vars;
            temp_vars++;
            staticSemantics(root->child_2);
            
            target_file << "STORE T" << local_var << "\n";
            staticSemantics(root->child_1);
            
            if (operator_map[root->tokens.at(0).desc] == "SLASH_tk")
            {
                target_file << "DIV T" << local_var << "\n";
            }
            else if (operator_map[root->tokens.at(0).desc] == "ASTERIK_tk")
            {
                
                target_file << "MULT T" << local_var << "\n";
            }
            return;
        }
    }
    
    // <A> -> <M> + <A> | <M> - <A> | <M>
    if (root->label == "<A>")
    {
        if (root->tokens.empty())
            staticSemantics(root->child_1);
        else
        {
            int local_var = temp_vars;
            temp_vars++;
            
            staticSemantics(root->child_2);
            target_file << "STORE T" << local_var << "\n";
            staticSemantics(root->child_1);
            
            if (operator_map[root->tokens.at(0).desc] == "PLUS_tk")
            {
                target_file << "ADD T" << local_var << "\n";
            }
            else if (operator_map[root->tokens.at(0).desc] == "MINUS_tk")
            {
                target_file << "SUB T" << local_var << "\n";
            }
            return;
        }
    }
    
    // <M> -> -<M> | <R>
    if (root->label == "<M>")
    {
        staticSemantics(root->child_1);
        
        if (!root->tokens.empty())
        {
            target_file << "MULT -1\n";
        }
        return;
    }
    
    // <R> -> (<expr>) | Identifier | Integer
    if (root->label == "<R>")
    {
        if (root->tokens.empty())
            staticSemantics(root->child_1);
        else
        {
            temp.token = root->tokens.front();
            temp.scope = var_scope;
            
            if (temp.token.ID == ID_tk)
            {
                compare_scope(temp);
                target_file << "STACKR " << find_on_stack(temp) << "\n";
            }
            
            else if (temp.token.ID == INT_tk)
            {
                target_file << "LOAD " << get_token_int(temp.token) << "\n";
            }
            return;
        }
    }
    
    // <stats> -> <stat> <mStat>
    if (root->label == "<stats>")
    {
        staticSemantics(root->child_1);
        staticSemantics(root->child_2);
    }
    
    // <stat> -> <in> | <out> | <block> | <if> | <loop> | <assign>
    if (root->label == "<stat>")
    {
        staticSemantics(root->child_1);
    }
    
    // <mStat> -> empty | <stat> <mStat>
    if (root->label == "<mStat>")
    {
        if (root->tokens.empty())
        {
            staticSemantics(root->child_1);
            staticSemantics(root->child_2);
        }
    }
    
    // <in> -> read ( Identifier ):
    if (root->label == "<in>")
    {
        temp.token = root->tokens.front();
        temp.scope = var_scope;
        
        int local_var = temp_vars;
        temp_vars++;
        compare_scope(temp);
        
        target_file << "READ T" << local_var << "\n";
        target_file << "LOAD T" << local_var << "\n";
        target_file << "STACKW " << find_on_stack(temp) << "\n";
        
        return;
    }
    
    // <out> -> print ( <expr> ) :
    if (root->label == "<out>")
    {
        int local_var = temp_vars;
        temp_vars++;
        
        staticSemantics(root->child_1);
        
        target_file << "STORE T" << local_var << "\n";
        target_file << "WRITE T" << local_var << "\n";
        
        return;
    }
    
    // <if> -> cond (<expr> <RO> <expr>) <stat>
    if (root->label == "<if>")
    {
        int loop = ++num_repeat;
        int local_var = temp_vars;
        temp_vars++;
        
        target_file << "loop" << loop << ": ";
        staticSemantics(root->child_1);
        
        target_file << "STORE T" << local_var << "\n";
        staticSemantics(root->child_3);
        target_file << "SUB T" << local_var << "\n";
        
        staticSemantics(root->child_2);
        staticSemantics(root->child_4);
        target_file << "branch" << loop << ": NOOP\n";
        
        return;
    }
    
    // <loop> -> iter (<expr> <RO> <expr>) <stat>
    if (root->label == "<loop>")
    {
        int loop = ++num_repeat;
        int local_var = temp_vars;
        temp_vars++;
        
        target_file << "loop" << local_var << ": ";
        
        staticSemantics(root->child_1);
        target_file << "STORE T" << local_var << "\n";
        
        staticSemantics(root->child_3);
        target_file << "SUB T" << local_var << "\n";
        
        staticSemantics(root->child_2);
        staticSemantics(root->child_4);
        target_file << "BR loop" << loop << "\n";
        target_file << "branch" << loop << ": NOOP\n";
    }
    
    // <assign> -> Identifier = <expr> :
    if (root->label == "<assign>")
    {
        temp.token = root->tokens.front();
        temp.scope = var_scope;
        compare_scope(temp);
    
        staticSemantics(root->child_1);
        
        temp.token = root->tokens.front();
        temp.scope = var_scope;
        target_file << "STACKW " << find_on_stack(temp) << "\n";
        
        return;
    }
    
    if (root->label == "<RO>")
    {
        temp.token = root->tokens.front();
        temp.scope = var_scope;
        root->tokens.erase(root->tokens.begin());
        if (operator_map[temp.token.desc] == "LESS_tk")
        {
            if (!root->tokens.empty())
            {
                stack_tt temp_2;
                temp_2.token = root->tokens.front();
                temp_2.scope = var_scope;
                root->tokens.erase(root->tokens.begin());
                
                if (operator_map[temp_2.token.desc] == "EQUALS_tk")
                {
                    target_file << "BRNEG branch" << num_repeat << "\n";
                    return;
                }
            }
            
            target_file << "BRZNEG branch" << num_repeat << "\n";
            return;
        }
        else if (operator_map[temp.token.desc] == "GREAT_tk")
        {
            if (!root->tokens.empty())
            {
                stack_tt temp_2;
                temp_2.token = root->tokens.front();
                temp_2.scope = var_scope;
                root->tokens.erase(root->tokens.begin());
                
                cout << "RO production: token is " << temp_2.token.desc << "\n";
                
                if (operator_map[temp_2.token.desc] == "EQUALS_tk")
                {
                    target_file << "BRPOS branch" << num_repeat << "\n";
                    return;
                }
            }
            target_file << "BRZPOS branch" << num_repeat << "\n";
            return;
        }
        else if (operator_map[temp.token.desc] == "EQUALS_tk")
        {
            if (!root->tokens.empty())
            {
                stack_tt temp_2;
                temp_2.token = root->tokens.front();
                temp_2.scope = var_scope;
                root->tokens.erase(root->tokens.begin());
                
                if (operator_map[temp_2.token.desc] == "EQUALS_tk")
                {
                    target_file << "BRZERO branch" << num_repeat << "\n";
                    return;
                }
            }
            
            target_file << "BRPOS branch" << num_repeat << "\n";
            target_file << "BRNEG branch" << num_repeat << "\n";
            return;
        }
    }
}
// -----------------------------------------------
//  check top of stack var
void check_var(stack_tt var)
{
    int var_defined = find_var(var);
    if (var_defined > 0)
    {
        cout << "Error: staticSemantics variable \'" <<
        get_token_desc(var.token) << "\' on line " << var.token.line_number
        << " has already been defined in this scope on line "
        << stackk.at(var_defined).token.line_number << ".\n";
        exit(EXIT_FAILURE);
    }
    else
    {
        stackk.push_back(var);
        stack_location++;
        target_file << "PUSH\n";
    }
}

// ------------------------------------------------------
//  check if var exists on stack
int check_var_exists(stack_tt var)
{
    for (int counter = 0; counter < stackk.size(); counter++)
    {
        if (stackk.at(counter).token.desc == var.token.desc)
            return counter;
    }
    return -1;
}

// -------------------------------------------------------
// find var on stack
int find_var(stack_tt var)
{
    for (int counter = 0; counter < stackk.size(); counter++)
    {
        if ((stackk.at(counter).token.desc == var.token.desc) &&
            (stackk.at(counter).scope == var.scope))
        {
            return counter;
        }
    }
    
    return -1;
}
// -------------------------------------------------------
// find the location on stack
int find_on_stack(stack_tt var)
{
    for (int counter = 0; counter < stackk.size(); counter++)
    {
        if ((stackk.at(counter).token.desc == var.token.desc) &&
            (stackk.at(counter).scope <= var.scope))
            return counter;
    }
    cout << "The identifier " << var.token.desc << " was not declared in "
    << "this scope.\n";
    exit(EXIT_FAILURE);
}



// delete all vars leaving block
void remove_local_vars(int scope)
{
    if (scope > 0)
    {
        for (int counter = 0; counter < stackk.size(); counter++)
        {
            if (stackk.at(counter).scope == scope)
            {
                target_file << "POP\n";
                stackk.erase(stackk.begin() + counter);
            }
        }
    }
}

// ---------------------------------------------------------
// function to print stack
void print_stack()
{
    for (int i = 0; i < stackk.size(); i++)
    {
        cout << "\tIndex " << i << " = " << stackk.at(i).token.desc <<
        ", scope is " << stackk.at(i).scope << "\n";
    }
}
