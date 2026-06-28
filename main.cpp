#include <iostream>
#include <vector>
#include "lexer.h"
#include "parsar.h"
#include<map>
using namespace std;
int main() {
    vector<TOKEN> tokens = lexer("test.ghost");
    parse(tokens);
    return 0;
}