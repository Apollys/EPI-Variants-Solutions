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

On 2nd thought, slow and fast pointers may in fact collide. Following is the math:

let (where all lengths are measured in traversal steps)
  L = length of the path that leads to the cycle
  C = length of the cycle
  c = length of the traversed path in cycle, when slow & fast meet for the 1st time (before fast pointer halves speed to 1 step per iteration)
  m = no. of cycles that the fast pointer has passed thru, when slow & fast meet for the 1st time
  n = no. of cycles that the slow pointer has passed thru, when slow & fast meet for the 1st time

therefore, when fast & slow pointers meet for the 1st time:
  distance that fast pointer has travelled
    = L + mC + c
  distance that slow pointer has travelled
    = L + nC + c
  
since fast pointer has travelled 2x distance of slow pointer in the same no. of iterations:
  L + mC + c = 2 * (L + nC + c)

reordering yields:
  c = (m-2n)C - L
  
hence, after fast & slow meet for the 1st time, and when fast halves its speed to 1 step per iteration, 
the fast pointer has to travel the below distance before it reaches the entrance to the cycle:
  distance to reach cycle entrance 
    = C - c
    = L + (1-m+2n)C

Note that since fast is now in the cycle, travelling L + anything * C is equal to travelling L.
Therefore, after slow restarting at head and travelling L, fast should also touches L, and they should meet.

(I tested several scenarios and found the variant algorithm to be correct. Though I may be wrong; so if anyone can find a counter-proof scenario that invalidate the variant algorithm, please let me know, thanks.)

---

**Remove values appearing more than m times in a sorted singly linked list**

Similar to the example of removing duplicates, but you just have to count the number of repetitions before removing an element.

Since requested by a forum user, here is my C++ code for this problem: [remove_duplicates_linked_list_variant.cpp](https://github.com/Apollys/EPI-Variants-Solutions/blob/main/Ch_07_Linked_Lists/remove_duplicates_linked_list_variant.cpp)

---

**Test whether a double linked list is palindromic**

This should be a trivial application of the definition of a palindrome.  Iterate from the ends inwards.

---

**Add integers represented by singly linked lists with MSD first**

Reverse the lists, apply the previous solution, reverse the result, and re-reverse the input lists so they return to MSD first order.
