//
//  parser.cpp
//  islam.p4
//  Copyright © 2018 Shaon Islam. All rights reserved.
//

#include "parser.hpp"
#include "token.hpp"
#include "scanner.hpp"
#include "node.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

Token tk;
static string expected_token;
static Token EMPTY_TOKEN;

/*__________Init Root of Tree Until EOF__________ */
Node *parser()
{
    EMPTY_TOKEN.desc = "EMPTY";
    scanner(tk);
    
    Node *root = NULL;
    root = program();

    if (tk.ID != EOFtk)
    {
        expected_token.assign("EOF");
        parser_error();
        exit(1);
    }
    else
    {
        return root;
    }
}

/* <program>: void <vars> <block> */
Node *program()
{
    Node *node = create_node("<program>");
    if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "void_tk"))
    {
        scanner(tk);
        node->child_1 = vars();
        node->child_2 = block();
        return node;
    }
    else
    {
        expected_token.assign("void");
        parser_error();
        exit(1);
    }
}

/* <vars>: empty | var Identifier : Integer <vars> */
Node *vars()
{
    Node *node = create_node("<vars>");
    
    if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "var_tk"))
    {
        scanner(tk);
        if (tk.ID == ID_tk)
        {
            node->tokens.push_back(tk);
            scanner(tk);
            
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "colon_tk"))
            {
                scanner(tk);
                
                if (tk.ID == INT_tk)
                {
                    node->tokens.push_back(tk);
                    scanner(tk);
                    node->child_1 = vars();
                    
                    return node;
                }
                else
                {
                    expected_token.assign("Integer");
                    parser_error();
                    exit(1);
                }
            }
            else
            {
                expected_token.assign(":");
                parser_error();
                exit(1);
            }
        }
        else
        {
            expected_token.assign("Identifier");
            parser_error();
            exit(1);
        }
    }
    else
    {
        node->tokens.push_back(EMPTY_TOKEN);
        return node;
    }
}


/* <block>: start <vars> <stats> stop */
Node *block()
{
    Node *node = create_node("<block>");
    
    if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "start_tk"))
    {
        scanner(tk);

        node->child_1 = vars();
        node->child_2 = stats();
        
        if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "stop_tk"))
        {
            scanner(tk);
            return node;
        }
        else
        {
            expected_token.assign("stop");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("start");
        parser_error();
        exit(1);
    }
}

/* <expr>: <A> / <expr> | <A> * <expr> | <A> */
Node *expr()
{
    Node *node = create_node("<expr>");
    node->child_1 = A();
    
    if (tk.ID == OPtk)
    {
        if (operator_map[tk.desc] == "slash_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = expr();
        }
        else if (operator_map[tk.desc] == "asterisk_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = expr();
        }
    }
    
    return node;
}

/* A: <M> + <A> | <M> - <A> | <M> */
Node *A()
{
    Node *node = create_node("<A>");
    node->child_1 = M();
    
    if (tk.ID == OPtk)
    {
        if (operator_map[tk.desc] == "plus_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = A();
        }
        else if (operator_map[tk.desc] == "minus_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = A();
        }
    }
    
    return node;
}

/* <M>: -<M> | <R> */
Node *M()
{
    Node *node = create_node("<M>");
    
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "minus_tk"))
    {
        node->tokens.push_back(tk);
        scanner(tk);
        node->child_1 = M();
        return node;
    }
    
    node->child_1 = R();
    return node;
}

/* <R>: (<expr>) | Identifier | Integer */
Node *R()
{
    Node *node = create_node("<R>");
    
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "leftparenthesis_tk"))
    {
        scanner(tk);
        node->child_1 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "rightparenthesis_tk"))
        {
            scanner(tk);
            return node;
        }
        else
        {
            expected_token.assign(")");
            parser_error();
            exit(1);
        }
    }
    else if (tk.ID == ID_tk)
    {
        node->tokens.push_back(tk);
        scanner(tk);
        return node;
    }
    else if (tk.ID == INT_tk)
    {
        node->tokens.push_back(tk);
        scanner(tk);
        return node;
    }
    else
    {
        expected_token.assign("( or Identifier or Integer");
        parser_error();
        exit(1);
    }
}

/* <stats>: <stat> <mStat> */
Node *stats()
{
    Node *node = create_node("<stats>");
    node->child_1 = stat();
    node->child_2 = mStat();
    
    return node;
}

/* <mStat>: empty | <stat> <mStat> */
Node *mStat()
{
    Node *node = create_node("<mStat>");
    
    if ( ((tk.ID == KEYWORDtk)&&
          ( (keyword_map[tk.desc] == "out_tk") || (keyword_map[tk.desc] == "scan_tk")
           || (keyword_map[tk.desc] == "start_tk")
           || (keyword_map[tk.desc] == "if_tk")
           || (keyword_map[tk.desc] == "loop_tk"))) || (tk.ID == ID_tk))
    {
        node->child_1 = stat();
        node->child_2 = mStat();
        return node;
    }
    else
    {
        node->tokens.push_back(EMPTY_TOKEN);
        return node;
    }
}


/* <stat>: <in> | <out> | <block> | <if> | <block> | <loop> | <assign> */
Node *stat()
{
    Node *node = create_node("<stat>");
    if (tk.ID == KEYWORDtk)
    {
        if (keyword_map[tk.desc] == "out_tk")
        {
            scanner(tk);
            node->child_1 = out();
            return node;
        }
        else if (keyword_map[tk.desc] == "scan_tk")
        {
            scanner(tk);
            node->child_1 = in();
            return node;
        }
        else if (keyword_map[tk.desc] == "start_tk")
        {
            node->child_1 = block();
            return node;
        }
        else if (keyword_map[tk.desc] == "if_tk")
        {
            scanner(tk);
            node->child_1 = ifs();
            return node;
        }
        else if (keyword_map[tk.desc] == "loop_tk")
        {
            scanner(tk);
            node->child_1 = loop();
            return node;
        }
        else if (keyword_map[tk.desc] == "let_tk")
        {
            scanner(tk);
            node->child_1 = assign();
            return node;
        }
    }
        parser_error();
        exit(1);
}

/* <in>: scan Identifier . */
Node *in()
{
    Node *node = create_node("<in>");
        
    if (tk.ID == ID_tk)
    {
        node->tokens.push_back(tk);
        scanner(tk);
            
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "period_tk"))
        {
            scanner(tk);
            return node;
        }
        else
        {
            expected_token.assign(".");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("Identifier");
        parser_error();
        exit(1);
    }
}


/* <out>: out [ <expr>  ] . */
Node *out()
{
    Node *node = create_node("<out>");
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "leftbracket_tk"))
    {
        scanner(tk);
        node->child_1 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "rightbracket_tk"))
        {
            scanner(tk);
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "period_tk"))
            {
                scanner(tk);
                return node;
            }
            else
            {
                expected_token.assign(".");
                parser_error();
                exit(1);
            }
        }
        else
        {
            expected_token.assign("]");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("[");
        parser_error();
        exit(1);
    }
}

/* <if> -> if (<expr> <RO> <expr>) <stat> */
Node *ifs()
{
    Node *node = create_node("<if>");
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "leftparenthesis_tk"))
    {
        scanner(tk);
        
        node->child_1 = expr();
        node->child_2 = RO();
        node->child_3 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "rightparenthesis_tk"))
        {
            scanner(tk);
            node->child_4 = stat();
            
            return node;
        }
        else
        {
            expected_token.assign(")");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("(");
        parser_error();
        exit(1);
    }
}

/* <loop> -> loop (<expr> <RO> <expr>) <stat> */
Node *loop()
{
    Node *node = create_node("<loop>");
    
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "leftparenthesis_tk"))
    {
        scanner(tk);
        
        node->child_1 = expr();
        node->child_2 = RO();
        node->child_3 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "rightparenthesis_tk"))
        {
            scanner(tk);
            node->child_4 = stat();
            
            return node;
        }
        else
        {
            expected_token.assign(")");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("(");
        parser_error();
        exit(1);
    }
}

/* <assign> -> let Identifier = <expr> . */
Node *assign()
{
    Node *node = create_node("<assign>");
    //scanner(tk);
    
    if (tk.ID == ID_tk)
    {
        node->tokens.push_back(tk);
        scanner(tk);

        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "eq_tk"))
        {
            scanner(tk);
            node->child_1 = expr();
        
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "period_tk"))
            {
                scanner(tk);
                return node;
            }
            else
            {
                expected_token.assign(".");
                parser_error();
                exit(1);
            }
        }
        else
        {
            expected_token.assign("=");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("Identifier");
        parser_error();
        exit(1);
    }
}

/* <RO> -> < | < = | > | > = | = = | = */
Node *RO()
{
    Node *node = create_node("<RO>");
    //check for operator tokens
    if (tk.ID == OPtk)
    {
        if (operator_map[tk.desc] == "lessthan_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "eq_tk"))
            {
                node->tokens.push_back(tk);
                scanner(tk);
                return node;
            }
            else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "eq_tk")
                     && (operator_map[tk.desc] != "leftparenthesis_tk"))
            {
                expected_token.assign("< =");
                parser_error();
                exit(1);
            }
            else
                return node;
        }
        else if (operator_map[tk.desc] == "greaterthan_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "eq_tk"))
            {
                node->tokens.push_back(tk);
                scanner(tk);
                return node;
            }
            else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "eq_tk")
                     && (operator_map[tk.desc] != "leftparenthesis_tk"))
            {
                expected_token.assign("> = ");
                parser_error();
                exit(1);
            }
            else
                return node;
        }
        else if (operator_map[tk.desc] == "eq_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "eq_tk"))
            {
                node->tokens.push_back(tk);
                scanner(tk);
                return node;
            }
            else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "eq_tk")
                     && (operator_map[tk.desc] != "leftparenthesis_tk"))
            {
                expected_token.assign("= =");
                parser_error();
                exit(1);
            }
            else
                return node;
        }
        else
        {
            expected_token.assign("< | < = | > | > = | = | = = ");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("< | < = | > | > = | = | = =");
        parser_error();
        exit(1);
    }
}

/*______ERROR_________*/
void parser_error()
{
    cout << "ERROR: From Parser: Line # " << tk.line_number <<
    ":\nshould be " << expected_token << "\ninstead:  " << tk.desc << "\n";
    exit(EXIT_FAILURE);
}


/* _______Init New Nodes________ */
Node *create_node(string production_name)
{
    Node *node = new Node();
    node->child_1 = NULL;
    node->child_2 = NULL;
    node->child_3 = NULL;
    node->child_4 = NULL;
    
    node->label = production_name;
    
    return node;
}
