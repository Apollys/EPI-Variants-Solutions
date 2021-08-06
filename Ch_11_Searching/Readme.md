## Chapter 11 - Searching - Variants Solutions

---

**Find the first element greater than a given key in a sorted array.**

From the preceeding example we know how to find the first occurrence of a given key in a sorted array.  Symmetrically, we could tweak that algorithm to find the last occurrence of the given key, rather than the first.  The first element greater than the target key is just the subsequent element.

---

**Find a local minimum of an unsorted array, given A\[1\] <= A\[0\] and A\[n-2\] <= A\[n-1\].  (Local minimum is defined by element <= both neighbors.)**

Interestingly, the question as phrased in the book adds the qualification "...find a local mimimum, *if one exists*?", but based on the conditions given, a local minimum must exist.

The important insight here is that just sampling one point in the middle (like a normal binary search) doesn't necessarily give us enough information to remove a section of the input array from the possible solution space.  Draw a graph and try to find where you could draw a dot in the middle such that it doesn't help.  The answer is when the dot is below the current left and right endpoints - either the left or right half could have the local minimum.

So instead we sample two points in the middle of our current "active region," and use this to eliminate one-third of that region that cannot contain the solution.  Draw out graphs of the cases to see how you would handle each one, and rememeber that the condition for local minimum in this question is only loose inequality <=, not strict <.  This means a horizontal line segment has local minima everywhere along that line (except possibly its endpoints).

You may be worried that this seems "slow", because we're only eliminating one third, rather than one half, of the solution space at each step.  It's important to take a quick look at the math behind this, using the infinite geometric series sum formula (valid for r < 1).
```
Sum of infinite geometric series: 1 + r + r^2 + r^3 + ... = 1/(1-r)
r = 1/2      -->  sum = 2
r = 2/3      -->  sum = 3
r = (p-1)/p  -->  sum = p
```
So, the first takeaway is that the infinite sum converges as long as we take away any fixed fraction of the total solution space at each step.  And more specifically, if the fraction we take away is 1/pth (e.g. 1/2, 1/3, 1/4) of the solution space, then the sum converges to p, which means the runtime is p times the runtime of processing one iteration on the full input, or in other words O(p\*n) = O(n).

In practice, if this number does get very large, for example suppose you only eliminate 1/10th of the solution space per iteration, then the algorithm may actually be outperformed by O(n log(n)) algorithms.  However, you should not be worried about the difference between numbers like 2, 3, and 4.

---

**Find the interval enclosing a given key, i.e. the indices of the first and last occurrence of the given key, in a sorted array.**

Just combine the ideas of finding the first instance of the key with the last instance (which is just a reflection of find first instance).  For the second part, you can start your solution space at index of first that you just computed, (though of course it won't impact the overall runtime complexity).

---

**Test if *p* is a prefix of a string in a sorted array of strings.**

(Reworded slightly to clarify that the array is the thing that's sorted, not the individual strings within the array, which wouldn't make any sense.)

This is actually just a direct application of binary search, with your comparison operator being lexicographical comparison.

---

**Search for an element equal to its index in a sorted array. Repeated elements are allowed.**

In terms of asymptotic runtime complexity, there's nothing better to do here than just look at all the elements.  We can construct an example to demonstrate this:

```
Index: [0, 1, 2, 3, 4, 5, 6, 7, 8,  9, 10, 11]
Value: [1, 2, 3, 4, 5, 6, 6, 8, 9, 10, 11, 12]
```
No matter where we look in this array, we can't eliminate anything from the solution space.  Imagine we looked at index `2` and saw `A[2] = 3`.  There could be a solution anywhere to the left, for example maybe `A[1] = 1`.  Also, there could be a solution anywhere to the right, e.g. maybe `A[3] = 3`.  This same logic applies to every element in the array (except ends).  So in general we can't do better than just walking through the array.

In most cases you can do a bit better though.  It's not going to be reflected in the runtime complexity of course, but the algorithm will be faster, especially on less pathological inputs.  So let's write out a less pathological input now:

```
Index: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
Value: [3, 4, 4, 5, 5, 5, 5, 5, 6, 7]
```
Suppose we start at the beginning and discover `A[0] = 3`.  Is it possible for `A[1]` to contain a solution, i.e. `A[1] = 1`?  No, because the array is sorted.  So since we've already seen a value of 3, the first possible position a solution could occur at is index 3.  So let's jump straight to index 3 and read the value there: `A[3] = 5`.  Now we've seen a 5, so the first possible index of a solution is 5.  Jumping straight to 5, we discover `A[5] = 5` and we're done.

This may seem like we've found the optimal solution, but we're not all the way there yet.  We've kind of forgotten about one more scenario, because we've ignored negative numbers.  Let's look at another example where this above algorithm is useless:

```
Index: [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
Value: [-1, 0, 1, 2, 3, 4, 6, 6, 6, 7]
```

Our algorithm above doesn't provide any shortcuts, because if we start from the left, the array value is always less than the index.  But can we apply a reflection of the same idea, starting from the right?  Of course!  We look at the right and see `A[9] = 7`.  Since values must decrease or stay the same as we travel left in the array, that means there's no chance of finding a solution to the right of index 7.  We look there and see `A[7] = 6`, this tells us to go to index 6, and we find the answer.

So if we interleave these two methods, checking from the right and the left, we will get the optimal skip in any scenario.  It may seem like a lot of work just to optimize an O(n) algorithm to a faster O(n) algorithm - and it is.  So if you were happy with the initial O(n) algorithm, having proved there's no way to get sublinear time complexity, that's totally fine.  But if this were an algorithm that really ran a lot and needed to be optimized, the difference between looking at every element and using the jump algorithm devised above would be very significant on realistic input arrays.

---

**Find the maximum element of a strictly ascending followed by a strictly descending sequence.**

Similar to how we approach the local minimum question, we want to sample two points in the middle of our active region and then try to eliminate at least one region based on our sampling.  Try to draw out the graphs yourself and do a case analysis to find out which region(s) can be eliminated under which circumstances.  Is the *strict* increasing/decreasing necessary? Why couldn't it be non-strict increasing/decreasing?

If the two sampled points are equal, we can actually eliminate both end regions (because of the strictly increasing/decreasing property - if it weren't strict we wouldn't be able to eliminate anything!).  If they're not equal, we can eliminate the end region corresponding to the lesser value.

---

**Search a cyclically sorted array for a given value k.**

From the previous sample question, we already have the algorithm to find the minimum element of a cyclically sorted array.  We can then split the array at this minimum element, giving us two standard sorted arrays.  Checking against their endpoints, we can find which of those two arrays *k* belongs to, and then binary search within that array to find *k*.

---

**Compute x/y to tolerance ϵ without using division (or other more advanced mathematical operators).**

We can rephrase this as a search problem: Given x and y, find z such that z * y = x.  If z * y > x, we need a smaller z, if z * y < x, we need a bigger z.

The only tricky part is dealing with the tolerance.  So we want the z we find to be at most ϵ away from the real x/y.
```
Let z' = x/y = exact answer
Then we must find z in [z' - ϵ, z' + ϵ]
--> z * y in [(z' - ϵ)*y, (z' + ϵ)*y]
Using z' * y = x, this gives
--> z * y in [x - ϵ*y, x + ϵ*y]
```
In other words, our tolerance for |x - z\*y| is ϵ\*y.

---

**How many comparisons does it take to find the min and the max in the worst case (with the optimal algorithm)?**

They already answered this above, it's n/2 (split into min candidates and max candidates) + n/2 - 1 (find min among n/2 min candidates) + n/2 - 1 (find max among n/2 max candidates) = 3n/2 - 2.

Unless they want to get into off-by-one issues based on the parity...  In that case, if n is odd we have an extra element left over after floor(n/2) initial comparisons, so we add it to each group and get an extra 2 comparisons in total.  So if we do the math precisely:
```
n even --> 3n/2 - 2 comparisons
n odd  --> 3(n-1)/2 - 2 + 2 comparisons
         = 3n/2 - 3/2 comparisons
```
So it's "half a comparison worse" as a function of n (remember there aren't actually any half comparisons happening here because n is odd), but this whole analysis seems very silly to me.

---

**Find the median element of an array.**

Median is just a special case of kth largest element, specifically k = n/2 if n is odd, or the average of k = n/2 and k = (n-1)/2 if n is even.

You may have heard of O(n) linear-time median finding or kth largest element finding algorithms, which do exist, but they are quite impractical as they have a high constant coefficient in front of that O(n).  Unless you want to learn how they work out of pure intellectual curiosity, I wouldn't worry about them much.  Randomized pivot-partition algorithms are much more useful to understand.

---

**Find the kth largest element, with duplicates.**

Again use a randomized pivot-partition algorithm, but make sure your partitioning algorithm is stable.  (This was probably covered earlier in this book in the "Dutch National Flag" problem.)

---

**A number of aprtment buildings are coming up on a new street. The postal service wants to place a single mailbox on the street. Their objective is to minimize the total distance that all residents have to walk to collect their mail each day.  Given an array representing the number of residents in each building and the x coordinate of the building along the streen, determine where to place the mailbox to minimize the sum total walking distance of all residents to the mailbox.**

This is a bit of a flash-of-insight type of problem.  It's quite different from the preceeding problems so don't expect it to have anything to do with median finding or binary searching.  In fact, I'm not really sure what it's doing in this chapter at all.

Try drawing some pictures and see if you can answer the question: given a certain starting mailbox position, how can I tell which way to move it to improve the solution?

Let's say there were only two buildings, one on the left with 10 residents and one on the right with 1 resident.  I start with the mailbox directly in between the two buildings, let's say 1 unit of distance away from each building.  Which way should I move it to reduce the sum total of everyone's walking distance?

Move it toward the building with 10 residents!  Then 10 people walk 1 unit less each, or 10 units less among them all, while 1 person walks 1 unit more.  So we save 9 units of walking by moving it left 1 unit.  Now how would you apply this idea to the more general situation?

All you need to know is how many people there are to the left of the mailbox and how many there are to the right, for each potential mailbox position.  You may have also observed by now that there's no real reason to consider positions between apartments - an optimal solution will necessarily occur right in front of one of the apartment buildings.  So now we have all the pieces, just put them together.

Sort the buildings by x coordinate, then forget about positions and just work with the populations of each building.  Precompute the total sum of building populations, so as we go we can just track the cumulative left sum and compute the right sum as total_sum - left_sum.  Starting with the mailbox in front of the first building, we track the cumulative left sum as we mentioned and compute the right sum.  If right sum is greater, we move the mailbox to the right.  At each step, see if the current placement is better than the optimal so far (least delta between left and ring sums is the goal). Once right sum is less than left sum, we can stop (it will never get to be more again by walking further right).

Time complexity: O(n log(n)) from sorting the buildings by x coordinate.  If the buildings are given in sequence, then O(n).

---
