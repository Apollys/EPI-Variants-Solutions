## Chapter 8 - Stacks and Queues - Variants Solutions

---

**Stack with max - improved space efficiency**

Their description to the solution for the previous problem is straightforward, but their implementation seems intentionally weird just to allow for this problem.

What you want is your normal stack, plus a stack of maxes (which will only be pushed to when the new value is greater than the max stack's top value).  Now, when we pop values off the stack, how do we know whether or not to pop the max?  For example, given the list `[1, 5, 2, 5]`, suppose we push all the values on, so we have a max stack of `[1, 5]`, and now we pop elements one by one.  We need to know not to pop the max until we pop the second `5`.

This suggests keeping a count variable in our max stack, and then the pop operation will look something like this:

```c++
// stack and max_stack declarations
std::vector<int> _stack;
std::vector<std::pair<int, size_t>> _max_stack;

void pop() {
    const int& value_to_pop = _stack.back();
    auto& [max_value, max_count] = _max_stack.back();
    if (value_to_pop == max_value) {
        --max_count;
    }
    if (max_count == 0) {
        _max_stack.pop_back();
    }
    _stack.pop_back();
}
```
    
Of course, whenever we push, we will want to increment the count if the value we are pushing equals the top value of the max stack.

---

**Polish notation**

This is going to be similar to the RPN example given before, except that you will encounter operators first, so your stack will need a way to store both operators and values.  The most natural way is to use strings.

<details>
<summary>Full code</summary>

```c++
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
```
</details>

[polish_notation.cpp](https://github.com/Apollys/EPI-Variants-Solutions/blob/main/Ch_08_Stacks_and_Queues/polish_notation.cpp)
    
---

**Given a sequence of buildings' heights which you must process in west to east order, return a list of all buildings that have a sunset view.  A building has a sunset view iff the height of all buildings to the west of it are less than its own height.**
    
West to east is actually the order we want for this problem.  No data structure required, just track the running maximum, and if the current building's height is greater than the running maximum it has a sunset view.
    
---
    
**Return the keys of a binary tree in top-down, alternating left-right/right-left order.**

I will first say there's not really any reason to be using queues over normal vectors in this problem.  They're just trying to force some queue practice.
    
With that in mind, the easiest way to do this is just to keep the current level and next level in vectors, and whenever you go to push the current level nodes onto the result vector, based on the parity of current_depth (which you track as a single integer), you decide whether or not to reverse the current level nodes.
    
---
    
**Return the keys of a binary tree in a bottom-up, left-right order.**
    
I would just generate a vector of keys in the reversed order (top-down, right-left) and the reverse it.
    
---
    
**Compute the average of the values at each level of a binary tree.**
    
We already know how to generate a list of all the nodes at each level of a binary tree, so this is a trivial extension of that.
    
---
