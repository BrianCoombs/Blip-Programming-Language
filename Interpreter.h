//Project 8 Interpreter
//Brian Coombs
//EE 312

#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "String.h"
#include "ExpTree.h"
#include <map>
#include <stdint.h>


extern std::map<String, int> vars;

extern bool readingFromDoVector;
extern map<int, int> currentTokenInDoLoop;
extern vector<vector<String>> tokensLists;
extern int currentDoVector;

void run(void);

//Primary functions

void text(void);

void output(void);

void var(void);

void set(void);

void whileLoop(void);

void endWhile(void);

void checkComment(void);

void ifState(void);

void endIf(void);

void elseState(void);
//Helper functions

bool strCompare(String token, String exp);

ExpTree* createOrLoadExpTree(void);

bool checkValidKeyWord(void);

Node* createSymbolNode(void);

void createTokenLists();

void skipNestedIfsUntilLastFi();

bool skipUntilReachFiOrElse();

void printVars(map<String, int> vars);








#endif //PROJECT_8_INTERPRETER_A_INTERPRETER_H
