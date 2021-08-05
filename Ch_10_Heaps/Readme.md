## Chapter 10 - Heaps - Variants Solutions

---

**\[Not from the book\] What is the time complexity of constructing a heap?**

This question is not in the book but it's one of those questions with a very counterintuitive answer, and worth explaining here.

The construction of a heap of n elements actually only takes **O(n)** time.

How is this possible?  A heap is a binary tree, and in a binary tree, almost all the elements are very near the bottom of the tree.  Half the elements are leaves, three quarters of the elements are leaves or the parents of leaves.  So if you construct the heap in the natural top-down way, adding new elements to the bottom and bubbling them up as needed, you would potentially have to bubble up all the elements you added O(height) = O(log n) distance, giving the O(n log(n)) time complexity that seems intuitive.

However, if you build the heap in a bottom-up fashion, instead bubbling *down* elements that break the heap property, the number of elements that could be bubbled down a singificant distance is very small.  Suppose we wanted to know how many elements could be bubbled down by a distance roughly h/2.  Remembering the formula that the sum of 2<sup>i</sup> from 0 to n is 2<sup>n + 1</sup> - 1, we get:
 - Number of nodes in top half of tree = 2<sup>h/2 + 1</sup> - 1
 - Number of nodes in the whole tree = 2<sup>h + 1</sup> - 1

Dropping the `- 1`s, we have the equation nodes_whole_tree = 2<sup>h + 1</sup> - 1 = 2<sup>h/2</sup> * 2<sup>h/2 + 1</sup> = 2<sup>h/2</sup> * nodes_top_half.  So if the height of the tree were 20 for example, only 1/2<sup>10</sup> = 1/1000th of the nodes will be in the top half of the tree!

This is of course not a proof of the linearity of the time complexity, but more of a justification for the intuition behind it.  For a formal mathetmatic proof, there are plenty out there, just a Google search away.

**Follow-up question: Wait, but does this mean we're sorting n values in O(n) time?**

Suppose we have a max heap and I ask you to tell me the largest value.  Okay, you just give me the root, and it's O(1) time.  How about the second to largest value?  You just check the left and right children of the root, and give me the bigger one.  Again, still kind of seems like O(1) time.

But how about the 100th largest value?  In a sorted array we could index straight into it, in a sorted linked list we could walk straight to it (O(k) steps for the kth largest\*).  But in the heap you have no way of knowing whether to go left or right first, so you'd have to try both, and then for each of those children try both again, and so on...  The heap gives you no information about the relationship between elements in the left and right subtrees of a given node.  It would actually be possible, for example, that *all* elements of the left subtree are greater than *all* elements of the right subtree, or vice versa.  And this weakness of the heap is what makes a BST so useful.

\*Actually O(min(k, n-k)), but it's not really important for the discussion at hand.

---

