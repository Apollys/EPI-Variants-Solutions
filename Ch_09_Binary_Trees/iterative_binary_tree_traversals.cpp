#include <iostream>
#include <memory>
#include <utility>
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
    
    std::vector<T> PreorderTraversal() {
        if (_root == nullptr) {
            return {};
        }
        std::vector<T> traversal_vector;
        std::vector<Node<T>*> frontier{_root.get()};
        while (frontier.size() > 0) {
            Node<T>* current_node_ptr = frontier.back();
            frontier.pop_back();
            traversal_vector.emplace_back(current_node_ptr->value);
            if (current_node_ptr->right_child_ptr != nullptr) {
                frontier.push_back(current_node_ptr->right_child_ptr.get());
            }
            if (current_node_ptr->left_child_ptr != nullptr) {
                frontier.push_back(current_node_ptr->left_child_ptr.get());
            }
        }
        return traversal_vector;
    }
    
    void PostorderTraversalR(Node<T>* current_node_ptr, std::vector<T>& traversal) {
        if (current_node_ptr->left_child_ptr != nullptr) {
            PostorderTraversalR(current_node_ptr->left_child_ptr.get(), traversal);
        }
        if (current_node_ptr->right_child_ptr != nullptr) {
            PostorderTraversalR(current_node_ptr->right_child_ptr.get(), traversal);
        }
        traversal.push_back(current_node_ptr->value);
    }
    
    std::vector<T> PostorderTraversal() {
        if (_root == nullptr) {
            return {};
        }
        enum class NextAction { kTraverse, kVisit };
        using NodeActionPair = std::pair<Node<T>*, NextAction>;
        std::vector<T> traversal_vector;
        std::vector<NodeActionPair> node_action_stack{
                std::make_pair(_root.get(), NextAction::kTraverse)};
        while (node_action_stack.size() > 0) {
            auto [current_node_ptr, next_action] = node_action_stack.back();
            node_action_stack.pop_back();
            if (next_action == NextAction::kVisit) {
                traversal_vector.emplace_back(current_node_ptr->value);
            } else {  // next_action == NextAction::kTraverse
                node_action_stack.emplace_back(current_node_ptr, NextAction::kVisit);
                if (current_node_ptr->right_child_ptr != nullptr) {
                    node_action_stack.emplace_back(
                            current_node_ptr->right_child_ptr.get(),
                            NextAction::kTraverse);
                }
                if (current_node_ptr->left_child_ptr != nullptr) {
                    node_action_stack.emplace_back(
                            current_node_ptr->left_child_ptr.get(),
                            NextAction::kTraverse);
                }
            }
        }
        return traversal_vector;
    }
    
    std::vector<T> InorderTraversal() {
        if (_root == nullptr) {
            return {};
        }
        enum class Action { kEnter, kVisit };
        using NodeActionPair = std::pair<Node<T>*, Action>;
        std::vector<T> traversal_vector;
        std::vector<NodeActionPair> node_action_stack{
                std::make_pair(_root.get(), Action::kEnter)};
        while (node_action_stack.size() > 0) {
            auto [current_node_ptr, action] = node_action_stack.back();
            node_action_stack.pop_back();
            if (action == Action::kVisit) {
                traversal_vector.emplace_back(current_node_ptr->value);
            } else {  // action == Action::kEnter
                if (current_node_ptr->right_child_ptr != nullptr) {
                    node_action_stack.emplace_back(
                            current_node_ptr->right_child_ptr.get(),
                            Action::kEnter);
                }
                node_action_stack.emplace_back(current_node_ptr, Action::kVisit);
                if (current_node_ptr->left_child_ptr != nullptr) {
                    node_action_stack.emplace_back(
                            current_node_ptr->left_child_ptr.get(),
                            Action::kEnter);
                }
            }
        }
        return traversal_vector;
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
            1,
            9, 3,
            12, 50, 100, 5,
            13, 16, 51, 200, 101, 102, 6, 999};
    
    BinaryTree<int> test_tree;
    for (const int value : test_values) {
        test_tree.insert(value);
    }
    std::cout << test_tree << std::endl << std::endl;
    
    std::cout << "Computing preorder traversal:" << std::endl;
    std::vector<int> preorder_traversal = test_tree.PreorderTraversal();
    std::cout << preorder_traversal << std::endl << std::endl;
    
    std::cout << "Computing postorder traversal:" << std::endl;
    std::vector<int> postorder_traversal = test_tree.PostorderTraversal();
    std::cout << postorder_traversal << std::endl << std::endl;
    
    std::cout << "Computing inorder traversal:" << std::endl;
    std::vector<int> inorder_traversal = test_tree.InorderTraversal();
    std::cout << inorder_traversal << std::endl << std::endl;
    
    return 0;
}
    
