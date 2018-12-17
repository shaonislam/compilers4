//
//  testTree.cpp
//  islam.p4
//
//  Created by Shaon Islam on 12/17/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#include "testTree.hpp"
#include <string>
#include <iostream>


//to print the tree
void preorder(node_t *node, int level)
{
    if (node == NULL)
        return;
    
    string line;
    
    for (int counter = 0; counter < level; counter++)
    {
        line.append("  ");
    }
    line.append(node->label);
    line.append(" ");
    
    for (int counter = 0; counter < node->tokens.size(); counter++)
    {
        line.append(node->tokens[counter].desc);
        
        if ( ((counter + 1) != node->tokens.size()) &&
            (node->tokens[counter].ID != OPtk))
            line.append(",");
        line.append(" ");
    }
    cout << line << "\n";
    
    preorder(node->child_1, level + 1);
    preorder(node->child_2, level + 1);
    preorder(node->child_3, level + 1);
    preorder(node->child_4, level + 1);
}
