#include <vector>
#include <map>
#include <iostream>
#include "parsar.h"
using namespace std;
vector<TOKEN> body;
vector<TOKEN> para;
map<string, int> variables;
map<string, string> Variables;
map<string, string> boolVariable;
int func_called = 0;
map<string, vector<TOKEN>> functions;
map<string, vector<TOKEN>> parameters;
int return_value = 0;
bool returned = false;

int getvalue(string s)
{
    if (variables.find(s) != variables.end())
    {
        return variables[s];
    }
    try
    {
        return stoi(s);
    }
    catch (exception e)
    {
        return 0;
    }
    return 1;
}
int evaluate(vector<TOKEN> &tokens)
{
    int result = 0;
    if (tokens.empty())
        return 0;
    result = getvalue(tokens[0].value);
    int i = 1;
    while (i < tokens.size())
    {
        string op = tokens[i].value;
        int val = getvalue(tokens[i + 1].value);
        if (op == "+")
            result += val;
        else if (op == "-")
            result -= val;
        else if (op == "*")
            result *= val;
        else if (op == "/")
            result /= val;
        i += 2;
    }
    return result;
}
bool evaluateconditon(vector<TOKEN> &cond)
{
    int left = getvalue(cond[0].value);
    string op = cond[1].value;
    int right = getvalue(cond[2].value);
    // cout<<"["<<op<<"]"<<endl;
    // cout<<left <<" "<<op <<" "<<right<<endl;
    if (op == ">")
        return left > right;
    if (op == "<")
        return left < right;
    if (op == "=")
        return left == right;
    return false;
}
vector<TOKEN> getBlock(vector<TOKEN> &tokens, int &pos)
{
    vector<TOKEN> block;
    int depth = 1;
    pos++;
    while (pos < tokens.size() && depth > 0)
    {
        if (tokens[pos].type == "LEFT_CURLY")
            depth++;
        else if (tokens[pos].type == "RIGHT_CURLY")
            depth--;
        if (depth > 0)
            block.push_back({tokens[pos].type, tokens[pos].value});
        pos++;
    }
    return block;
}
void parse(vector<TOKEN> &tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        if (tokens[i].type == "LET")
        {
            if (i + 3 < tokens.size() && i + 5 < tokens.size() && tokens[i + 1].type == "IDENTIFIER" && tokens[i + 2].type == "EQUAL" && tokens[i + 3].type == "IDENTIFIER" && tokens[i + 4].type == "LEFT_PARA")
            {
                if (functions.find(tokens[i + 3].value) != functions.end())
                {
                    int pos = i + 5;
                    vector<TOKEN> s_c;
                    while (pos < tokens.size() && tokens[pos].type != "RIGHT_PARA")
                    {
                        s_c.push_back({tokens[pos].type, tokens[pos].value});
                        pos++;
                    }
                    if (s_c.size() == para.size())
                    {
                        for (int k = 0; k < s_c.size(); k++)
                        {
                            string param_name = parameters[tokens[i + 3].value][k].value;
                            if (variables.find(s_c[k].value) != variables.end())
                            {
                                variables[param_name] = variables[s_c[k].value];
                            }
                            else if (s_c[k].type == "NUMBER")
                            {
                                variables[param_name] = stoi(s_c[k].value);
                            }
                            else
                            {
                                Variables[param_name] = s_c[k].value;
                            }
                        }
                        parse(functions[tokens[i + 3].value]);
                        variables[tokens[i + 1].value] = return_value;
                    }
                    else
                    {
                        cout << "ERROR:- ARGUMENT MISSMATCH" << endl;
                    }
                }
            }
            else if (tokens[i + 1].type == "IDENTIFIER" && tokens[i + 2].type == "SEMI_COLON")
            {
                variables[tokens[i + 1].value] = 1;
            }
            else
            {
                vector<TOKEN> temp;
                int pos = i + 3;
                while (pos < tokens.size() && tokens[pos].value != ";")
                {
                    temp.push_back({tokens[pos].type, tokens[pos].value});
                    pos++;
                }
                variables[tokens[i + 1].value] = evaluate(temp);
            }
        }
        else if (tokens[i].type == "INPUT")
        {
            int pos = i + 1;
            string input;
            while (pos < tokens.size() && tokens[pos].type != "SEMI_COLON")
            {
                if (tokens[pos].value != ",")
                {
                    cin >> input;
                    variables[tokens[pos].value] = stoi(input);
                }
                pos++;
            }
        }
        else if (tokens[i].type == "RETURN")
        {
            // if(tokens[i+1].type="IDENTIFIER" && tokens[i+2].type="LEFT_PARA"){

            // }
            vector<TOKEN> temp;
            int pos = i + 1;
            while (pos < tokens.size() && tokens[pos].type != "SEMI_COLON")
            {
                temp.push_back({tokens[pos].type, tokens[pos].value});
                pos++;
            }
            int value = evaluate(temp);
            return_value = value;
            return;
        }
        else if (i + 3 < tokens.size() && tokens[i].type == "IDENTIFIER" && tokens[i + 1].type == "EQUAL" && tokens[i + 3].type != "LEFT_PARA")
        {
            vector<TOKEN> temp;
            if (variables.find(tokens[i].value) != variables.end())
            {
                int pos = i + 2;
                while (pos < tokens.size() && tokens[pos].value != ";")
                {
                    temp.push_back({tokens[pos].type, tokens[pos].value});
                    pos++;
                }
                variables[tokens[i].value] = evaluate(temp);
            }
            else
            {
                cout << "DECLARATION ERROR " << endl;
            }
        }
        else if (tokens[i].type == "PRINT")
        {
            if (tokens[i + 2].type == "NUMBER")
            {
                cout << tokens[i + 2].value << endl;
            }
            else if (tokens[i + 2].type == "IDENTIFIER")
            {
                int pos = i + 2;
                vector<TOKEN> temp;
                while (pos < tokens.size() && tokens[pos].type != "RIGHT_PARA")
                {
                    temp.push_back({tokens[pos].type, tokens[pos].value});
                    pos++;
                }
                int a = evaluate(temp);
                if (a == 0)
                {
                    cout << tokens[i + 2].value << "NOT DECLARE" << endl;
                }
                else
                {
                    cout << a << endl;
                }
            }
            else if (tokens[i + 2].type == "D_COATES")
            {
                string result = "";
                int pos = i + 2;
                pos++;
                bool space = false;
                    while (tokens[pos].value != "\"")
                    {
                        if (pos + 2 < tokens.size() && tokens[pos].value == "-" && tokens[pos + 1].value == "n")
                        {
                            space = true;
                            pos += 2;
                        }
                        else
                        {
                            result += tokens[pos].value + " ";
                            pos++;
                        }
                    }
                    int a ;
                    if(pos+1<tokens.size() && tokens[pos+1].type=="IDENTIFIER"){
                        a = getvalue(tokens[pos+1].value);
                        pos++;
                    }
                    if (space)
                        cout << result << endl;
                    else
                        cout << result;
                    cout<< to_string(a);
            }
        }
        else if (tokens[i].type == "CONDITION")
        {
            int pos = i + 2;
            vector<TOKEN> temp;
            while (pos < tokens.size() && tokens[pos].value != ")")
            {
                temp.push_back({tokens[pos].type, tokens[pos].value});
                pos++;
            }
            pos++;
            if (evaluateconditon(temp))
            {
                int a = pos;
                vector<TOKEN> block = getBlock(tokens, a);
                parse(block);
                if (a < tokens.size() && tokens[a].type == "ELSE")
                {
                    a++;
                    getBlock(tokens, a);
                }
                i = a - 1;
            }
            else
            {
                int a = pos;
                getBlock(tokens, a);
                if (a < tokens.size() && tokens[a].type == "ELSE")
                {
                    a++;
                    vector<TOKEN> temp = getBlock(tokens, a);
                    parse(temp);
                }
                i = a - 1;
            }
        }
        else if (tokens[i].type == "WHILE_LOOP")
        {
            int pos = i + 2;
            vector<TOKEN> condition;
            while (pos < tokens.size() && tokens[pos].type != "RIGHT_PARA")
            {
                condition.push_back({tokens[pos].type, tokens[pos].value});
                pos++;
            }
            pos++;
            vector<TOKEN> block = getBlock(tokens, pos);
            i = pos - 1;
            while (evaluateconditon(condition))
            {
                parse(block);
            }
        }
        else if (tokens[i].type == "FUNCTION")
        {
            string f_name = tokens[i + 1].value;
            int pos = i + 3;
            while (pos < tokens.size() && tokens[pos].type != "RIGHT_PARA")
            {
                para.push_back({tokens[pos].type, tokens[pos].value});
                pos++;
            }
            pos++;
            vector<TOKEN> body = getBlock(tokens, pos);
            functions[f_name] = body;
            parameters[f_name] = para;
            i = pos - 1;
        }
        else if (i + 2 < tokens.size() && tokens[i].type == "IDENTIFIER" && tokens[i + 1].value == "(" && tokens[i + 2].type == "RIGHT_PARA")
        {
            parse(functions[tokens[i].value]);
        }
        else if (i > 0 && i + 3 < tokens.size() && tokens[i - 1].type != "EQUAL" && tokens[i].type == "IDENTIFIER" && tokens[i + 1].value == "(" && tokens[i + 2].value != ")")
        {
            if (functions.find(tokens[i].value) != functions.end())
            {
                int pos = i + 2;
                vector<TOKEN> s_c;
                while (pos < tokens.size() && tokens[pos].type != "RIGHT_PARA")
                {
                    s_c.push_back({tokens[pos].type, tokens[pos].value});
                    pos++;
                }
                if (s_c.size() == para.size())
                {
                    for (int k = 0; k < s_c.size(); k++)
                    {
                        string param_name = parameters[tokens[i].value][k].value;
                        if (variables.find(s_c[k].value) != variables.end())
                        {
                            variables[param_name] = variables[s_c[k].value];
                        }
                        else if (s_c[k].type == "NUMBER")
                        {
                            variables[param_name] = stoi(s_c[k].value);
                        }
                        else
                        {
                            Variables[param_name] = s_c[k].value;
                        }
                    }
                    parse(functions[tokens[i].value]);
                }
                else
                {
                    cout << "ERROR:- ARGUMENT MISSMATCH" << endl;
                }
            }
        }
    }
    return;
}