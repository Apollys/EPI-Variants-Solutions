## Chapter 7 - Linked Lists - Variants Solutions

---

**Merge two sorted doubly linked lists**

This is just like the example of merging sorted singly linked lists with a little more pointer bookkeeping, nothing special here.

---

**Reverse a singly linked list**

This is just an easier subproblem of the preceeding example to reverse a sublist.

---

**Reverse blocks of k nodes at a time**

Just apply the reverse sublist solution to blocks of k elements at a time.

---

**Is this cycle-finding code correct?**

No, focus on the following lines:

```c++
slow = head;
while (slow != fast) {
  slow = slow->next;
  fast = fast->next;
}
```
This clearly gives no guarantee of the "slow" and "fast" pointers colliding, since they're moving at the same speed. They will just be running circles around each other forever (unless you get really lucky).

---

**Remove values appearing more than m times in a sorted singly linked list**

Similar to the example of removing duplicates, but you just have to count the number of repetitions before removing an element.

---

**Test whether a double linked list is palindromic**

This should be a trivial application of the definition of a palindrome.  Iterate from the ends inwards.

---

**Add integers represented by singly linked lists with MSD first**

Reverse the lists, apply the previous solution, reverse the result, and re-reverse the input lists so they return to MSD first order.
