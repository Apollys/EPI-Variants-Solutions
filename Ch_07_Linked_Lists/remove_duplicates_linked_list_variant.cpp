#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
struct Node {
    std::unique_ptr<Node<T>> next;
    T data;
    
    Node(const T& data) : data(data) {}
};

template <typename T>
class SinglyLinkedList {
  public:
    SinglyLinkedList() {}
    
    void Append(const T& value) {
        _tail->next = std::make_unique<Node<T>>(value);
        _tail = _tail->next.get();
    }
    
    // Helper function to find last occurence of the element in a block,
    // assuming list is partitioned into groups of equal-valued elements
    // Requirement: before_start_ptr's next field is not a nullptr
    // (Placed here for convenience of solution readability, normally should be private)
    Node<T>* FindLastOccurrenceOf(Node<T>* before_start_ptr, std::size_t* el_count) {
        const T target_value = before_start_ptr->next->data;
        Node<T>* curr_ptr = before_start_ptr->next.get();
        *el_count = 1;
        while ((curr_ptr->next != nullptr) && (curr_ptr->next->data == target_value)) {
            curr_ptr = curr_ptr->next.get();
            ++*el_count;
        }
        return curr_ptr;
    }
    
    // Remove all instances of elements occurring more than M times
    // Assumption: list is *sorted* (or at least partitioned into groups of equal elements)
    void RemoveElementsMoreThanM(std::size_t m) {
        Node<T>* before_start_ptr = _head.get();
        Node<T>* next_before_start_ptr = nullptr;
        while (before_start_ptr->next != nullptr) {
            std::size_t el_count = 0;
            // Find end of repeated element block, and get count of repeated elements
            Node<T>* end_ptr = FindLastOccurrenceOf(before_start_ptr, &el_count);
            if (el_count > m) {
                // Update tail if needed (can omit this if not tracking tail pointer)
                if (end_ptr == _tail) {
                    _tail = before_start_ptr;
                }
                before_start_ptr->next = std::move(end_ptr->next);
            } else {  // only update this pointer if we didn't delete the current block
                before_start_ptr = end_ptr;
            }
            // Uncomment to see blocks being processed step-by-step
            // std::cout << to_string() << std::endl;
        }
    }
    
    std::string to_string() {
        std::ostringstream oss;
        oss << "[";
        Node<T>* curr_ptr = _head->next.get();
        while (curr_ptr->next != nullptr) {
            oss << curr_ptr->data << ", ";
            curr_ptr = curr_ptr->next.get();
        }
        oss << curr_ptr->data << "]";
        return oss.str();
    }
    
  private:
    std::unique_ptr<Node<T>> _head = std::make_unique<Node<T>>(T{});
    Node<T>* _tail = _head.get();  // track tail for O(1) append
};

// Set m = 2, corresponds to removing all blocks of 3 or more elements
const int kM = 2;

const std::vector<std::vector<int>> kTestCaseVectors{
        {1, 2, 2, 3, 3, 3},  // simple case
        {1, 2, 2, 3, 3, 3, 4, 4, 4, 4},  // adjacent removals
        {1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5},  // above, but end with non-removed value
        {1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 6, 7, 7, 7, 8, 8, 8, 9, 10, 10, 10},  // multiple groups
};

int main() {
    for (const std::vector<int>& test_vector : kTestCaseVectors) {
        SinglyLinkedList<int> test_ll;
        for (const int& value : test_vector) {
            test_ll.Append(value);
        }
        std::cout << "Before removal: " << test_ll.to_string() << std::endl;
        test_ll.RemoveElementsMoreThanM(kM);
        std::cout << "After removal:  " << test_ll.to_string() << std::endl << std::endl;
    }
    
    return 0;
}
