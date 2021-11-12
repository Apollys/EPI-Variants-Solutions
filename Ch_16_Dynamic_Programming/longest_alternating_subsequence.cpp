#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i + 1 < v.size()) os << ", ";
    }
    os << "]";
    return os;
}

/* Compute the longest subsequence of the given input vector,
 * in which the subsequence's consecutive elements must alternate between
 * strictly increasing and strictly decreasing.  The first pair of elements
 * must be strictly increasing (by the problem definition in EPI).
 *
 * Algorithm: We walk through the array, examining consecutive pairs of elements.
 * If the second element is greater than the first, we can utilize those two
 * elements as the beginning of our sequence.  If the second element is less than
 * the first, that second element is now the less restrictive element to find
 * a subsequent greater element, so we will keep track of it as the "top of our stack".
 * Now we look at the third element and compare to the top of our stack.
 * If it is greater, we have the following situation: a < b < c, which we collapse to
 * a < c, storing a and c on our stack (since c is less restrictive than b to find
 * a lesser element).  If it is lesser, we have a < b > c, which is perfect, and we keep
 * all three on our stack.  Now we continue, flipping the logic based on the result of the
 * last comparison performed.
 */
template <typename T>
std::vector<T> ComputeLongestAlternatingSubsequence(const std::vector<T>& input_vector) {
    if (input_vector.size() == 0) {
        return {};
    }
    std::vector<T> result_vector{input_vector[0]};
    bool looking_for_greater = true;
    for (std::size_t i = 1; i < input_vector.size(); ++i) {
        const T& current_value = input_vector[i];
        const auto compare_func = [looking_for_greater](const T& left, const T& right) {
            return looking_for_greater ? left < right : left > right;
        };
        // If we found the ordering we're looking for, add new value to result vector
        // Also, toggle the looking_for_greater flag!
        if (compare_func(result_vector.back(), current_value)) {
            result_vector.push_back(current_value);
            looking_for_greater = !looking_for_greater;
        }
        // Otherwise, we found consecutive increase/decrease (or equal value), so we replace
        // last value of subsequence with newly found (less restrictive) value
        else {
            result_vector.back() = current_value;
        }
    }
    return result_vector;
}

int main() {
    std::vector<int> test_vector{4, 2, 7, 8, 8, 9, 9, 3, 2, 3, 5, 4, 1, 1, 1, 9, 2, 1, 4, 1, 7, 8};
    
    std::vector<int> result_vector = ComputeLongestAlternatingSubsequence(test_vector);
    
    std::cout << result_vector << std::endl;
    
    return 0;
}
