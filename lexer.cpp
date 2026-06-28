#include<fstream>
#include<cctype>
#include"lexer.h"
vector<TOKEN> lexer (string filename){
    ifstream file("test.ghost");
    string token="";
    vector<TOKEN> tokens; 
    char ch;
    while(file.get(ch)){
        if(isalnum(ch)) token+=ch;
        else {
            if(!token.empty()){
                if(token=="let")tokens.push_back({"LET",token});
                else if(token=="in")tokens.push_back({"INPUT",token});
                else if(token=="print")tokens.push_back({"PRINT", token});
                else if (token == "bool")tokens.push_back({"BOOLEAN",token});
                else if(isdigit(token[0]))tokens.push_back({"NUMBER",token});
                else if(token == "true" || token == "false")tokens.push_back({"BOL_VALUES",token});
                else if(token =="if")tokens.push_back({"CONDITION",token});
                else if(token == "else")tokens.push_back({"ELSE",token});
                else if(token == "while")tokens.push_back({"WHILE_LOOP",token});
                else if(token == "func")tokens.push_back({"FUNCTION",token});
                else if(token == "return")tokens.push_back({"RETURN",token});
                else tokens.push_back({"IDENTIFIER",token});
                token="";
            }
            if(!isspace(ch)){
                if(ch=='=')tokens.push_back({"EQUAL","="});
                else if(ch=='+')tokens.push_back({"ADDITION","+"});
                else if(ch=='-')tokens.push_back({"SUBTRACTION","-"});
                else if(ch=='*')tokens.push_back({"MULTIPLY","*"});
                else if(ch=='/')tokens.push_back({"DIVISION","/"});
                else if(ch=='(')tokens.push_back({"LEFT_PARA","("});
                else if(ch==')')tokens.push_back({"RIGHT_PARA",")"});
                else if(ch=='{')tokens.push_back({"LEFT_CURLY","{}"});
                else if(ch=='}')tokens.push_back({"RIGHT_CURLY","}"});
                else if(ch=='"')tokens.push_back({"D_COATES","\""});
                else if(ch=='>')tokens.push_back({"GREATER",">"});
                else if(ch=='<')tokens.push_back({"SMALLER","<"});
                else if(ch==';')tokens.push_back({"SEMI_COLON",";"});
                else if(ch=='!')tokens.push_back({"NOT_EQUALS","!"});
            }
        }
    }
    if(!token.empty()){
        if(isdigit(token[0]))tokens.push_back({"NUMBER",token});
        else tokens.push_back({"IDENTIFIER",token});
    }
    return tokens;
}