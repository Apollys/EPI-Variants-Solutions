#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <unordered_map>
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

struct IndexLengthPair {
    size_t index;
    size_t length;
};

template <typename T>
std::vector<T> ComputeLongestNondecreasingSubsequence(const std::vector<T>& input_vector) {
    // Keep BST of active sequences, mapping end value to index in input_vector and sequence length
    // We always maintain the property: shorter sequences have smaller end values
    std::map<T, IndexLengthPair> active_sequence_bst;
    using bst_const_iterator = typename std::map<T, IndexLengthPair>::const_iterator;
    // Keep a hash map to look up active sequences in the BST by length
    std::unordered_map<size_t, bst_const_iterator> length_to_bstnode_map;
    // Keep track of the max length as we go to save an extra iteration
    size_t max_length = 0;
    // Keep track of the previous element in each sequence, by index
    // Mapping to -1 will indicate there is no previous element
    std::unordered_map<size_t, size_t> index_to_previous_index_map;
    for (size_t i = 0; i < input_vector.size(); ++i) {
        const T& current_value = input_vector[i];
        std::cout << "current_value = " << current_value << std::endl;
        // Search for longest active sequence (which means sequence with largest end
        // value, by our invariant property above) that this element can be appended to
        bst_const_iterator upper_bound_it = active_sequence_bst.upper_bound(current_value);
        const bool found_upper_bound = !(upper_bound_it == active_sequence_bst.begin());
        // upper_bound returns first element greater, we want largest element less or equal
        bst_const_iterator real_upper_bound_it =
                found_upper_bound ? std::prev(upper_bound_it) : active_sequence_bst.end();
        // If none found, new sequence just has this element, else is appended to found sequence
        const size_t new_sequence_length =
                found_upper_bound ? real_upper_bound_it->second.length + 1 : 1;
        const size_t new_sequence_previous_index = 
                found_upper_bound ? real_upper_bound_it->second.index : -1;
        std::cout << "    found_upper_bound = " << found_upper_bound << std::endl;
        std::cout << "    new_sequence_length = " << new_sequence_length << std::endl;
        // Check if there is another sequence of the same length that needs to be deleted
        // Note that the new sequence is always better, because if new end element
        // were greater than old end element, it would have been appended to it instead
        if (length_to_bstnode_map.contains(new_sequence_length)) {
            std::cout << "Erasing old sequence of same length" << std::endl;
            active_sequence_bst.erase(length_to_bstnode_map[new_sequence_length]);
            length_to_bstnode_map.erase(new_sequence_length);
        }
        // In the case of repeated elements, if new end value equals found sequence end
        // value, we want to replace that sequence, not just add a new one
        if (found_upper_bound && (real_upper_bound_it->first == current_value)) {
            std::cout << "    deleting found sequence because same end value" << std::endl;
            length_to_bstnode_map.erase(real_upper_bound_it->second.length);
            active_sequence_bst.erase(real_upper_bound_it);
        }
        std::cout << "    inserting sequence {" << new_sequence_length << ", " << current_value 
                  << "}" << std::endl;
        // Insert in bst
        bst_const_iterator new_sequence_bstnode = active_sequence_bst.insert(
                std::make_pair(current_value, IndexLengthPair{i, new_sequence_length})).first;
        // Save iterator for length to bstnode map
        length_to_bstnode_map[new_sequence_length] = new_sequence_bstnode;
        // Save previous index in index_to_previous_index_map
        index_to_previous_index_map[i] = new_sequence_previous_index;
        // Update max_length
        max_length = std::max(max_length, new_sequence_length);
    }
    // Now max_length contains the length of the longest non-decreasing subsequence.
    std::cout << "Max length: " << max_length << std::endl;
    // If we just wanted the max length, we could return it here (and wouldn't need the index
    // tracking in our bst).
    // To recover the actual sequence, we use the index_to_prev_index map.
    std::vector<T> longest_subsequence;
    bst_const_iterator bst_node_end = length_to_bstnode_map[max_length];
    size_t current_index = bst_node_end->second.index;
    // Print BST
    std::cout << "BST contains: " << std::endl;
    for (const auto& [end_value, index_length_pair] : active_sequence_bst) {
        std::cout << "end_value: " << end_value << ", index: " << index_length_pair.index
                  << ", length: " << index_length_pair.length << std::endl;
    }
    std::cout << "Index to previous index hash map contains:" << std::endl;
    for (const auto& [index, previous_index] : index_to_previous_index_map) {
        std::cout << index << " -> " << previous_index << ", ";
    }
    std::cout << std::endl;
    std::cout << "Retracing subsequence backwards" << std::endl;
    while (current_index != -1) {
        longest_subsequence.push_back(input_vector[current_index]);
        current_index = index_to_previous_index_map[current_index];
    }
    // Now we have the reversed longest subsequence, just need to reverse before returning
    std::reverse(longest_subsequence.begin(), longest_subsequence.end());
    return longest_subsequence;
}

int main() {
    std::cout << std::boolalpha;

    //const std::vector<int> test_vector{0, 8, 4, 12, 2, 10, 6, 1, 9, 5};
    const std::vector<int> test_vector{1, 2, 3, 2, 2, 1, 3, 2, 3, 1, 2, 3, 3};
    //const std::vector<int> test_vector{1, 4, 2, 1};
    
    std::vector<int> longest_subsequence = ComputeLongestNondecreasingSubsequence(test_vector);
    std::cout << "Longest nondec subsequence: " << longest_subsequence << std::endl;
    
    return 0;
}

