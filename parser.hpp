//
//  parser.hpp
//  islam.p4
//
//  Created by Shaon Islam on 12/16/18.
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#ifndef parser_hpp
#define parser_hpp

#include "token.hpp"
#include "node.hpp"
#include <string>

using namespace std;

extern Token tk;

Node *parser();
Node *program();
Node *block();
Node *vars();
Node *expr();
Node *A();
Node *M();
Node *R();
Node *stats();
Node *mStat();
Node *stat();
Node *in();
Node *out();
Node *ifs();
Node *loop();
Node *assign();
Node *RO();


void parser_error();

//Create a new node.
Node *create_node(string);

#endif /* parser_hpp */
