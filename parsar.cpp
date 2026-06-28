#include "parsar.h"
#include <iostream>
#include <map>
using namespace std;
map<string, int> variables;
map<string, string> Variables;
map<string, string> boolVariable;
map<string, vector<TOKEN>> functions;
map<string, vector<TOKEN>> parameter;
int getvalue(string s)
{
    cout<< "["<<s<<"called ]"<<endl;
    if (variables.find(s) != variables.end())
    {
        return variables[s];
    }
    if (Variables.find(s) != Variables.end())
    {
        return stoi(Variables[s]);
    }
    return 0;
}
bool checkcondition(string left, string right, string op)
{
    int l = getvalue(left);
    int r = getvalue(right);
    if (op == ">")
        return l > r;
    if (op == "<")
        return l < r;
    if (op == "=")
        return l == r;
    if (op == "!")
        return l != r;
    return false;
}
void parse(vector<TOKEN> &tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        //? USES OF LET :-
        if (tokens[i].type == "LET")
        {
            string varname = tokens[i + 1].value;
            if (tokens[i + 3].type == "NUMBER")
            {
                variables[varname] = stoi(tokens[i + 3].value);
            }
            else if (tokens[i + 3].type == "IDENTIFIER" && i + 4 < tokens.size() && tokens[i + 4].type == "SEMI_COLON")
            {
                variables[varname] = getvalue(tokens[i + 3].value);
            }
            else
            {
                int result = getvalue(tokens[i + 3].value);
                int pos = i + 4;
                while (pos < tokens.size() && tokens[pos].type != "SEMI_COLON")
                {
                    string op = tokens[pos].value;
                    pos++;
                    int val = getvalue(tokens[pos].value);
                    if (op == "+")
                        result += val;
                    else if (op == "-")
                        result -= val;
                    else if (op == "*")
                        result *= val;
                    else if (op == "/")
                        result /= val;
                    pos++;
                }
                variables[varname] = result;
            }
        }
        else if (i > 0 && i + 2 < tokens.size() && tokens[i].type == "IDENTIFIER" && tokens[i - 1].type != "LET")
        {
            string result = "";
            int pos = i + 2;
            if (tokens[pos].type == "IDENTIFIER")
                result = to_string(variables[tokens[pos].value]);
            else
                result = tokens[pos].value;
            pos++;
            while (pos < tokens.size() && tokens[pos].type != "SEMI_COLON")
            {
                pos++;
                if (tokens[pos].type == "ADDITION")
                {
                    result = to_string(stoi(result) + getvalue(tokens[pos].value));
                }
            }
            cout<<"["<<result<<"]"<<endl;
            variables[tokens[i].value]=stoi(result);
        }
        else if (i > 0 && i + 2 < tokens.size() && tokens[i].type == "IDENTIFIER" && tokens[i + 1].type == "EQUAL" && tokens[i + 2].type == "NUMBER" && tokens[i - 1].type != "LET" && tokens[i - 1].type != "BOOLEAN")
        {
            if (variables.find(tokens[i].value) != variables.end())
            {
                // cout<<"stoi 9"<<tokens[i+2].value<<"]"<<endl;
                variables[tokens[i].value] = stoi(tokens[i + 2].value);
                // cout << variables[tokens[i].value] << endl;
            }
            else if (boolVariable.find(tokens[i].value) != boolVariable.end())
            {
                cout << "ERROR :- CANNOT ASSIGN A INT TO BOOL VARIABLE" << endl;
            }
            else
            {
                cout << "variable is not declared correctly , use let or bool" << endl;
            }
        }
        else if (tokens[i].type == "IDENTIFIER" && i + 2 < tokens.size() && tokens[i + 1].type == "EQUAL" && tokens[i + 2].type == "IDENTIFIER" && tokens[i - 1].type != "LET" && tokens[i - 1].type != "BOOLEAN")
        {
            if (variables.find(tokens[i].value) != variables.end() && variables.find(tokens[i + 2].value) != variables.end())
            {
                variables[tokens[i].value] = variables[tokens[i + 2].value];
            }
            else
            {
                cout << "variable not declared" << endl;
            }
        }
        else if (tokens[i].type == "IDENTIFIER" && i + 2 < tokens.size() && tokens[i + 1].type == "EQUAL" && tokens[i + 2].type == "BOL_VALUES" && tokens[i - 1].type != "LET" && tokens[i - 1].type != "BOOLEAN")
        {
            if (boolVariable.find(tokens[i].value) != boolVariable.end())
            {
                boolVariable[tokens[i].value] = tokens[i + 2].value;
            }
            else
            {
                cout << "ERROR DECLARATION REQUIRED" << endl;
            }
        }
        if (tokens[i].type == "BOOLEAN")
        { //? USES OF BOOLEAN
            if (tokens[i + 3].value == "true")
            {
                string varname = tokens[i + 1].value;
                boolVariable[varname] = "1";
            }
            else if (tokens[i + 3].value == "false")
            {
                string varname = tokens[i + 1].value;
                boolVariable[varname] = "0";
            }
            boolVariable[tokens[i + 1].value] = tokens[i + 3].value;
        }
        //? FOR PRINT STATEMENTS:-
        if (tokens[i].type == "PRINT")
        {
            int pos = i + 2;
            if (tokens[i + 2].type == "IDENTIFIER" && tokens[i + 3].value == ")")
            {
                cout << variables[tokens[i + 2].value] << endl;
            }
            else if (tokens[i + 3].type == "ADDITION")
            {
                string result = "";
                if (tokens[pos].type == "IDENTIFIER")
                    result = to_string(variables[tokens[pos].value]);
                else
                    result = tokens[pos].value;
                pos++;
                while (tokens[pos].type != "RIGHT_PARA")
                {
                    if (tokens[pos].type == "ADDITION")
                    {
                        pos++;
                        cout << "stoi 10" << result << "]" << endl;
                        result = to_string(stoi(result) + getvalue(tokens[pos].value));
                    }
                    pos++;
                }
                cout << result << endl;
            }
            else if (tokens[i + 3].type == "SUBTRACTION")
            {
                string result = "";
                if (tokens[pos].type == "IDENTIFIER")
                    result = to_string(variables[tokens[pos].value]);
                else
                    result = tokens[pos].value;
                pos++;
                while (pos < tokens.size() && tokens[pos].type != "RIGHT_PARA")
                {
                    if (tokens[pos].type == "SUBTRACTION")
                    {
                        pos++;
                        cout << "stoi 11" << result << "]" << endl;
                        result = to_string(stoi(result) - getvalue(tokens[pos].value));
                    }
                    pos++;
                }
                cout << result << endl;
            }
            else if (tokens[i + 3].type == "MULTIPLY")
            {
                string result = "";
                if (tokens[pos].type == "IDENTIFIER")
                    result = to_string(variables[tokens[pos].value]);
                else
                    result = tokens[pos].value;
                pos++;
                while (tokens[pos].type != "RIGHT_PARA")
                {
                    if (tokens[pos].type == "MULTIPLY")
                    {
                        pos++;
                        cout << "stoi 12" << result << "]" << endl;
                        result = to_string(stoi(result) * getvalue(tokens[pos].value));
                    }
                    pos++;
                }
                cout << result << endl;
            }
            else if (tokens[i + 3].type == "DIVISION")
            {
                string result = "";
                if (tokens[pos].type == "IDENTIFIER")
                    result = to_string(variables[tokens[pos].value]);
                else
                    result = tokens[pos].value;
                pos++;
                while (tokens[pos].type != "RIGHT_PARA")
                {
                    if (tokens[pos].type == "DIVISION")
                    {
                        pos++;
                        cout << "stoi 13" << result << "]" << endl;
                        result = to_string(stoi(result) / getvalue(tokens[pos].value));
                    }
                    pos++;
                }
                cout << result << endl;
            }
            else if (tokens[i + 2].type == "D_COATES")
            {
                string result = "";
                int pos = i + 2;
                pos++;
                while (tokens[pos].value != "\"")
                {
                    result += tokens[pos].value + " ";
                    pos++;
                }
                cout << result << endl;
            }
            else if (i + 3 < tokens.size() && tokens[i + 2].type == "NUMBER" && tokens[i + 3].value == ")")
                cout << variables[tokens[i + 2].value] << endl;
            else if (tokens[i + 2].type == "IDENTIFIER")
            {
                if (variables.find(tokens[i + 2].value) != variables.end())
                {
                    cout << getvalue(tokens[i + 2].value) << endl;
                }
                else if (Variables.find(tokens[i + 2].value) != Variables.end())
                {
                    cout << Variables[tokens[i + 2].value] << endl;
                }
                else if (boolVariable.find(tokens[i + 2].value) != boolVariable.end())
                {
                    cout << boolVariable[tokens[i + 2].value] << endl;
                }
                else if (!(boolVariable.find(tokens[i + 2].value) != boolVariable.end() || variables.find(tokens[i + 2].value) != variables.end()))
                {
                    cout << "ERROR DECLARE THE " << tokens[i + 2].value << endl;
                }
            }
        }
        //? FOR CONDITIONS STATEMENT:-
        if (tokens[i].type == "CONDITION" && i + 4 < tokens.size())
        {
            string left = tokens[i + 2].value;
            string op = tokens[i + 3].type;
            string right = tokens[i + 4].value;
            vector<TOKEN> temp;
            vector<TOKEN> temp_else;
            int pos = i;
            while (tokens[pos].type != "LEFT_CURLY")
                pos++;
            pos++;
            if (op == "GREATER")
            {
                if (getvalue(left) > getvalue(right))
                {
                    while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                    {
                        temp.push_back({tokens[pos].type, tokens[pos].value});
                        pos++;
                    }
                    pos++;
                    if (pos < tokens.size() && tokens[pos].type == "ELSE")
                    {
                        pos++;
                        while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                        {
                            pos++;
                        }
                        pos++;
                    }
                    i = pos - 1;
                    parse(temp);
                }
                else if (!(getvalue(left) > getvalue(right)))
                {
                    while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                    {
                        pos++;
                    }
                    pos++;
                    if (pos < tokens.size() && tokens[pos].type == "ELSE")
                    {
                        while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                        {
                            temp_else.push_back({tokens[pos].type, tokens[pos].value});
                            pos++;
                        }
                        pos++;
                        i = pos - 1;
                        parse(temp_else);
                    }
                    i = pos - 1;
                }
            }
            else if (op == "SMALLER")
            {
                if (getvalue(left) < getvalue(right))
                {
                    while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                    {
                        temp.push_back({tokens[pos].type, tokens[pos].value});
                        pos++;
                    }
                    pos++;
                    if (pos < tokens.size() && tokens[pos].type == "ELSE")
                    {
                        pos++;
                        while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                        {
                            pos++;
                        }
                        pos++;
                    }
                    i = pos - 1;
                    parse(temp);
                }
                else if (!(getvalue(left) < getvalue(right)))
                {
                    while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                    {
                        pos++;
                    }
                    pos++;
                    if (pos < tokens.size() && tokens[pos].type == "ELSE")
                    {
                        while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                        {
                            temp_else.push_back({tokens[pos].type, tokens[pos].value});
                            pos++;
                        }
                        pos++;
                        i = pos - 1;
                        parse(temp_else);
                    }
                    i = pos - 1;
                }
            }
            else if (op == "EQUAL")
            {
                if (getvalue(left) == getvalue(right))
                {
                    while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                    {
                        temp.push_back({tokens[pos].type, tokens[pos].value});
                        pos++;
                    }
                    pos++;
                    if (pos < tokens.size() && tokens[pos].type == "ELSE")
                    {
                        pos++;
                        while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                        {
                            pos++;
                        }
                        pos++;
                    }
                    i = pos - 1;
                    parse(temp);
                }
                else if (!(getvalue(left) == getvalue(right)))
                {
                    while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                    {
                        pos++;
                    }
                    pos++;
                    if (pos < tokens.size() && tokens[pos].type == "ELSE")
                    {
                        while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
                        {
                            temp_else.push_back({tokens[pos].type, tokens[pos].value});
                            pos++;
                        }
                        pos++;
                        i = pos - 1;
                        parse(temp_else);
                    }
                    i = pos - 1;
                }
            }
        }
        //? USES OF LOOP:-
        if (tokens[i].type == "WHILE_LOOP")
        {
            vector<TOKEN> loop;
            string l = "";
            string op = "";
            string r = "";
            if (i + 4 < tokens.size())
            {
                l = tokens[i + 2].value;
                r = tokens[i + 4].value;
                op = tokens[i + 3].value;
            }
            int pos = i + 6;
            pos++;
            while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
            {
                loop.push_back({tokens[pos].type, tokens[pos].value});
                pos++;
            }
            pos++;
            i = pos - 1;
            while (checkcondition(l, r, op))
            {
                parse(loop);
            }
        }
        //? USES OF FUNCTION:-
        if (tokens[i].type == "FUNCTION")
        {
            string f_name = tokens[i + 1].value;
            vector<TOKEN> body;
            vector<TOKEN> para;
            int p = i;
            while (p < tokens.size() && tokens[p].value != "(")
                p++;
            p++;
            while (p < tokens.size() && tokens[p].value != ")")
            {
                para.push_back({tokens[p].type, tokens[p].value});
                p++;
            }
            int pos = p;
            while (pos < tokens.size() && tokens[pos].type != "LEFT_CURLY")
            {
                pos++;
            }
            pos++;
            while (pos < tokens.size() && tokens[pos].type != "RIGHT_CURLY")
            {
                body.push_back({tokens[pos].type, tokens[pos].value});
                pos++;
            }
            functions[f_name] = body;
            parameter[f_name] = para;
            i = pos;
        }
        if (i + 2 < tokens.size() && tokens[i].type == "IDENTIFIER" && tokens[i + 1].value == "(" && tokens[i + 2].value == ")")
        {
            string f_name = tokens[i].value;
            parse(functions[f_name]);
        }
        else if (i + 3 < tokens.size() && tokens[i].type == "IDENTIFIER" && tokens[i + 1].value == "(" && tokens[i + 2].value != ")")
        {
            string f_name = tokens[i].value;
            int p = i + 2;
            vector<string> paras;
            while (p < tokens.size() && tokens[p].value != ")")
            {
                paras.push_back(tokens[p].value);
                p++;
            }
            int index = 0;
            if (parameter.find(f_name) != parameter.end())
            {
                while (index < paras.size() && index < parameter[f_name].size())
                {
                    cout << parameter[f_name][index].value << "->" << paras[index] << endl;
                    index++;
                }
            }
            for (int k = 0; k < paras.size() && k < parameter[f_name].size(); k++)
            {
                Variables[parameter[f_name][k].value] = paras[k];
            }
            parse(functions[f_name]);
        }
    }
    return;
}