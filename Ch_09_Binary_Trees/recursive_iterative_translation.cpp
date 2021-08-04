#include <iostream>
#include <vector>

int foo_recursive(int value) {
    std::cout << "Start" << std::endl;
    if (value < 10) {
        return value;
    }
    std::cout << "A" << std::endl;
    const int a = foo_recursive(value / 2);
    std::cout << "B" << std::endl;
    const int b = foo_recursive(value - 3);
    std::cout << "C" << std::endl;
    return a + b;
}

int foo_iterative(int input_value) {
    enum class NextLine { Start, A, B, C };
    std::vector<std::pair<int, NextLine>> call_stack;
    // Initialize call stack with our starting value and state
    call_stack.emplace_back(input_value, NextLine::Start);
    // We'll also want a return value stack to hold the returns from our calls
    std::vector<int> return_value_stack;
    // Iterate as long as there are things to do
    while (!call_stack.empty()) {
        const auto& [value, next_line] = call_stack.back();
        call_stack.pop_back();
        if (next_line == NextLine::Start) {
            std::cout << "Start" << std::endl;
            // Base case: just a return (translates to a push to return value stack)
            if (value < 10) {
                return_value_stack.push_back(value);
            }
            // If we didn't return though, we need to move on to line A with the same value,
            // which translates to a push (emplace because std::pair) to the call stack
            else {
                call_stack.emplace_back(value, NextLine::A);
            }
        } else if (next_line == NextLine::A) {
            std::cout << "A" << std::endl;
            // Push items onto the stack in the reverse order we want them to occur,
            // so the thing we want to occur immediately ends up on top of the stack.
            // Therefore we push the continuation of this function (starting from B)
            // first, then the recursive call for value / 2 second.
            call_stack.emplace_back(value, NextLine::B);
            call_stack.emplace_back(value / 2, NextLine::Start);
        } else if (next_line == NextLine::B) {
            std::cout << "B" << std::endl;
            // Same as above, make sure recursive call ends up on top of stack
            call_stack.emplace_back(value, NextLine::C);
            call_stack.emplace_back(value - 3, NextLine::Start);
        } else if (next_line == NextLine::C) {
            std::cout << "C" << std::endl;
            // Retrieve a and b from the return value stack
            // (in reverse order since b will have been pushed more recently)
            const int b = return_value_stack.back();
            return_value_stack.pop_back();
            const int a = return_value_stack.back();
            return_value_stack.pop_back();
            // To return a + b, simply push it to the top of the return value stack
            return_value_stack.push_back(a + b);
        }
    }
    // Now the call stack is empty and the return value
    // stack has a single item - our final answer!
    std::cout << "Final return value stack size: "
              << return_value_stack.size() << std::endl;
    return return_value_stack.back();
}

int main() {
    std::cout << "=================================" << std::endl;
    const int result_recursive = foo_recursive(20);
    std::cout << "=================================" << std::endl;
    const int result_iterative = foo_iterative(20);
    std::cout << "=================================" << std::endl;
    
    std::cout << "Recursive result: " << result_recursive << std::endl;
    std::cout << "Iterative result: " << result_iterative << std::endl;

    return 0;
}
