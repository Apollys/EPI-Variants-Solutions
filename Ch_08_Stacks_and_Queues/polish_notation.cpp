#include <cctype>

#include <iostream>
#include <functional>
#include <vector>

bool IsNumeric(const std::string& s) {
    for (const char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

// Note: this function assumes the input is a well-formed polish notation expression
int EvaluatePolishNotation(const std::vector<std::string>& input_expression) {
    // Create map for op functions for convenience
    static const std::unordered_map<std::string, std::function<int(int, int)>> kOpFunctions{
            {"+", std::plus<int>()},
            {"-", std::minus<int>()},
            {"*", std::multiplies<int>()},
            {"/", std::divides<int>()}};
    // Initialize stack of strings, so it can hold both numbers and expressions
    std::vector<std::string> stack;
    for (const std::string& current_element : input_expression) {
        stack.push_back(current_element);
        // Check if we should evaluate (top two values of stack are numbers)
        while ((stack.size() >= 3) && IsNumeric(stack.back()) && IsNumeric(stack[stack.size() - 2])) {
            int right_operand = std::stoi(stack.back());
            stack.pop_back();  // pop right operand
            int left_operand = std::stoi(stack.back());
            stack.pop_back();  // pop left operand
            const auto& op_func = kOpFunctions.at(stack.back());
            stack.pop_back();  // pop operator
            stack.push_back(std::to_string(op_func(left_operand, right_operand)));
        }
    }
    std::cout << "Stack size at end (should be 1): " << stack.size() << std::endl;
    return std::stoi(stack.back());
}

int main() {
    const std::vector<std::string> test_input{"+", "*", "-", "3", "1", "5", "+", "44", "66"};
    //                                                      (3 - 1) * 5        (44 + 66)
    //                                                         10         +       110
    //                                                              --> 120
    
    const int result = EvaluatePolishNotation(test_input);
    std::cout << "Result = " << result << std::endl;
    
    return 0;
}
