## Chapter 5 - Arrays - Variants Solutions

---

**Assuming that keys take one of three values, reorder the array so that all objects with the same key appear together. Use O(n) time and O(1) space.**

The algorithms presented on the previous pages already solve this problem in O(n) time and O(1) space, so there is nothing new to do here.  Just apply the previously given solutions.

---

**Given keys taking one of four values, group like keys together in O(n) time and O(1) space.**

Again, this is very similar to above and there are many ways to implement it, but the easiest is just to first group all elements with the first key value at the beginning of the array, then group all elements with the second key value after the first group, and so on.  This nicely leads itself to an algorithm for grouping with k possible key values: for each key, swap all elements of that key into the beginning of the unorganized segment of the array (which you track via a current_index variable).  This leads to O(n\*k) time complexity.

Of course, you can be fancier and first count the number of instances of each key, which then gives you the starting indices for each key group right away.  This way you'll know immediately where to swap each key upon first seeing it, giving a time complexity of O(n) (and space complexity O(k) of course).

---

**Given an array of objects with boolean-valued keys, group them by key in O(n) time and O(1) space.**

This is just a simpler version of the previous problems, so there's nothing to discuss here.

---

**Given an array of objects with boolean-valued keys, group them by key (false first, true second) while keeping the ordering of true-valued keys stable (same time and space as as above).**

We need the last true key to end up in the last slot, and the second to last in the second to last slot, and so on.  So we simply iterate from right to left to make sure we encounter the last true key first and put it at the end, and so on.  If we wanted to keep the first group stable instaed of the second, we would iterate left to right.

It is natural to wonder if we could do both easily.  To analyze this, let's think about what conditions on the swap cause the ordering to be changed.

The ordering changes when we "jump" a value over other equal values.  This means we can only swap values that are directly adjacent if we want to preserve ordering of both groups of values.  So if we think of a worst case scenario: starting with T1, T2, T3, F1, F2, F3 and aiming for F1, F2, F3, T1, T2, T3, each value has to move 3 steps, or in general transforming the array TTT...TFFF...F into FFF...FTTT...T requires that each value move n/2 steps, which yields O(n<sup>2</sup>) time complexity.

So you'd be best off just using the O(n) extra space instead, and keeping the runtime complexity O(n) time as well.

---

**Binary addition: Given two strings which contain binary integers, return the string containing the binary sum of those two integers.**

This is just binary addition, nothing special here.  Just like we did addition in elementary school, except it's even easier because there are only zeros and ones.

---

**Advancing through an array: Given an array of integers that tells you the maximum distance you can advance forward from the given position, compute the minimum number of steps needed to reach the last location.  (You can advance by any value less than or equal to maximum distance.)**

In the feasability problem (the example beforehand), we only needed to track the furthest we could reach so far.  Now we need to track the associated number of steps it takes to reach that square in addition.

The main trick here is that we now care about not just the furthest we can reach, but the furthest we can reach for each number of steps.  So we could have an array tracking the furthest reachable square per number of steps.  However, we would notice that at any given time the only important information is how far we could reach in the current number of steps, and in the current number of steps plus one.  We don't need to worry about previous number of steps, because we have already handled them, and we don't need to worry about more than one step forward, becase we can only take one step at a time.

So we track the following: `current_num_steps`, `reachable_in_current_num_steps`, and `reachable_in_next_step`.  Once `current_num_steps > reachable_in_current_num_steps`, we shift everything over, so `reachable_in_next_step` becomes `reachable_in_current_num_steps` and `current_num_steps` is incremented by 1.  Once `reachable_in_next_step` equals the last index in the array, we are done, because we know we already handled all the `current_num_steps - 1` cases, which would be the only way to reach the end in fewer than `current_num_steps + 1` steps (given we have to add 1 to `current_num_steps` when we take a setp).

---

**Remove all instances of a given value from an array, and shift left the remaining values.**

This is basically the same as the remove duplicates from sorted array example: we keep track of two indices, one is a read index and one is a write index.  If the read index points to a value we should remove, we simply increment the read index.  Otherwise we move the value at read index to write index and increment both.

---

**Find the longest subarray all of whose entries are equal.**

Keep running track of the length of the current block of equal elements as you walk through the array.  On seeing a new element, if it is equal to previous, increment length, otherwise set length to 1.  Update max length at each step.

---

**Buy and sell twice in O(1) space (and O(n) time).  Given an array of prices (in chronological order), what is the maximum profit earnable by buying, selling, buying, then selling? (You only buy/sell one "quantity" of the item at a time).**

The observation I made here is that the best solution is either (A) the greedy choice: find the single best buy and sell, and then find the best buy and sell of what is remaining, or (B) the greedy choice split by the interval of biggest decline (the worst buy and sell inside the best buy and sell).  I had to draw a bunch of graphs and go down a lot of false rabbit holes to deduce this, so I definitely recommend the reader to try to convince themselves of this with graphs and examples as well.

Either way, this decomposes the problem into a set of a fixed number of single buy-sell problems, all of which we can solve in constant space and linear time.  So we simply solve them all and select the best option.

Note: I thought that this was one of the hardest problems in this book, so don't worry if you didn't get the flash of insight to solve it.  I think understanding the linear time linear space algorithm is much more valuable and appliccable to real programming.

---

**Apply permutation in constant space**

This question is a fake/trick question: you can't apply a permutation in constant space and linear time in general.  You can, of course, increase your time complexity from linear to quadratic just to reduce space from linear to constant, but that is **never** worth it.

You could also make some assumption like permutation indices can all be stored in signed ints without overflow (which is how they've prototyped it in the book, but that would be completely foolish in practice -- they are indices and should be a `size_t` type).  Then you use the corresponding negative index to indicate the permutation for that element has been applied already, while retaining the original permutation value in a recoverable form.  But you're assuming here that the input data only uses half of its valid values, which indicates to me there's a problem in the software design already.  Anyway, see [this SO post](https://stackoverflow.com/questions/16501424/algorithm-to-apply-permutation-in-constant-memory-space) for a detailed walkthrough of how this works.

I really hate this kind of solution because you're assuming the input data is not utilizing all of its informational entropy, when there's no reason based on the semantics of the data that should be the case.  In general, if the input data isn't using its entropy and you need to optimize space, just eliminate the wasted space in the data storage, don't try to make up for it later in the algorithm (the latter can **never** be more efficient than the former, and will always be more esoteric and less maintainable, and usually require some magic flash of insight to implement).

---

**Transform a permutation into its inverse in constant space**

Again, same as above, it's not possible without extra space because you need to track which elements have been updated and which have not yet.  So you can fake O(1) space by assuming the input values only use half their space (you're really still using O(n) space -- the other half that the input left unusued), or you can add a factor of n to your time complexity.  Either way, I wouldn't worry about these problems, they aren't practical at all.

If you want a practical problem, try generating and returning the inverse of a permutation.  This should be quite straightforward but good practice with permutations and inverses.

---

**Compute the kth permutation under dictionary ordering, starting from the identity permutation.**

The obvious way to do this is to apply the `NextPermutation` solution k times, which gives you an O(k\*n) algorithm.

If we're trying to optimize, we could go digit by digit from leftmost to rightmost value, using some math to figure out what the digits should be.  For example, suppose that we have a permutation of length 4.  This means there are a total of 4! permutations, and more specifically: 3! permutations that start with 0, 3! permutations that start with 1, 3! permutations that start with 2, and 3! permutations that start with 3.  So based on the value of k, we can immediately determine what the first value of the permutation must be in O(1) time (use floor division of k by (n-1)!).

Now the same idea can be applied recursively to the remaining portion of the permutation.  Your new k is k - (number of permutations covered already), and your new length is length - 1.  The tricky bit is to make sure you map all the digits correctly, but hopefully the idea is clear.  I don't think the plumbing of this algorithm is particularly interesting, and I'll leave it to the reader to implement if they so desire.

---

**Previous permutation**

Clearly there's nothing special about previous versus next, everything is symmetrical and so this is exactly the asme problem as the next permutation example given.

---

**The `rand()` function returns a uniformly random number in `[0, RAND_MAX - 1]`. Does `rand() % n` generate a uniform random number in `[0, n - 1]`?**

Only if n divides RAND_MAX, otherwise no.

Example: let `RAND_MAX = 5, n = 2`, so given uniform random values in `[0, 4]`, let's see what happens if we mod by 2:

`[0, 1, 2, 3, 4]` --> `[0, 1, 0, 1, 0]`

We got one extra zero, so it's not uniform.  The only way we would get the same number is if the number of options we want evenly divides the number of options we are initially given.

You may try to remedy this by making multiple calls to your rng.  Let's suppose the rng produces uniform values in `[0, n - 1]` and you want uniform random values in `[0, m - 1]`.  What would calling your rng twice give?  You may be tempted to say it will give you `2m` options, but think for a second about rolling two dice: there are 36 possible options.  So calling our rng twice gives m<sup>2</sup> options, which isn't as helpful as we may have hoped.  If n didn't divide m, the odds that it's going to divide m<sup>2</sup>, m<sup>3</sup>, or some higher power of m seems very slim, especially because m is pretty much guaranteed to be a power of two itself in practice.

So your best bet is finding the largest value in `[0, m - 1]` that n divides, and whenever your rng gives you a value larger than this (which will be super rare), you just call the rng again.  Technically not halting, but practically very efficient.

---

**Given a uniform random number generator on `[0, 1]`, how would you generate a value from a continuous probability distribution?**

This is a bit of a "gotcha" math question: just use the [CDF](https://en.wikipedia.org/wiki/File:Normal_Distribution_CDF.svg) of the probability distribution, and check where it intersects your randomly distributed `[0, 1]` value.

---
