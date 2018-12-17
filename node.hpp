//
//  node.hpp
//  islam.p4
//
//  Created by Shaon Islam on 12/16/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#ifndef node_hpp
#define node_hpp

#include <set>
#include <vector>
#include <string>

#include "token.hpp"

using namespace std;

struct node_t
{
    
    string label;
    int token_length;
    int level;
    
    vector<Token> tokens;
    
    struct node_t *child_1,
    *child_2,
    *child_3,
    *child_4;
};

#endif /* node_hpp */
