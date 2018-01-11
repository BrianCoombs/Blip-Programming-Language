//Project 8 Interpreter
//Brian Coombs
//EE 312

#include <iostream>
#include "Interpreter.h"
#include "Parse.h"
#include "String.h"
#include "ExpTree.h"
#include <map>

map<String, int> vars;
bool readingFromDoVector = false;
//Counts loops by each do for debugging
map<int, int> currentTokenInDoLoop;
map<int, int> currExprInDoLoop;
vector<vector<String>> tokensLists;
vector<vector<ExpTree*>> expressionsLists;
int currentDoVector = 0;
int numDoVectors = 0;
String tokenStr = "";
int openNestedIfs = 0;

/**
 * Runs the process that checks each of the first 'name' tokens to decide results of parsing
 */

void run(void){
    checkComment();
    read_next_token();
    checkComment();


    while(next_token_type != END){
        //printVars(vars);
        //std::cout<<next_token_type<<std::endl;
        checkComment();
        tokenStr = next_token();
        if(strCompare(tokenStr, "text")){
            //std::cout<<"TEXT"<<std::endl;
            text();
        }
        else if(strCompare(tokenStr, "output")){
            //std::cout<<"OUTPUT"<<std::endl;
            output();
        }
        else if(strCompare(tokenStr, "var")){
            //std::cout<<"VAR"<<std::endl;
            var();
        }
        else if(strCompare(tokenStr, "set")){
            //std::cout<<"SET"<<std::endl;
            set();
        }
        else if(strCompare(tokenStr, "do")){
            //std::cout<<"DO"<<std::endl;
            if(readingFromDoVector){
                currentDoVector++;
                currExprInDoLoop[currentDoVector] = 1;
                currentTokenInDoLoop[currentDoVector] =0;
                read_next_token();
            } else{
                whileLoop();
                currentDoVector = 0;
                currentTokenInDoLoop[0]=0;
                currExprInDoLoop[currentDoVector] = 1;
                readingFromDoVector = true;
                read_next_token();
                //End it immedietly if condition not met
                if(expressionsLists[currentDoVector][0]->parse() == 0){
                    endWhile();
                }
            }

        } else if(strCompare(tokenStr, "od")){
            endWhile();
        } else if(strCompare(tokenStr, "if")){
            ifState();
        } else if(strCompare(tokenStr, "fi")){
            endIf();
        } else if(strCompare(tokenStr, "else")){
            elseState();
        }
        else{
            //cout<<"NEXT"<<std::endl;
            read_next_token();
        }
    }
}

/**********************************************************************************************************************
 * Primary Functions
 *********************************************************************************************************************/

void text(void){
    read_next_token();
    //std::cout<<next_token_type<<std::endl; //debug
    //Must be a string for text to process
    if(next_token_type == NAME || next_token_type == SYMBOL){
        std::cout<<next_token()<<std::endl;
    }
    checkComment();
    read_next_token();

}

void output(void){
    //Creates a vector (resizable list) for filling with nodes of operators and operands
    read_next_token();
    //If it's a single variable then just output the variable
    if(vars.count(next_token())){
        std::cout<<vars[next_token()]<<std::endl;
    }
    //If it's just a single number, output it without creating exp tree
    else if(next_token_type == NUMBER){
        std::cout<<token_number_value<<std::endl;
    }
    //Make a new exp tree with that symbol as the root Node
    else if(next_token_type == SYMBOL){
        //Output the parsed version
        ExpTree* newTree = createOrLoadExpTree();
        std::cout<< newTree->parse() <<std::endl;
        checkComment();
        return;
    }
    checkComment();
    read_next_token();

}

void var(void) {
    read_next_token();
    String var = next_token();
    //cout << var.c_str() << std::endl;
    //Check if var is already in map
    if (vars.count(var) > 1) {
        cout << "variable " << var.c_str() << " incorrectly re-initialized" << endl;
        read_next_token();
        return;
    }
    read_next_token();
    //If it's just a single number, output it without creating exp tree
    if (next_token_type == NUMBER) {
        vars[var] = token_number_value;
        //cout<<token_number_value<<endl;
    }
    //Make a new exp tree with that symbol as the root Node
    else if (next_token_type == SYMBOL) {
        //Load the parsed expression's value into the variable's storage
        vars[var] = createOrLoadExpTree()->parse();
        return;
    } else if(next_token_type = NAME){
        vars[var] = vars[next_token()];
    }
    read_next_token();

}
void set(void){
    read_next_token();
    String var = String(next_token());
    read_next_token();
    if (next_token_type == NUMBER) {
        vars[var] = token_number_value;
        //cout<<token_number_value<<endl;
    }
    //Make a new exp tree with that symbol as the root Node
    if (next_token_type == SYMBOL) {
        //Load the parsed expression's value into the variable's storage
        ExpTree* out = createOrLoadExpTree();
        vars[var] = out->parse();
        //cout<<"x has been set to" << vars[var] <<endl;

    }
    //felse if(next_token_type == )
}

void whileLoop(void){
    vector<ExpTree*> expressions;
    vector<String> tokens;
    numDoVectors++;
    currentDoVector=numDoVectors-1;
    read_next_token();

    //Creates spots in maps for this do loop
    //Skips first expr bc it is only used to eval moving on
    currExprInDoLoop[currentDoVector] = 1;

    //Adds expression to front of expression list
    if(next_token_type == NUMBER) {
        if (token_number_value != 0) {
            cout << "Error, this do function produces an infinite loop" << endl;
            read_next_token();
            return;
        } else {
            cout << "Do function skipped because expression evaluated to false" << endl;
            read_next_token();
            return;
        }
    }
    expressions.push_back(createOrLoadExpTree());
    expressionsLists.push_back(expressions);



    //Returns list of tokens and adds it to overall list of do token lists, also loads all other expr in
    createTokenLists();
    //Then read from tokens and bring on the first of the stored tokens


}

void endWhile(void){
    //If the expr for the do vector is positive, then reset to top of it
    int exprValue = expressionsLists[currentDoVector][0]->parse();
    //Check if expr is changing
    //Handle infinite loops
    if(tokensLists[currentDoVector].size() <=1){
        exprValue = 0;
    }
    if(exprValue > 0){
        currentTokenInDoLoop[currentDoVector]=0;
        currExprInDoLoop[currentDoVector] = 1;
        read_next_token();
        return;

    } else {
        currentDoVector--;
        //If this is the last outer loop, clear the entire set of do vectors and their maps
        if (expressionsLists[0][0]->parse() == 0) {
            currentTokenInDoLoop.clear();
            readingFromDoVector = false;
            currExprInDoLoop.clear();
            expressionsLists.clear();
            tokensLists.clear();
            currentDoVector = -1;
            numDoVectors = 0;
        }
        read_next_token();
    }

    //List of token within while loop
    //Variable to be counted when token list created
    //Deals with infinite loop from just number expression




}


void ifState(){
    read_next_token();
    //If true, then just continue parsing
    if(next_token_type == NUMBER){
        if(token_number_value != 0){
            return;
        }
        //If its a var
    } else if(next_token_type == NAME){
        if(vars[next_token()] != 0){
            return;
        }
        //If it's a symbol
    } else if(createOrLoadExpTree()->parse()>0){
        openNestedIfs++;
        return;
    }
    skipUntilReachFiOrElse();
    openNestedIfs++;
    return;


}

void endIf(){
    openNestedIfs--;
    read_next_token();
}

void elseState(){   //Deal with state where its true but else is still there
    if(openNestedIfs>=0){
        skipNestedIfsUntilLastFi();
        openNestedIfs--;
    }
}

/** Checks for comments on that line and skips it/moves to next line if there is a comment
 *
 */
void checkComment()
{
    if(strCompare(String(next_token()), "//"))
    {
        skip_line();
        read_next_token();
    }
}


/**********************************************************************************************************************
 * HELPER FUNCTIONS
 *********************************************************************************************************************/


/** strCompare
 * Compares two strings
 * @param token
 * @param exp
 * @return
 */

bool strCompare(String token, String exp) {
    if(token == exp){
        return true;
    } else{
        return false;
    }
}


/**createExpTree
 * Creates an exp starting with the first symbol as the root
 * @return
 */
ExpTree* createOrLoadExpTree(void){
    //Load the exptree if executing do after first loop instead of creating new one
    if(readingFromDoVector){
        while(!checkValidKeyWord()){
            read_next_token();
        }
        return expressionsLists.at(currentDoVector).at(currExprInDoLoop[currentDoVector]++);
    }
    vector<Node*> nodes;

    //Parse first root symbol and add it to list of nodes
    if(next_token_type == SYMBOL){
        nodes.push_back(createSymbolNode());
    } else if(next_token_type == NAME && vars.count(next_token())){
        Node *newNode = new Node(true, next_token(), "");
        nodes.push_back(newNode);
    }
    //While the next token are symbols or numbers
    //While there is a number, symbol, or name but not a valid keyword to start a new routine, continue building the exptree
    while((next_token_type == NUMBER || next_token_type == SYMBOL || next_token_type == NAME) && !checkValidKeyWord()){
        read_next_token();
        if(next_token_type == NAME){
            //If the next token is in the vars aka a name, then pull it's current value from the vars and add it to exptree
            String token = next_token();
            if(vars.count(token)){
                Node *newNode = new Node(true, token, "");
                nodes.push_back(newNode);
            }
        } else if(next_token_type == NUMBER){
            //If the next token is a number, then push a new operand node containing it
            Node *newNode = new Node(true, token_number_value, "");
            nodes.push_back(newNode);
        } else if(next_token_type == SYMBOL) {
            nodes.push_back(createSymbolNode());
        }

    }
    //Generate the expression tree out of those nodes
    ExpTree* expr = new ExpTree(nodes);
    //Loads this expr into the do loop

    //Return the integer value of that entire ExpTree
    return expr;
}

/** checkValidKeyWord
 * Checks to see if the next keyword is something unrelated to an exp and is instead a new routine
 * @return
 */
bool checkValidKeyWord(void){
    if(strCompare(next_token(), "text")||strCompare(next_token(), "output") || strCompare(next_token(), "var") || strCompare(next_token(), "set")|| strCompare(next_token(), "do")|| strCompare(next_token(), "od")|| strCompare(next_token(), "if")|| strCompare(next_token(), "fi" ) || strCompare(next_token(), "else" )){
        return true;
    }
    else{
        return false;
    }
}

Node* createSymbolNode(){
    //If the next token is a operator, then push a new operator node containing it
    String *newString = new String(next_token());
    Node *root;
    //Handles uranary operators
    if(*newString == "~" || *newString == "!"){
        root = new Node(false, 0, *newString, true);
    }else{
        root = new Node(false, 0, *newString);
    }
    return root;
}


void createTokenLists(){
    vector<String> tokens;
    //Collects all the token into a vector until the end of this do
    bool done = false;
    int thisDoVectorNum = currentDoVector;
    tokensLists.push_back(tokens);
    while(!done){

        //If there is an inner do, then start skipping their tokens
        String s = next_token();
        int type = next_token_type;
        if(s == "do"){
            //Add new layer
            whileLoop();
            //Puts expr in and then checks for deeper
        }
        else if(s == "od") {
            //First time it encounters its own od, return and push it to its stack
            tokensLists[thisDoVectorNum].push_back(s);
            return;

        }
        if(type == SYMBOL && s.size() <=2 && s != "\n"){
            expressionsLists[thisDoVectorNum].push_back(createOrLoadExpTree());
        }else{
            read_next_token();
        }
        tokensLists[thisDoVectorNum].push_back(s);

    }
    return;
}

bool skipUntilReachFiOrElse(){
    int nestedIfLoops = 0;
    //Must be the outer if and a closer before ending
    while((!strCompare(next_token(), "fi")) || nestedIfLoops != -1){ //While the token is not equal to fi, or not equal to -1
        if(strCompare(next_token(), "if")){
            nestedIfLoops++;
        } else if(strCompare(next_token(), "fi")){
            nestedIfLoops--;
            if(nestedIfLoops == -1){
                return true;
            }
        } else if(strCompare(next_token(), "else") && nestedIfLoops == 0){
            read_next_token();
            return false;   //means else reached
        }
        read_next_token();
        if(next_token_type == SYMBOL && String(next_token()).size() <= 2 && String(next_token()) != "\n"){
            currExprInDoLoop[currentDoVector]++;
            while(!checkValidKeyWord()){
                read_next_token();
            }

        }
    }
    return true;   //Means fi reached
}

void skipNestedIfsUntilLastFi(){
    int nestedIfLoops = 0;
    //Must be the outer if and a closer before ending
    while(nestedIfLoops != -1){ //While it's not fi and not -1 keep repeating
        if(next_token_type == SYMBOL && String(next_token()).size() <= 2 && String(next_token()) != "\n"){
            currExprInDoLoop[currentDoVector]++;
            while(!checkValidKeyWord()){
                read_next_token();
            }
        }
        if(strCompare(next_token(), "if")){
            nestedIfLoops++;
        } else if(strCompare(next_token(), "fi")) {
            nestedIfLoops--;
            if(nestedIfLoops == -1){
                read_next_token();
                return;
            }
        }
        read_next_token();
    }
}


/**********************************************************************************************************************
 * PRINT FUNCTIONS
 *********************************************************************************************************************/

/** Print vars
 *
 */

void printVars(map<String, int> vars){
    for(map<String, int >::const_iterator it = vars.begin();
        it != vars.end(); ++it){
        std::cout << it->first.c_str() << " " << it->second << "\n";
    }
}

void printExprList(vector<ExpTree*> exprList){
    for(vector<ExpTree*>::const_iterator it = exprList.begin();
        it != exprList.end(); ++it){
        (*it)->printTree();
    }
}