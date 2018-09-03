#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>
#include <stack>

using namespace std;

string poliz;

enum States
{
    State_A = 0, // begin
    State_B,
    State_C,
    State_D,
    State_E,
    State_F,
    State_G,
    State_H
};

void error (int i, int num_error)
{
    cout << "\nerror in column " << i << ":\n";
    if (num_error == 0) cout << "unknown character";
    if (num_error == 1) cout << "incorrect brackets";
    if (num_error == 2) cout << "incorrect ','";
    if (num_error == 3) cout << "unknown function";
    cout << '\n';
    //abort();
    exit(1);
}
  /**   0 - unknown character
    *   1 - incorrect brackets
    *   2 - incorrect ','
    *   3 - unknown function
    */

int priority (string input)
{
    if (input == "^") return 3;
    if (input == "~") return 2;
    if (input == "*" || input == "/") return 1;
    if (input == "+" || input == "-") return 0;
}

void poliz_input (string input, stack <string> &stack_)
{
    //cout << input << '\n';
    if ((input[0] >= '0' && input[0] <= '9') || input[0] == '&'){
        poliz += (input + " ");
    }
    else if (input == ",") {
        while(stack_.top() != "("){
            poliz += (stack_.top() + " ");
            stack_.pop();
        }
    }
    else if (input == "+" || input == "-" || input == "*" || input == "/" || input == "^" || input == "~"){
        if (stack_.empty()) goto end_;
        while (stack_.top() == "+" || stack_.top() == "-" || stack_.top() == "*" || stack_.top() == "/" || stack_.top() == "^" || stack_.top() == "~"){
            if (input == "^"){
                if (priority(input) < priority(stack_.top())){
                    poliz += (stack_.top() + " ");
                    stack_.pop();
                    if (stack_.empty()) break;
                }
                else {
                    break;
                }
            }
            else {
                if (priority(input) <= priority(stack_.top())){
                    poliz += (stack_.top() + " ");
                    stack_.pop();
                    if (stack_.empty()) break;
                }
                else {
                    break;
                }
            }
        }
        end_:;
        stack_.push(input);
    }
    else if (input == "(") {
        stack_.push(input);
    }
    else if (input == ")"){
        while(stack_.top() != "("){
            poliz += (stack_.top() + " ");
            stack_.pop();
        }
        stack_.pop();
        if (!stack_.empty()){
            if (!(stack_.top()[0] >= '0' && stack_.top()[0] <= '9') && stack_.top()[0] != '&' && stack_.top() != "+" && stack_.top() != "-" && stack_.top() != "*" && stack_.top() != "/" && stack_.top() != "^" && stack_.top() != "~" && stack_.top() != "("){
                poliz += (stack_.top() + " ");
                stack_.pop();
            }
        }
    }
    else if (input == "sin" || input == "cos" || input == "ln" || input == "log" || input == "exp" || input == "sqrt"){
        stack_.push(input);
    }
}

void avtomat (string input)
{
    input += ")";
    States state = State_A;
    stack <int> brack;
    brack.push(0);

    string number;

    stack <string> stack_;
    poliz_input("(", stack_);
    // унарный минус ~

    for (unsigned int i=0; i<input.size(); i++){
        const char current = input[i];
        switch(state)
        {
        case State_A:
            if (current == '+'){
                state = State_B;
            }
            else if (current == '-'){
                state = State_B;
                poliz_input("~", stack_);
            }
            else if (current >= '0' && current <= '9'){
                state = State_C;
                number.clear();
                number += current;
            }
            else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')){
                state = State_H;
                number.clear();
                number += current;
            }
            else if (current == '('){
                brack.push(0);
                poliz_input("(", stack_);
            }
            else {
                error(i, 0);
            }

            break;
        case State_B:
            if (current == '('){
                state = State_A;
                brack.push(0);
                poliz_input("(", stack_);
            }
            else if (current >= '0' && current <= '9'){
                state = State_C;
                number.clear();
                number += current;
            }
            else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')){
                state = State_H;
                number.clear();
                number += current;
            }
            else {
                error(i, 0);
            }

            break;
        case State_C:
            if (current >= '0' && current <= '9'){
                number += current;
            }
            else if (current == '.'){
                state = State_D;
                number += current;
            }
            else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                state = State_G;
                poliz_input(number, stack_);
                number.clear();
                if (current == '+') poliz_input("+", stack_);
                else if (current == '-') poliz_input("-", stack_);
                else if (current == '/') poliz_input("/", stack_);
                else if (current == '*') poliz_input("*", stack_);
                else if (current == '^') poliz_input("^", stack_);
            }
            else if (current == ')'){
                state = State_F;
                poliz_input(number, stack_);
                number.clear();
                if (brack.top() == 0){
                    brack.pop();
                    poliz_input(")", stack_);
                }
                else {
                    error(i, 1);
                }
            }
            else if (current == ','){
                state = State_A;
                poliz_input(number, stack_);
                number.clear();
                poliz_input(",", stack_);
                if (brack.top() > 0){
                    brack.top()--;
                }
                else {
                    error(i, 2);
                }
            }
            else {
                error(i, 0);
            }

            break;
        case State_D:
            if (current >= '0' && current <= '9'){
                state = State_E;
                number += current;
            }
            else {
                error(i, 0);
            }

            break;
        case State_E:
            if (current >= '0' && current <= '9'){
                number += current;
            }
            else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                state = State_G;
                poliz_input(number, stack_);
                number.clear();
                if (current == '+') poliz_input("+", stack_);
                else if (current == '-') poliz_input("-", stack_);
                else if (current == '/') poliz_input("/", stack_);
                else if (current == '*') poliz_input("*", stack_);
                else if (current == '^') poliz_input("^", stack_);
            }
            else if (current == ')'){
                state = State_F;
                if (brack.top() == 0){
                    brack.pop();
                    poliz_input(")", stack_);
                }
                else {
                    error(i, 1);
                }
            }
            else if (current == ','){
                state = State_A;
                poliz_input(number, stack_);
                number.clear();
                poliz_input(",", stack_);
                if (brack.top() > 0){
                    brack.top()--;
                }
                else {
                    error(i, 2);
                }
            }
            else {
                error(i, 0);
            }

            break;
        case State_F:
            if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                state = State_B;
                if (current == '+') poliz_input("+", stack_);
                else if (current == '-') poliz_input("-", stack_);
                else if (current == '/') poliz_input("/", stack_);
                else if (current == '*') poliz_input("*", stack_);
                else if (current == '^') poliz_input("^", stack_);
            }
            else if (current == ')'){
                if (brack.top() == 0){
                    brack.pop();
                    poliz_input(")", stack_);
                }
                else {
                    error(i, 1);
                }
            }
            else if (current == ','){
                state = State_A;
                poliz_input(",", stack_);
                if (brack.top() > 0){
                    brack.top()--;
                }
                else {
                    error(i, 2);
                }
            }
            else {
                error(i, 0);
            }

            break;
        case State_G:
            if (current == '('){
                state = State_A;
                brack.push(0);
                poliz_input("(", stack_);
            }
            else if (current >= '0' && current <= '9'){
                state = State_C;
                number.clear();
                number += current;
            }
            else if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z')){
                state = State_H;
                number.clear();
                number += current;
            }
            else {
                error(i, 0);
            }

            break;
        case State_H:
            if ((current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') || (current >= '0' && current <= '9')){
                number += current;
            }
            else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^'){
                state = State_B;
                number.insert(number.begin(), '&');
                poliz_input(number, stack_);
                number.clear();
                if (current == '+') poliz_input("+", stack_);
                else if (current == '-') poliz_input("-", stack_);
                else if (current == '/') poliz_input("/", stack_);
                else if (current == '*') poliz_input("*", stack_);
                else if (current == '^') poliz_input("^", stack_);
            }
            else if (current == ','){
                state = State_A;
                number.insert(number.begin(), '&');
                poliz_input(number, stack_);
                number.clear();
                poliz_input(",", stack_);
                if (brack.top() > 0){
                    brack.top()--;
                }
                else {
                    error(i, 2);
                }
            }
            else if (current == '('){
                state = State_A;
                if (number == "sin" || number == "cos" || number == "ln" || number == "exp" || number == "sqrt"){
                    poliz_input(number, stack_);
                    number.clear();
                    poliz_input("(", stack_);
                    brack.push(0);
                }
                else if (number == "log"){
                    poliz_input(number, stack_);
                    number.clear();
                    poliz_input("(", stack_);
                    brack.push(1);
                }
                else {
                    error(i, 3);
                }
            }
            else if (current == ')'){
                state = State_F;
                number.insert(number.begin(), '&');
                poliz_input(number, stack_);
                number.clear();
                if (brack.top() == 0){
                    brack.pop();
                    poliz_input(")", stack_);
                }
                else {
                    error(i, 1);
                }
            }
            else {
                error(i, 0);
            }

            break;
        }
        //cout << current << " , " << state << " | " << poliz << '\n';
    }
    //poliz_input(")", stack_);
}



int main()
{
    string inp;
    inp = "-x";
    avtomat(inp);
    cout << poliz;

    return 0;
}
