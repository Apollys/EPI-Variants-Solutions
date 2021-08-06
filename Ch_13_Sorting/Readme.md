## Chapter 13 - Sorting - Variants Solutions

---

**Compute the h-index for an array of integers.  You cannot alter the array and cannot allocate additional memory.  The h-index is the largest value h in the array such that there are at least h elements in the array greater than or equal to h.**

I will start off by saying I'm not really a fan of this question.  If you thought just sorting the array was a great solution, I agree with you!  Who wouldn't want their data sorted anyway? It's hard to imagine a scenario where sorting data would be considered harmful.  And it's not like we're going to be able to get faster runtime by avoiding sorting.

That being said, let's jump into it.  If you can't sort or modify data in any way, and you can't allocate additional memory, what *can* you do?  Well, about all you can do is guess a value and try it.  The nice thing is, if the array contains n values, the h index can't be any greater than n (there can't be more than n values greater than or equal to something, because there just aren't more than n values), and so we have a limited range of values to guess from: \[0, n\].

Maybe this suggests to you the idea of trying a binary search!  We have a range, and if we try a value and it works, we know we only need to look higher, if we try a value and it doesn't work, we only need to look lower.  So we are binary searching on the potential h-index, not on the array itself.  At each iteration of the binary search, we need to check how many values are greater than or equal to our h, which means we have to walk through the whole array and count them (if we could make some sort of data structure or sort the array, this would be a whole lot easier of course).  So each iteration takes O(n) time, and there are potentially log<sub>2</sub>(n) iterations, so our final time complexity is O(n log(n)).

---

**Compute the h-index of a sorted array.**

This is literally just the example with the sort line deleted.

---

**Suppose you can allocate as much additional memory as you want. Design a fast algorithm for computing the h-index.**

The trick here stems from the realization we came to in the constant memory solution above: the solution *must* lie in the range \[0, n\].  So is there any different to the answer between seeing the value n, n + 1, n+ 5, or n + 100000?  Nope!  They all would contribute a count of 1 to every possible h-index up to n, and so they're all the same.  This means we can "bucket sort" this array and just glob everything greater than or equal to n in one bucket:

```c++
int ComputeHIndex(const std::vector<int>& input_vector) {
    const size_t n = input_vector.size();
    std::vector<int> count_vector(n + 1, 0);  // initialize vector of n+1 zeros, corresponding to 0..n
    for (int value : input_vector) {
        if (value > n) ++count_vector[n];
        else ++count_vector[value];
    }
```
So now what?  There's one trick left to extracting the final answer from this count vector in a single pass.  See if you can figure it out.

Without thinking about how to implement it for now, how many values are greater than or equal to i, as a function of count_vector?  The answer is... the right partial sum of the count vector from index i to index n.  Now, how could we know this with just a single iteration?

Just iterate from right to left through the count vector, keeping track of a running sum.  As soon as that sum reaches or exceeds your current index for the first time, you've found your answer.

Time complexity: O(n), since we avoided sorting by doing this counting trick instead.  Space complexity: O(n).

---

**Users share an internet connection, user i uses a specified amount of bandwidth over a specified time interval.  What is the peak bandwidth usage?**

This is exactly the same as the example, but you track bandwidths instead of just a count.  Note that their explanation of the sample is super confusing (I wouldn't have understood it if I didn't know the solution already, and I was still confused even though I did).  They tell you to focus on "endpoints", but what they really mean is to consider start and end points uniformly.  The name `Endpoint` that they use is horrible.

I would design it by first creating a `PointOfInterest` struct:

```C++
struct PointOfInterest {
    int time;
    double bandwidth_delta;  // positive at interval start, negative at interval end
};
```
Then convert all your input data to points of interest (making sure to set `bandwidth_delta = -bandwidth` for interval endpoints).  Sort by the `time` field, and you can consider using either `bandwidth_delta` or `-bandwidth_delta` as the secondary key if you want to control how to handle cases where one interval starts and the same time another ends.  (Sorting by `bandwidth_delta` as secondary key would make all interval endpoints occur before start points, and using the negative would have the reverse effect.)

Finally, you can just iterate through your sorted points of interest, doing `current_bandwidth += bandwidth_delta` at each step, and tracking the max bandwith encountered.

---

**Given a list of employee salaries and a target total salary budget, compute the salary cap that yields sum(salaries) = target total salary budget.  The salary cap lowers any salary above the cap to the cap, and does not affect any other salaries.  Use O(1) space complexity.**

Thinking back to the O(1) space h-index problem, I'm again thinking that there's not much we can do other than try a solution and see if it works, and again we know which way to go based on whether or not it worked, so trial-and-error binary search seems to be the way to go.

Suppose we have a current guess for the salary cap, and we want to check what the sum of all salaries will be given that cap.  Well, we have to iterate through the array, clipping each salary to the cap, and then adding this to a running total.  This takes O(n) time.  Now how many iterations of the binary search do we need to do?  Well we can safely assume that the salaries and salary cap must all be integers, since they are salaries (measured in whole cents).  So if the max salary is M, this would give us O(log(M)) iterations, or O(n log(M)) total runtime.

But remember, we like to measure complexity in terms of input *size*, not input *value* (measuring in terms of input value is a good way to accidentally think you've solved an NP-hard problem in linear time), so in terms of size, M = 2<sup>b</sup>, where b is the number of bits in the integer data type holding the salary.  Thus, our time complexity becomes O(n * b), which you could think of as exactly linear in the input size.

**Bonus side note: why did I assume that salaries can be stored as integers? What's wrong with them being floating point values?**

Binary searching on integers is easy.  If we take the midpoint of an integer interval, we are guaranteed to have split the total number of integers in the interval in half.  However, if we take the midpoint of a range of floating point data, do we still have that guarantee?  What portion of values ended up in the left versus the right half?  Remember the whole point of floating point values is that they have a non-uniform density within the range of values they represent -- that's how they can represent numbers like 0, 1, 2, and 3, but also numbers way out of the range of integers like a trillion or one trillionth.

I'm sure people have devised formulas to binary search on floating point values and roughly evenly split a given range, in fact there may even be a proof that splitting in the middle is good enough, however I did want to make the point that it's not trivial and shouldn't be glossed over without at least realizing what assumptions are being made.

---
