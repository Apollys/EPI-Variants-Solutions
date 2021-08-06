## Chapter 12 - Hash Tables - Variants Solutions

---

**Group words into anagrams, assuming words consist only of lowercase English characters. Assuming there are n words, each of length at most m, the total runtime complexity should be O(nm).**

The solution is the same as the previous example, but here they just want us to say that since there are only 26 options for characters, we can sort in linear time (just count the number of instances of each character, for example).

---

**Given an array, find the shortest subarray that contains each distinct value in the given array.**

This is the same problem as the previous example, but you just have to find all the distinct values first.  Use a hash map/set for this (`std::unordered_set` in C++).

---

**Given an array, rearrange the elements so that the shortest subarray containing all the distinct values in A has maximal possible length.**

Right off the bat, you might be thinking if I had two values that only occurred once in the array, I could just put one at the beginning and one at the end and that requires the subarray to be the whole array.  This is a great starting intuition.  In general, we want the elements with smallest number of repetitions on each end, which forces the largest possible subarray (the only values it will not contain are the repetitions of these two elements).

---

**Given an array A and a positive integer k, rearrange the elements so taht no two equal elements are k or less apart.**

This problem can be tricky to design a hard enough example that you don't accidentally cut corners, while also making it simple enough that you can reason about it logically.  I'll give you a good example to get starting with:

```
Array = [a, a, a, b, b, c, c, d, d], k = 2
I like to visualize the array like this:
[ a          ]
[ a  b  c  d ]
[ a  b  c  d ]
```
Imagine grabbing letters of the top of the stacks as you go.  If you just make groups of 3, starting from the beginning each time, you'll get a sequence like this: `[a, b, c, a, b, c, a, d, d <-- FAIL]`, where we have a problem because the two d's end up too close to each other.

However, what if we just rotate around cyclically?  `[a, b, c, d, a, b, c, d, a,]`  In fact, we didn't even use k in this process at all.  Is there any way this could fail?  What about the case where we have a bunch of a's, and then one of each of many other letters:
```
[ a                   ]
[ a                   ]
[ a  b  c  d  e  f  g ]
```
Now the cyclical method will get us into trouble, we need to keep using `a` in each group of 3.

To me, this suggests that there isn't any algorithmic trick to employ that can avoid the necessity of keeping the counts of each value updated at all times and grabbing the 3 most common remaining values at each step.  Strangely, this problem is given before the BST chapter, but this seems like the perfect job for a BST.  (I suppose with some effort you could wrangle a max heap into doing what you what as well, which is maybe what they're getting at, but a BST seems more suited to the task to me.)

So my solution is keep a BST of (count, value) pairs, with count as the key (and value as the secondary part of the key to break ties in a consistent way), and append elements into the array in groups of k + 1.  For each group, you grab the largest k + 1 values from the BST and decrement each of their counts by 1 (if count reaches 0 you can just remove them from the BST).  Once the BST is empty, you're done.  It's important to break ties consistently within the BST ordering, consider for example you have 5 a's and 5 b's, and k = 1 (so you're adding in groups of 2).  Say the first time you go to your BST, it gives you a then b.  Now the BST has 4 a's and 4 b's, and you *must* get a before b this time as well to avoid adjacent b's.

---

**Chess position hash function: how can you include castling rights and en passant information?**

Let's also add player-to-move because that's pretty easy while we're at it.

Following the vein of their hash function design, we can generate one more code for black to move, the absence of this code will indicate white to move.  So on each move update, we simply XOR the current hash with the black-to-move code.

For castling rights, we can generate two more codes per player, corresponding to "can castle kingside" and "can castle queenside".  We will just XOR the state with the corresponding castling rights code(s) when a player loses their castling rights for one or both sides.

For en passant, there are 8 positions an en-passant-capturable pawn may stand per side (4th rank for white, 5th rank for black), so we can just generate 16 codes corresponding to each of these squares en-passant-capturable.  Note: we could only use 8 codes, since only one side's pawns may be en-passant captured at any given time, but for simplicity we just do 16 (there are already over 800 in the base hash function anyway).  The update will just be a simple XOR anytime a pawn moves two squares forward, and XOR the en-passant code away on the next move.

---
