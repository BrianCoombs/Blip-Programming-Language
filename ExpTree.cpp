//
// Created by Vallath Nandakumar on 11/15/17.
//

#include "ExpTree.h"
#include "Interpreter.h"

#define DEBUG true
ExpTree::ExpTree(vector<Node*>& expr) { // construct from expression vector
    overallRoot = add(overallRoot, expr);
}

// private method
Node* ExpTree::add(Node* root, vector<Node*>& expr) { // passing expr as a reference
    Node* first = expr[0]; // store the first element
    expr.erase(expr.begin()); // remove the first element
    root = first; // root points to the first token Node
    if (!(first->isOperand) && !(first->hasSingleOperator)) { // if first is an operator, add 2 operands
        root->left = add(root->left, expr);
        root->right = add(root->right, expr);
    }
    else if(!(first->isOperand)){
        root->left = add(root->left, expr);
    }
    return root; // return the new sub-tree
}

void ExpTree::printTree() {
    printTree(overallRoot, 0); // call helper method
}

void ExpTree::printSpaces(int level) { // print 4 spaces per level, for indentation
    for (int i = 0; i < level*4; i++) {
        cout << ' ';
    }
}

// private recursive helper method
void ExpTree::printTree(Node* root, int level) {
    if (root == nullptr) return;
    printTree(root->right, level+1); // print the right sub-tree
    printSpaces(level); // print the indentation for the root of sub-tree
    root->print(); // print the root
    printTree(root->left, level+1); // print the left sub-tree
}

int ExpTree::parse() {
    return parse(overallRoot);
}

int ExpTree::parse(Node* root) {
    if (root->isOperand) {
        //If this operand is a variable, pull it from the variable map
        if(root->var != ""){
            return vars[root->var];
        }else{
            return root->operand;
        }

    }
    int arg1 = parse(root->left);
    int arg2;
    if(!(root->hasSingleOperator)){
         arg2 = parse(root->right);
    }
    if(root->optr == "+"){
        return arg1+arg2;
    } else if(root->optr == "-"){
        return arg1-arg2;
    } else if(root->optr == "/"){
        return arg1/arg2;
    } else if(root->optr== "*") {
        return arg1 * arg2;
    } else if(root->optr == "=="){
        return arg1 == arg2 ? 1 : 0;
    } else if(root->optr == "&&"){
        return arg1 > 0 && arg2 > 0 ? 1 : 0;
    } else if(root->optr == "||"){
        return (arg1 > 0 || arg2 > 0) ? 1 : 0;
    } else if(root->optr == "!="){
        return (arg1 != arg2) ? 1 : 0;
    } else if(root->optr == ">"){
        return arg1 > arg2 ? 1 : 0;
    } else if(root->optr == "<"){
        return arg1 < arg2 ? 1 : 0;
    } else if(root->optr == "!"){
        return (arg1 != 0) ? 0 : 1;
    } else if(root->optr == "~"){
        return -arg1;
    } else if(root->optr == ">="){
        return (arg1>=arg2);
    }else if(root->optr == "<="){
        return (arg1<=arg2);
    } else if(root->optr == "%"){
        return (arg1%arg2);
    }

}
