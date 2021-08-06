/* Problem: Given a BST, generate all possible sequences of values
 * such that when values are inserted into an empty BST in the 
 * sequence computed, the given BST is constructed.
 */

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

template <typename T>
void GenerateInterleavedSequencesRecursive(
        const std::vector<T>& input_sequence_a,
        const std::vector<T>& input_sequence_b,
        std::vector<T>& current_sequence,
        size_t i,
        size_t j,
        std::vector<std::vector<T>>& generated_sequences);

template <typename T>
std::vector<std::vector<T>> GenerateInterleavedSequences(
        const std::vector<T>& sequence, const std::vector<T>& other_sequence) {
    std::vector<T> current_sequence;
    size_t i = 0;
    size_t j = 0;
    std::vector<std::vector<T>> generated_sequences;
    GenerateInterleavedSequencesRecursive(
            sequence, other_sequence, current_sequence, i, j, generated_sequences);
    return generated_sequences;
}

template <typename T>
void GenerateInterleavedSequencesRecursive(
        const std::vector<T>& input_sequence_a,
        const std::vector<T>& input_sequence_b,
        std::vector<T>& current_sequence,
        size_t i,
        size_t j,
        std::vector<std::vector<T>>& generated_sequences) {
    if (i < input_sequence_a.size()) {
        current_sequence.emplace_back(input_sequence_a[i]);
        GenerateInterleavedSequencesRecursive(
                input_sequence_a, input_sequence_b, current_sequence,
                i + 1, j, generated_sequences);
        current_sequence.pop_back();
    }
    if (j < input_sequence_b.size()) {
        current_sequence.emplace_back(input_sequence_b[j]);
        GenerateInterleavedSequencesRecursive(
                input_sequence_a, input_sequence_b, current_sequence,
                i, j + 1, generated_sequences);
        current_sequence.pop_back();
    }
    if ((i == input_sequence_a.size()) && (j == input_sequence_b.size())) {
        generated_sequences.emplace_back(current_sequence);
    }
}

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

template <typename T>
struct Node {
    std::unique_ptr<Node<T>> left_child_ptr;
    std::unique_ptr<Node<T>> right_child_ptr;
    Node<T>* parent_ptr_raw;
    T value;
};

template <typename T>
class BinaryTree {
  public:
    BinaryTree() {}
    
    // Simple insert: inserts value at next available node to make complete tree
    void insert(const T& value) {
        std::vector<Node<T>*> path_to_inserted_node = FindNthNodePath(_size + 1);
        path_to_inserted_node.back()->value = value;
    }
    
    // Return by value, because if it's empty we still need to return something
    // Also it doesn't make sense to return a reference because we don't want
    // users modifying values of the heap (which would ruin the heap structure)
    T top() {
        if (_root == nullptr) {
            return T{};
        }
        return _root->value;
    }
    
    // NYI
    T pop() {
        if (_root == nullptr) {
            return T{};
        }
        return _root->value;
    }
    
    bool is_empty() {
        return _root == nullptr;
    }
    
    size_t size() {
        return _size;
    }
    
    // Assuming the current binary tree satisfies the rules of a BST,
    // generate a list of all possible node sequences, such that inserting
    // nodes in the sequence into an empty BST would yield this BST.
    // Approach: dispatch to recursive helper function
    std::vector<std::vector<T>> BstSequences() {
        return BstSequencesRecursive(_root.get());
    }
    
    // Approach: compute left and right subtree sequences,
    // then interleave the results in all possible ways
    std::vector<std::vector<T>> BstSequencesRecursive(const Node<T>* root_ptr) {
        const bool has_left_child = root_ptr->left_child_ptr != nullptr;
        const bool has_right_child = root_ptr->right_child_ptr != nullptr;
        if (!has_left_child && !has_right_child) {
            return {std::vector<T>{root_ptr->value}};
        } else if (has_left_child && !has_right_child) {
            std::vector<std::vector<T>> left_subtree_sequences =
                    BstSequencesRecursive(root_ptr->left_child_ptr.get());
            for (std::vector<T>& left_seq : left_subtree_sequences) {
                left_seq.insert(left_seq.begin(), root_ptr->value);
            }
            return left_subtree_sequences;
        } else if (!has_left_child && has_right_child) {
            std::vector<std::vector<T>> right_subtree_sequences =
                    BstSequencesRecursive(root_ptr->right_child_ptr.get());
            for (std::vector<T>& right_seq : right_subtree_sequences) {
                right_seq.insert(right_seq.begin(), root_ptr->value);
            }
            return right_subtree_sequences;
        }
        // Otherwise: (has_left_child && has_right_child)
        std::vector<std::vector<T>> left_subtree_sequences =
                BstSequencesRecursive(root_ptr->left_child_ptr.get());
        std::vector<std::vector<T>> right_subtree_sequences =
                BstSequencesRecursive(root_ptr->right_child_ptr.get());
        std::vector<std::vector<T>> all_interleaved_sequences;
        for (const std::vector<T>& left_seq : left_subtree_sequences) {
            for (const std::vector<T>& right_seq : right_subtree_sequences) {
                std::vector<std::vector<T>> new_sequences =
                        GenerateInterleavedSequences(left_seq, right_seq);
                all_interleaved_sequences.insert(
                        all_interleaved_sequences.end(),
                        std::make_move_iterator(new_sequences.begin()),
                        std::make_move_iterator(new_sequences.end()));
            }
        }
        for (std::vector<T>& interleaved_seq : all_interleaved_sequences) {
            interleaved_seq.insert(interleaved_seq.begin(), root_ptr->value);
        }
        return all_interleaved_sequences;
               
    }

    // Print out the tree level-by-level
    friend std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree) {
        std::vector<Node<T>*> current_level_node_ptrs{tree._root.get()};
        std::vector<Node<T>*> next_level_node_ptrs;
        while (current_level_node_ptrs.size() > 0) {
            for (Node<T>* current_node_ptr : current_level_node_ptrs) {
                os << current_node_ptr->value << " ";
                if (current_node_ptr->left_child_ptr != nullptr) {
                    next_level_node_ptrs.push_back(current_node_ptr->left_child_ptr.get());
                }
                if (current_node_ptr->right_child_ptr != nullptr) {
                    next_level_node_ptrs.push_back(current_node_ptr->right_child_ptr.get());
                }
            }
            os << std::endl;
            current_level_node_ptrs = std::move(next_level_node_ptrs);
            next_level_node_ptrs.clear();
        }
        return os;
    }
        
  private:
    // Retrieve the nth node in the tree assuming the tree is complete,
    // inserting a new node if it does not exist (and any nodes along the path).
    // Return a vector of the nodes along the path from route to the nth node.
    // Note: n is 1-indexed here, as it makes the math easier!
    std::vector<Node<T>*> FindNthNodePath(size_t n) {
        size_t modulus = 1;
        while (n / modulus > 1) {
            modulus *= 2;
        }
        // std::cout << "Starting modulus: " << modulus << std::endl;
        std::vector<Node<T>*> path_vector;
        Node<T>* current_node_ptr = _root.get();
        if (_root == nullptr) {
            _root = std::make_unique<Node<T>>();
            ++_size;
            current_node_ptr = _root.get();
        }
        path_vector.push_back(current_node_ptr);
        for (/* modulus */; modulus > 1; modulus /= 2) {
            if ((n % modulus) < (modulus / 2)) {  // left
                // std::cout << "left" << std::endl;
                if (current_node_ptr->left_child_ptr == nullptr) {
                    current_node_ptr->left_child_ptr = std::make_unique<Node<T>>();
                    ++_size;
                }
                current_node_ptr = current_node_ptr->left_child_ptr.get();
            } else {  // right
                // std::cout << "right" << std::endl;
                if (current_node_ptr->right_child_ptr == nullptr) {
                    current_node_ptr->right_child_ptr = std::make_unique<Node<T>>();
                    ++_size;
                }
                current_node_ptr = current_node_ptr->right_child_ptr.get();
            }
            // Add whichever node we took, left or right, to the path vector
            path_vector.push_back(current_node_ptr);
        }
        return path_vector;
    }            
  
    std::unique_ptr<Node<T>> _root;
    size_t _size = 0;
};

int main() {
    std::vector<int> test_values{
            3,
            1, 5,
            0, 2};
    // std::vector<int> test_values{1, 1, 1};
    
    BinaryTree<int> test_tree;
    for (const int value : test_values) {
        test_tree.insert(value);
    }
    std::cout << test_tree << std::endl << std::endl;
    
    std::vector<std::vector<int>> bst_sequences = test_tree.BstSequences();
    std::cout << "All bst sequences:" << std::endl;
    for (const std::vector<int>& seq : bst_sequences) {
        std::cout << "    " << seq << std::endl;
    }
    
    /*
    std::vector<int> seq_a{1, 11, 55};
    std::vector<int> seq_b{2, 8, 100};
    std::vector<std::vector<int>> interleavings =
            GenerateInterleavedSequences(seq_a, seq_b);
    std::cout << "All interleaved sequences:" << std::endl;
    for (const std::vector<int>& seq : interleavings) {
        std::cout << "    " << seq << std::endl;
    }
    std::cout << std::endl;
    */
    
    return 0;
}
    
