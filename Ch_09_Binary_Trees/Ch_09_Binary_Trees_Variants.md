## Chapter 9 - Binary Trees - Variants Solutions

---

**Find the size of the largest complete subtree of a binary tree.**

This requires a little bit of case analysis.  What conditions on the left and right subtrees make a binary tree complete?

If the left and right subtrees were both perfect trees of the same height, then the binary tree would certainly be complete (and perfect).

But if they were perfect and the left subtree had height 1 greater than the right subtree, that would also be complete.

More generally, one of the subtrees will have to be perfect, the other complete, and based on which is which you will have either a requirement of same heights or left subtree having height 1 greater than right subtree. The perfect left, perfect right case will probably still have to be handled specifically.

So I would probably write a recursive function that returns whether or not the subtree rooted at a given node is complete, and how many nodes that subtree contains in total.  If the binary tree nodes don't store a height property, also return height from this function.

---

**Find node that is not k-balanced but all descendants are k-balanced.**

This is a bit of a weird question, especially because they way they define k-balanced is different from how normal binary tree balance is defined (subtrees must also be balanced).  But often times these strange or overly specific questions are the easiest, because they pidgeonhole you straight into the only solution to the problem.

In this case you make a recursive function to check if all descendants are k-balanced, and as soon as it returns false you've found your answer.  Note that a leaf node will always be k-balanced, so when this happens, we will never return false right away, which would ignore the "all descendants are k-balanced" criterion.  (Now you just need to propogate this answer up the recursive call stack, either with an extra return value or a return parameter.)

---

**Find all paths to leaves whose path sum equals a given value.**

Here we have another one of these "find all the answers" questions, which usually means the only way is brute force - there's no room for cleverness or optimization.  We just traverse the whole tree and keep track of our current path and sum at all times.

The worst case time complexity is pretty bad of course, but there's nothing to be done about that.  Imagine the tree is a binary tree of all zeros and the target sum is zero.  Then all leaves satisfy the condition, for which there are n/2, and each path to a leaf has size log<sub>2</sub>(n), so the solution itself requires O(n log(n)) space to hold.  This means our algorithm's time and space complexities will both be O(n log(n)).

---

**Recursive to iterative algoirthm translation - nonrecursive binary tree traversals.**

These questions actually open the door to a very interesting category of problem in computer science, which is emulating the recursive function call stack with an iterative algorithm.  A nice general theoretical result to know is that it can always be done.

The basic idea is to start with a stack to store your recursive function calls.  For example, if we give a super simple example (will look like overkill here) of the factorial function, we have:

```c++
unsigned int factorial_recursive(unsigned int n) {
    if (n > 1) {
        // Note: intentionally writing this "backwards", i.e. recursive call before multiplication,
        // in order to make the translation to the iterative stack version more logical
        return factorial_recursive(n - 1) * n;
    }
    return n;
}

// This may seem overly complicated because you know the function is really tail recursive,
// but try to think about translating the above code directly to an iterative version
// (all the recursion happens before the multiplication), so we can get a more general understanding.
unsigned int factorial_iterative(unsigned int n) {
    std::vector<unsigned int> stack;
    // "Make recursive calls"
    while (n > 1) {
        stack.push_back(n);
        --n;
    }
    unsigned int result = n;  // n is now 1, or 0 if it started at 0
    // "Unwind function call stack"
    while (!stack.empty()) {
        result *= stack.back();
        stack.pop_back();
    }
    return result;
}
```

If you imagined that multiplication were not commutative and not associative, perhaps this translation above would make sense.  So this handles the translation of non-tail-recursive recursive functions to iterative functions.

But as you start to try to implement recursive functions with multiple calls in different places, you will notice another problem arise that this stack doesn't seem to handle.  Namely, when I return from my recursive function, it knows where in the code to return to automatically, but in the iterative version how do I track that?  For example:

```c++
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
```

How do we translate this to an iterative algorithm that still prints these A's, B's, and C's exactly as they are printed in the recursive version?  We need to keep track of the state ourselves in our stack!  Along with this, we'll introduce the idea of a return value stack, which will make our translation much more general.  It looks something like this:

```c++
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
```
Or view as source file: [recursive_iterative_translation.cpp](https://github.com/Apollys/EPI-Variants-Solutions/blob/main/Ch_09_Binary_Trees/recursive_iterative_translation.cpp)

For implementations of iterative binary tree traversals, see: [iterative_binary_tree_traversals.cpp](https://github.com/Apollys/EPI-Variants-Solutions/blob/main/Ch_09_Binary_Trees/iterative_binary_tree_traversals.cpp)  However, I would **highly recommend** making sure you understand the above translation code first, as it is more focused on what's important (i.e. not cluttered by the plumbing details of binary tree traversal) and it offers a great understanding of the more general problem we are tackling here.
        
---

**Preorder and postorder traversals with constant space, given nodes have parent pointers.**

These are going to be very similar to the inorder traversal, but the point when you save the current node's value to your result vector will be different.  This is more an exercise in plumbing that algorithm design, so I don't think there's much value in going into detail here.

---

**Binary tree reconstructions from two traversal sequences.**

This is actually quite an interesting question, and here I will discuss the theory behind all possible combinations of sequences.

Let's use the following general notation for our sequences:
 - Preorder traversal sequence: `[x_0, x_1, x_2, ..., x_n-1]`
 - Inorder traversal sequence: `[y_0, y_1, y_2, ..., y_n-1]`
 - Postorder traversal sequence: `[z_0, z_1, z_2, ..., z_n-1]`

**Preorder and inorder:**

So we are given `[x_0, x_1, x_2, ..., x_n-1]`and `[y_0, y_1, y_2, ..., y_n-1]`.  From this we can deduce that `x_0` is the root of the tree, since it is the first element in the preorder traversal.  We can then find `x_0` in the inorder traversal sequence, i.e. find `y_i = x_0`.  Then we know the inorder traversal sequences of the left and right subtrees are `[y_0, ..., y_i-1]` and `[y_i+1, ..., y_n-1]` respectively.  Finally, we know that the lengths of the preorder traversal sequences must match those of the inorder traversal sequences, so the preorder traversal sequences for left and right subtrees are `[x_1, ..., x_n-1]` partitioned by lengths of the respective inorder traveral sequences.

Now that we have obtained the preorder and inorder traversal sequences of the left and right subtrees, we can just recurse on each subtree.

**Inorder and postorder:**

Here we are given `[y_0, y_1, y_2, ..., y_n-1]` and `[z_0, z_1, z_2, ..., z_n-1]`.  From the postorder sequence we know the root is `z_n-1`.  As before, we can now find the `y_i = z_n-1` that partitions the inorder sequence into left and right subtree inorder sequences.  Finally, the lengths of those sequences tell us where to partition the postorder sequences.  As before, having determined the corresponding sequences for the left and right subtrees, we can now recurse on left and right subtrees and we're done.

**Preorder and postorder:**

In this case we are given `[x_0, x_1, x_2, ..., x_n-1]` and `[z_0, z_1, z_2, ..., z_n-1]`.  We know that `x_0` is the root from the first sequence, and `z_n-1` is the root from the second sequence (they must be the same value).  However, for the remaining part of the sequence, it's not immediately obvious how to partition it into left and right subtree sequences.  Let's look at a simple 7-node binary tree:

```
       1
     /   \
   2       3
  / \     / \
 4   5   6   7

Preorder:  [1, 2, 4, 5, 3, 6, 7]
Postorder: [4, 5, 2, 6, 7, 3, 1]
```
Here we can see that we could search for the matching continguous blocks within the preorder sequences (excluding first value) and postorder (excluding last value): `[2, 4, 5]` matches `[4, 5, 2]`, and `[3, 6, 7]` matches `[6, 7, 3]`.  Note that our definition of matching here is just that they contain the same values ignoring order.  (You could think of it as set equality, but for sets that allow repeated elements.)

You may be thinking: this doesn't seem very efficient.  And you would be right.  There's a flash of insight that can eliminate this matching problem entirely. Look back at the sequences again: just from the sequences, what are the roots of the left and right subtrees?  Is there any way those can be used to partition the traversal sequences?

The root of the left subtree (2 in this case) will always be the second element in the preorder traversal.  Symmetrically, the root of the right subtree (3 here) will always be the second to last element of the postorder traversal.  But now observe where the 2 is in the postorer traversal, and where the 3 is in the preorder traversal.  The 2 in the postorder traversal marks the end of the left subtree postorder traversal - this is no accident, this is by definition!  Symmetrically, the 3 in the preorder traversal marks the start of the right subtree preorder traversal.  This allows us to bypass the matching problem and partition the traveral sequences directly.

However, whether we use the left/right subtree root method, or try to find matching blocks, we don't always get a clear answer.  Consider the following case:

```
Preorder:  [1, 2]
Postorder: [2, 1]
```
So the root is 1, and are left and right subsequence blocks are `[2]` and `[]`, but which is left and which is right?  There's no way to know actually, it's completely ambiguous.  The following trees both have these traversal sequences:

```
   1      1
  /        \
 2          2
 ```
 
Therefore, an additional necessary condition on our sequences to remove ambiguity is that if a node has one child, it must have two children.  In other words, no node may have exactly one child.  Alternatively, you could allow single children, but assume that any lone child is always a left child.

With this ambiguity out of the way, we once again have extracted the corresponding sequences for left and right subtrees, and thus reduced the problem into two more recursive instances of itself.  So recurse on left and right subtrees and we are done.

---

**Construct max tree in linear time.**
