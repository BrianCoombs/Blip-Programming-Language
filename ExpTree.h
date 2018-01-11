//
// Created by Vallath Nandakumar on 11/15/17.
//

#ifndef DAY22_EXPRESSIONTREE_EXPTREE_H
#define DAY22_EXPRESSIONTREE_EXPTREE_H
#include <vector>
#include <iostream>
#include "String.h"
#include "Parse.h"

using namespace std;

class Node {
public:
    bool isOperand;
    bool hasSingleOperator;
    int operand;
    String var;
    String optr;
    Node* left;
    Node* right;

    //Primary simple constructor
    Node(bool isOperand, int operand, String optr) {
        left = nullptr;
        right = nullptr;
        this->isOperand = isOperand;
        this->operand = operand;
        this->optr = optr;
        this->hasSingleOperator = false;
        this->var = "";
    }
    //Constructor for variables
    Node(bool isOperand, String var, String optr) {
        left = nullptr;
        right = nullptr;
        this->isOperand = isOperand;
        this->var = var;
        this->operand = 0;
        this->optr = optr;
        this->hasSingleOperator=false;
    }
    //Constructor for operators
    Node(bool isOperand, int operand, String optr, bool hasSingleOperator) {
        left = nullptr;
        right = nullptr;
        this->isOperand = isOperand;
        this->operand = operand;
        this->optr = optr;
        this->hasSingleOperator=hasSingleOperator;
        this->var = "";
    }
    void print() {
        if (isOperand) {
            std::cout << operand << std::endl;
        } else {
            std::cout <<optr.c_str()<<std::flush;
        }
    }
};

class ExpTree{
private:
    const int spaces_per_level = 4;
    Node* overallRoot;
    Node* add(Node* root, vector<Node*>&);
    void printTree(Node*, int);
    void printSpaces(int);
    int parse(Node*);
public:
    int parse();
    ExpTree(vector<Node*>& expr); // constructor
    void printTree();
};


#endif //DAY22_EXPRESSIONTREE_EXPTREE_H
