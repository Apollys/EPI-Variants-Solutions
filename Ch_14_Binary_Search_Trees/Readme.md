## Chapter 14 - Binary Search Trees - Variants Solutions

---

**The book asks the questions of reconstructing a BST from preorder and postorder traversal sequences.  What ambiguity arises if we try to reconstruct a BST from an inorder sequence?  Furthermore, what additional constraint could you add on the BST to eliminate this ambiguity?**

You will notice a very interesting result when you start writing out inorder traversal sequences of a BST: it will always be the sorted sequence of the values, no matter the shape of the tree.

However, suppose we said we wanted the BST to be a *complete binary tree* (all levels full except possibly the last, nodes in the last level as far left as possible).  With this added condition, there is exactly one BST for each given inorder traversal sequence, and it may be a good excercise to think how you would write the algorithm for reconstructing that tree.

---

**\[Addition question not from book\]  Given a BST, compute a sequence such that inserting the values from this sequence in order into an empty BST would yield the given BST.**

There are actually many such sequences, but the easiest thing to do is to first start with the root node, then insert the root nodes of the left and right children, and so on...

**How would you compute *all* possible sequences that generate the given BST?**

This is a bit more tricky, and I would definitely recommend the reader to think about how they would solve it, (but not necessarily write out all the implementation details, as the plumbing overhead can sort of overpower the important algorithmic ideas in this case.)

So step one is to start with the root - this is a necessity.  But after this, you may observe that the left and right subtrees are completely independent.  We could fill in the entire left subtree first and it wouldn't mess with the structure of the right subtree, or we could fill in the entire right subtree first and end with the left subtree.  Or we could do any sort of interleaving of the two.

This lends itself to designing a recursive algorithm.  If we knew all the possible sequences for the left and right subtrees, we would just pick every pairwise combination of left and right subtree sequences, and interleave them in every possible way, and that gives us all the possible sequences for the tree rooted at the current node.

Note that this is going to lend itself to a solution size that explodes exponentially, but that's the nature of the problem (once again, it's one of those "find all the answers" questions).  And like I said, the implementation details of tracking all the sequences and picking all the pairs and interleaving them all does get a little messy, so if you understand the algorithm and aren't interested in implementing it yourself, that's perfectly sensible.

Here's my code for this problem though: binary_search_tree_sequences.cpp

---
