//
//  node.hpp
//  islam.p4
//
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#ifndef node_hpp
#define node_hpp

#include <set>
#include <vector>
#include <string>
#include "token.hpp"

using namespace std;

struct Node
{
    string label;
    int token_length;
    int level;
    vector<Token> tokens;
    struct Node *child_1, *child_2, *child_3, *child_4;
};

#endif /* node_hpp */
