#include <iostream>
#include <memory>
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
    Node(const T& value) : value(value) {}
    
    T value;
    std::unique_ptr<Node<T>> left_child;
    std::unique_ptr<Node<T>> right_child;
    Node<T>* parent = nullptr;
};

// Idea from here: https://stackoverflow.com/a/51730733
template <typename PointerT>
void PrintBTRecursive(const std::string& prefix, const PointerT& node, bool is_left_child) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (is_left_child ? "├──L:" : "└──R:" );
        std::cout << node->value << std::endl;
        // Recurse on left and right subtrees
        const std::string subtree_prefix = prefix + (is_left_child ? "│     " : "      ");
        PrintBTRecursive(subtree_prefix, node->left_child.get(), true);
        PrintBTRecursive(subtree_prefix, node->right_child.get(), false);
    }
}

template <typename PointerT>
void PrintBT(const PointerT& root) {
    PrintBTRecursive("", root, false);
}

template <typename T>
std::unique_ptr<Node<T>> ConstructMaxTree(const std::vector<T>& input_vector) {
    if (input_vector.size() == 0) {
        return nullptr;
    }
    std::unique_ptr<Node<T>> root = std::make_unique<Node<T>>(input_vector[0]);
    Node<T>* current_node = root.get();
    for (size_t i = 1; i < input_vector.size(); ++i) {
        const T& value_to_insert = input_vector[i];
        // Walk up the tree until new value is not greater than parent value
        while ((current_node != nullptr) && (value_to_insert > current_node->value)) {
            current_node = current_node->parent;
        }
        // Now we need to make the new value current_node's right child
        // and move current_node's current right child to new node's left child
        // If current node is nullptr, then the new node will be the new root
        // and the current root will be this new node's left child
        if (current_node == nullptr) {
            std::unique_ptr<Node<T>> new_root = std::make_unique<Node<T>>(value_to_insert);
            new_root->left_child = std::move(root);
            new_root->left_child->parent = new_root.get();
            root = std::move(new_root);
            current_node = root.get();
        } else {
            // Insert and rotate right child, if exists
            if (current_node->right_child != nullptr) {
                std::unique_ptr<Node<T>> new_node = std::make_unique<Node<T>>(value_to_insert);
                new_node->left_child = std::move(current_node->right_child);
                new_node->left_child->parent = new_node.get();
                current_node->right_child = std::move(new_node);
            } else {  // no right child of current_node, so just insert as right child
                current_node->right_child = std::make_unique<Node<T>>(value_to_insert);
            }
            current_node->right_child->parent = current_node;
            // Update current_node as node we just inserted
            current_node = current_node->right_child.get();
        }
    }
    return root;
}

int main() {
    {
        std::vector<int> test_vector{6, 4, 2, 1};
        std::unique_ptr<Node<int>> max_tree_root = ConstructMaxTree(test_vector);
        PrintBT(max_tree_root);
    }
    
    {
        std::vector<int> test_vector{6, 2, 4, 1};
        std::unique_ptr<Node<int>> max_tree_root = ConstructMaxTree(test_vector);
        PrintBT(max_tree_root);
    }
    
    {
        std::vector<int> test_vector{6, 2, 1, 4, 3, 7, 2, 5, 1, 6, 8};
        std::unique_ptr<Node<int>> max_tree_root = ConstructMaxTree(test_vector);
        PrintBT(max_tree_root);
    }
    
    return 0;
}

