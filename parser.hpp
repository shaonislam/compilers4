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

node_t *parser();
node_t *program();
node_t *block();
node_t *vars();
node_t *expr();
node_t *A();
node_t *M();
node_t *R();
node_t *stats();
node_t *mStat();
node_t *stat();
node_t *in();
node_t *out();
node_t *ifs();
node_t *loop();
node_t *assign();
node_t *RO();


void parser_error();

//Create a new node.
node_t *create_node(string);

#endif /* parser_hpp */
