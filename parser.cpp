//
//  parser.cpp
//  islam.p4
//
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
node_t *parser()
{
    EMPTY_TOKEN.desc = "EMPTY";
    scanner(tk);
    
    node_t *root = NULL;
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
node_t *program()
{
    node_t *node = create_node("<program>");
    if ((tk.ID == KEYWORDtk) && (keyword_map[tk.desc] == "VOID_tk"))
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
node_t *vars()
{
    node_t *node = create_node("<vars>");
    
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
node_t *block()
{
    node_t *node = create_node("<block>");
    
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
node_t *expr()
{
    node_t *node = create_node("<expr>");
    node->child_1 = A();
    
    if (tk.ID == OPtk)
    {
        if (operator_map[tk.desc] == "SLASH_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = expr();
        }
        else if (operator_map[tk.desc] == "ASTERIK_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = expr();
        }
    }
    
    return node;
}

/* A: <M> + <A> | <M> - <A> | <M> */
node_t *A()
{
    node_t *node = create_node("<A>");
    node->child_1 = M();
    
    if (tk.ID == OPtk)
    {
        if (operator_map[tk.desc] == "PLUS_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = A();
        }
        else if (operator_map[tk.desc] == "MINUS_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            node->child_2 = A();
        }
    }
    
    return node;
}

/* <M>: -<M> | <R> */
node_t *M()
{
    node_t *node = create_node("<M>");
    
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "MINUS_tk"))
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
node_t *R()
{
    node_t *node = create_node("<R>");
    
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
    {
        scanner(tk);
        node->child_1 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
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
node_t *stats()
{
    node_t *node = create_node("<stats>");
    node->child_1 = stat();
    node->child_2 = mStat();
    
    return node;
}

/* <mStat>: empty | <stat> <mStat> */
node_t *mStat()
{
    node_t *node = create_node("<mStat>");
    
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
node_t *stat()
{
    node_t *node = create_node("<stat>");
    cout << "IN stats\n";
    if (tk.ID == KEYWORDtk)
    {
        if (keyword_map[tk.desc] == "out_tk")
        {
            cout << "IN OUT\n";
            scanner(tk);
            node->child_1 = out();
            return node;
        }
        else if (keyword_map[tk.desc] == "scan_tk")
        {
            cout << "IN IN\n";
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
node_t *in()
{
    node_t *node = create_node("<in>");
    //scanner(tk);
        
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
node_t *out()
{
    node_t *node = create_node("<out>");
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OBRACKET_tk"))
    {
        scanner(tk);
        node->child_1 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CBRACKET_tk"))
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
node_t *ifs()
{
    node_t *node = create_node("<if>");
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
    {
        scanner(tk);
        
        node->child_1 = expr();
        node->child_2 = RO();
        node->child_3 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
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
node_t *loop()
{
    node_t *node = create_node("<loop>");
    
    if ((tk.ID == OPtk) && (operator_map[tk.desc] == "OPAR_tk"))
    {
        scanner(tk);
        
        node->child_1 = expr();
        node->child_2 = RO();
        node->child_3 = expr();
        
        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "CPAR_tk"))
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

/* _________________ LET IF STATEMENT_________*/
/* <assign> -> let Identifier = <expr> . */
node_t *assign()
{
    node_t *node = create_node("<assign>");
    //scanner(tk);
    
    if (tk.ID == ID_tk)
    {
        node->tokens.push_back(tk);
        scanner(tk);

        if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
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
node_t *RO()
{
    node_t *node = create_node("<RO>");
    //check for operator tokens
    if (tk.ID == OPtk)
    {
        if (operator_map[tk.desc] == "LESS_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
            {
                node->tokens.push_back(tk);
                scanner(tk);
                return node;
            }
            else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "EQUALS_tk")
                     && (operator_map[tk.desc] != "OPAR_tk"))
            {
                expected_token.assign("< =");
                parser_error();
                exit(1);
            }
            else
                return node;
        }
        else if (operator_map[tk.desc] == "GREAT_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
            {
                node->tokens.push_back(tk);
                scanner(tk);
                return node;
            }
            else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "EQUALS_tk")
                     && (operator_map[tk.desc] != "OPAR_tk"))
            {
                expected_token.assign("> = ");
                parser_error();
                exit(1);
            }
            else
                return node;
        }
        else if (operator_map[tk.desc] == "EQUALS_tk")
        {
            node->tokens.push_back(tk);
            scanner(tk);
            
            if ((tk.ID == OPtk) && (operator_map[tk.desc] == "EQUALS_tk"))
            {
                node->tokens.push_back(tk);
                scanner(tk);
                return node;
            }
            else if ((tk.ID == OPtk) && (operator_map[tk.desc] != "EQUALS_tk")
                     && (operator_map[tk.desc] != "OPAR_tk"))
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
            expected_token.assign("< or < = or > or > = or = or = = ");
            parser_error();
            exit(1);
        }
    }
    else
    {
        expected_token.assign("< or < = or > or > = or = or = =");
        parser_error();
        exit(1);
    }
}

/*______ERROR_________*/
void parser_error()
{
    cout << "Error: parser - line number " << tk.line_number <<
    ": expected '" << expected_token << "' but received '" << tk.desc << "'\n";
    exit(EXIT_FAILURE);
}


/* _______Init New Nodes________ */
node_t *create_node(string production_name)
{
    node_t *node = new node_t();
    node->child_1 = NULL;
    node->child_2 = NULL;
    node->child_3 = NULL;
    node->child_4 = NULL;
    
    node->label = production_name;
    
    return node;
}
