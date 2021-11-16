## Chapter 17 - Greedy Algorithms - Variants Solutions

---

**Ray covering problem - given a set of circular arcs, compute the minimum number of rays that intersect all circular arcs at least once.**

For this problem I would a `PointOfInterest` struct that represents either an arc start or endpoint:
```c++
struct PointOfInterest {
    size_t arc_id;
    bool is_end;
    double angle;
};
```

Each arc can then be represented as two `PointOfInterest`s (we can just use the arc's index in the input array as its id).  Convert the input arcs into `PointOfInterest`s and sort them all together by angle.  Assuming the arcs represent closed intervals (include their endpoints), we want start points to appear before end points, so is_end should be our secondary key to sorting (note: is_end = 0 for start points, 1 for end points, so start points come first).  If we wanted the intervals to be open (excluding their endpoints), then we could change the `is_end` field to `is_start` and then end points (is_start = 0) appear before start points (is_start = 1).

We will also keep a hash map that allows us to track which arcs are currently "open" (we have encountered their start point, but not yet encountered their end point).  In C++, this would just be a `std::unordered_set<size_t>` (in other languages, any sort of hash set or hash map to boolean values works).  Let's call this `open_arc_set`.  In addition, let's keep a set of arcs that have been covered already, called `covered_arc_set`.

The next trick is to decide where to start.  Pick any interval start angle and traverse forward through the sorted `PointOfInterest`s in sorted order (rotate around the circle) until you reach the first end point.  There must be a ray here (specifically anywhere between here and the last interval start angle) in the optimal covering of these arcs, so this will be our starting point.  Any arc start point reached during this initial traverse should be inserted into open_arc_set.

Now we add a ray at the current position to our solution.  In practice, since the question doesn't ask us to track the rays, only count the number, this translates to incrementing a `ray_count` variable by 1, moving the elements in `open_arc_set` into `covered_arc_set`, and clearing `open_arc_set`. We then continue iterating counterclockwise through our points of interest.  Note: when we reach the end of the array, we cycle back to the start - our end condition is not reaching the end of the array, but `covered_arc_set.size() == input_array.size()`.
 - At each iteration, if we encounter an interval start point, we add that interval id to `open_arc_set`.
 - If we encounter an interval end point, we check if the corresponding interval is not yet covered (interval id is not in `covered_arc_set`), if the answer is yes, we add a ray at the current position as described above.
 - Once the size of `covered_arc_set` reaches the total number of arcs, we are done.

Time complexity is O(n log(n)) to sort plus O(n) to find the start angle and O(n) for the remainder of the algorithm, yielding O(n log(n)) in total.

Space complexity is O(n) for the sorted arc `PointOfInterest`s plus O(n) for the hash sets, yielding O(n) total.

---

**There are a number of points in the plane that you want to observe.  You are located at the point (0, 0).  You can rotate about this point, and your field of view is a fixed angle.  Which direction should you face to maximize the number of visible points.**

First, sort points by their angle to the origin (distance will not be important, so we can throw that out, just getting an array of sorted angles).

Next, we're going to walk through the array using a start and end index, both starting at 0.  Initialize `max_num_points` and `current_num_points` variables both to 1.
 - Increment end index (corresponds to moving the end of the field of view to the next point) and increment `current_num_points`.
 - While the angle difference from end index to start index is greater than your given field of view, increment start index and decrement `current_num_points`.
 - Now we have a new valid field of view.  Update `max_num_points = max(max_num_points, current_num_points)`.
 - Again, this is a cyclic problem, so when indices increment, we wrap around to the beginning of the array where appliccable.
 - Our termination condition is that start_index returns to 0.

For time complexity, assuming the input field of view is never more than 360 degrees, we could never make more than two full passes through the array (one full pass with start index always, and end index is field-of-view angle ahead of start index), so the iteration part of the algorithm is O(n).  That means the total time complexity is dominated by sorting: O(n log(n)).

Space complexity is O(n) for the sorted angles.

---

**\[Example Problem] Three sum problem - given an array and a target sum, find if any three values in the array sum to the target sum.  Repeated uses of the same element are allowed.**

Note: this is the example problem, not the variant.  I wanted to start with it because there is a much simpler algorithm than the book's algorithm that has the same time complexity.  You can just iterate through pairs of values a\[i], a\[j] in the array, at each step checking if the missing value to make the target sum exists in the array (target_sum - a\[i] - a\[j]).  This existence check can be done in O(1) time with a hash set of values in the array.

This gives us O(n<sup>2</sup>) time for the pairwise iteration and O(n) time for the hash set.  Now, the book claims their algorithm uses O(1) space, but they sorted the input - so it's not really O(1) space.  They're re-using the O(n) input space to organize the data in a different way, and if the input were `const` then they would need to explicitly allocate an extra O(n) space for the sorted array.

Of course, you can often make the argument, well who wouldn't want their data sorted?  And a lot of times that's valid, sorting data is generally helpful, or at the very least not harmful.  However, here's a classic case where it would be harmful: Suppose I have (key, value) data sorted by key.  Now I pass that data along to Alice and ask her to solve the 3-sum problem with the `value` item of this data.  If I give her non-const data, and she sorts it by value, I lose my key-sorting.  So, in this case the book solution to the 3-sum problem would clearly require O(n) additional space.

So my underlying point is that I thought the book's solution to this question was a little silly, as they went way overboard on a complicated "clever" solution that doesn't improve performance at all over the simpler thematic solution that occurs as a natural extension of the 2-sum problem: build a hash set of values that exist, and iterate through the array, checking if the value you need to reach the target sum exists in the hash set.

---

**Solve the 3-sum problem when the 3 elements must be distinct.**

Clarification since the example in the book doesn't address this: presumably the elements can be *equal*, for example, if the input array contained two threes we could use two threes, they just must be different threes.  But we couldn't use three threes because that would involve double-using one of the threes.

The nice thing about the hash set solution above is it can be easily extended to handle this scenario.  All we need to do is now track the counts of each value in the input array, instead of just existence (so we'll use a hash map from value to count).  Then when we're checking if the missing value exists, we need to make sure that the count in the hash map is greater than the number of instances of that value we've used already.  For example, if A\[i] = 6, A\[j] = 6, and our missing value is 6, we need to check if the hash map's count associated with 6 is greater than 2.

One other minor thing to note is in this case our pairwise iteration will be generated by:
```
for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
        ...
    }
}
```

Whereas above, we would have started `j` at `j = i`.

Same time and space complexities as above: O(n<sup>2</sup>) time and O(n) space.

---

**Solve the k-sum problem: determine if there exist k elements in the input array that sum to a given target sum.  Assumption: values may be reused as many times as we want (extremely unclear from the book which version they want us to solve).**

My first instinct on seeing this problem was to again extend the hash set idea to handle combinations of k values.  If k values sum to target_sum, then that must mean sum(some k/2 values) + sum(some other k/2 values) = target_sum (let's assume for now k is even).  So for example, if k = 8, and I knew all possible 4-value sums, then I could iterate through my 4-value sums looking for the missing value `target_sum - current_4_sum` in my 4-value sums set.

This gives a very general and relatively easy to implement solution to this problem.  The main drawback that I feel instinctively is that it doesn't seem very efficient.  We haven't really done much optimization, but on the other hand, I'm not sure there's even room for much optimization in this problem.  Since I know that integer sum problems are hard in general, and specifically Integer Linear Programming is NP-Complete, it seems at least justifiable that there may not be a better algorithm to solve this problem.

Let's analyze the time complexity and space complexities:  We need to build the set of all possible sums of k/2 values (separate sets for floor and ceiling if k is odd), which could have size up to n<sup>ceil(k/2)</sup>, so this will give us O(n<sup>ceil(k/2)</sup>) time and space complexities to precomupte the sum set(s).  Once the precomputation is done, we just need to iterate through the sums and check if the difference to reach the target sum exists, so that will again be O(n<sup>ceil(k/2)</sup>) time, and no additional space.

Final complexities: O(n<sup>ceil(k/2)</sup>) time and O(n<sup>ceil(k/2)</sup>) space.

Note: a quick Google search on this problem has shown that this is in fact a good solution, and improving beyond this is an open field of research.

**What if elements must be distinct?**

Then the hash set that contains k/2-sums can be modified to also store the indices of the values that generate each sum (this increases space complexity to O(k/2 * n<sup>ceil(k/2)</sup>)).  Then when we check for target_sum - current_sum, we make sure there are no repeated indices, which will also cause time complexity to increase to O(k/2 * n<sup>ceil(k/2)</sup>).

---

**Fuzzy 3-sum problem - given an array and a target sum, find the set of 3 distinct elements in the array that sum to the nearest value to the target sum.**

First, I'm going to start by brainstorming a couple ideas.  A (semi) brute-force solution that we can come up with right away is to compute all sums of pairs, then sort them (or store them in a BST), and then for each value in the input array, find the nearest value to `target_sum - current_value` in the sorted array (or BST).  This will require O(n<sup>2</sup>) space to hold the precomputed pair sums, O(n<sup>2</sup> log(n)) time to sort these precomputed sums, and then O(n log(n)) time to find the optimal value in the sorted array (or BST) per input value.  This gives us a nice baseline to beat: O(n<sup>2</sup> log(n)) time and O(n<sup>2</sup>) space.

We can flip around what we save in the sorted array or BST, and what we are iterating through.  This saves us space complexity, because the iteration absorbs the O(n<sup>2</sup>) part of the algorithm instead: We simply build a sorted array or BST of the input values (O(n log(n)) time and O(n) space) and iterate through pairs of input elements, finding the nearest value to `target_value - value_1 - value_2`.  This keeps the same time complexity, but reduces space complexity to O(n).  So the question is, can we eliminate this log(n) factor in the time complexity?

Let's start with an observation: as with the original 2-sum and 3-sum problems, we can always solve a 3-sum problem by re-using the 2-sum solution and adding an O(n) factor.  We know that 2-sum can be solved in linear time, but since our 3-sum problem is now a fuzzy-3-sum problem, we need a fuzzy-2-sum solution now.  So let's focus in on the fuzzy-2-sum problem, and see if we can find a linear time solution.

Storing values in a hash set isn't going to help here, because we are looking for fuzzy solutions.  And searching in a BST every time isn't going to work either, because it adds the extra log(n) factor to our runtime.  So we're looking for a way to walk through the array, finding the pair of values whose sum is nearest a given target.  Let's pre-sort the array since we only have to do that once, and in the larger scope of the 3-sum problem, a single O(n log(n)) operation will be trivial.  Now how do we walk through the sorted array?

Instead of starting with our two indices right next to each other, let's start with one at the beginning and one at the end.  This way, if the sum is too big, we can decrement the right index, and if the sum is too small, we can increment the left index.  Since everything is sorted, incrementing an index is guaranteed to increase the sum, and decrementing an index is guaranteed to reduce the sum.  We'll store the best pair as we go, and terminate when the indices meet.  But does this guarantee we find the optimal pair?

We might be worried about some situation such as the following arising, in which we found a pair (labelled "lesser pair" below) but the optimal solution was some other pair (labelled "greater pair" below):

```
        lesser pair
     _________________
    |                 |
a b c d e f g h i j k l m n o p q
        |_________________|
       greater pair - optimal
```

Since the solution we found was "lesser pair", at some point our left index advanced to `c`.  If, at this point, the right index was at or to the right of `n`, it would have been decremented until it reached `n` (since the left pointer could no longer move).  Let's now examine the situation that arises at this point:

```
    |———————————C—————————|
    |—————————L———————|   |
    |                 |   |
a b c d e f g h i j k l m n o p q
        |                 |
        |————————G*———————|
```

C is the current pair, G* is the optimal pair, and L is the suboptimal pair we end up selecting.  Let's call the target sum T.  This gives us the following inequalities:
 - T < C, because we chose to decrement at C
 - L < C < G*, due to index positions (and sorted order of array)

Combining these together gives an immediate contradiction: T < C < G*, so C is a better solution than G*.  This is impossible, since G* was defined to be optimal.

We're not quite done, remember all this was done under the hypothesis "If, at this point, the right index was at or to the right of `n`".  The more general picture looks like this:

```
  |————————————C——————————————|
  |   |————————L————————|     |
  |   |                 |     |
a b c d e f g h i j k l m n o p q
          |                   |
          |—————————G*————————|
```

Suppose at this point, we decrement the right index, losing the opportunity to ever find G* in the future.  In this more general case, we have the following inequalities:
 - T < C, because we chose to decrement at C
 - C < G*, by index positions and sorted order
 - T < C < G*, combining the above two
 - L < G*, by index positions and sorted order

In this case we don't know the relative ordering of L and T.  If L > T, then we have T < L < G*, which means L is a better solution than G* (contradiction). If L < T, we have L < T < C < G*, giving C as a better solution than G* (contradiction again).  And of course, if L equals T, it's the guaranteed optimal solution, which is once again a contradiction.  So finally, we can say that this situation cannot occur, thus we are guaranteed to find the optimal pair of values with this algorithm.

So we have an O(n) runtime algorithm for finding the solution to the fuzzy 2-sum problem, given a sorted input array.  The space complexity here is O(1).

Inserting this back into the the fuzzy 3-sum problem, we get O(n log(n)) time to sort the input, followed by n * O(n) time for the n iterations of the fuzzy 2-sum solution.  This gives a final time complexity of O(n<sup>2</sup>).  Space complexity is O(n) to store the sorted array, or O(1) if the input array is allowed to be sorted in-place. (Often times we want our inputs to be `const`, so it's important to note that sorting your input should be considered O(n) space in the most general case.  However, sorting is often not a bad thing, in which case that extra space is not required.  A common situation in which sorting could be undesirable is one in which we have pairs, triplets, or structs holding many values inside. You may want to sort by a certain value for your algorithm, but someone else may want to sort by a different variable for their algorithm in a different part of the pipeline.)

---

**Given an array and a max sum, count the number of distinct triplets of elements whose sum is less than or equal to the max sum.**

Again, it seems very similar to the problems we've seen before, but let's outline specifically what we need to be able to do.

We'll start by iterating through pairs of elements A\[i], A\[j] and computing their sum as current_sum.  Then at each step we want to know the number of values in the original array that are less than or equal to target_sum - current_sum.  We would then add this number to a running count.  If A\[i] or A\[j] or both also satisfy the criteria of being within the search range though, they will be counted in the result.  Since our tuples must contain distinct values, we need to check for that case and subtract those out of our result.

So we need to know the number of values that are less than or equal to some given value.  There's a super easy way to achieve this: just make a sorted array!  Binary search can find a value in log(n) time and the index we reached tells us how many values there are less than or equal to that value.

So, to analyze the time complexity, we start by making a sorted array which takes O(n log(n)) time and O(n) space.  We then iterate through pairs O(n<sup>2</sup>) time and at each step in the iteration do an O(log(n)) search.  The only extra information we store during this iteration is the total count, so we use O(1) extra space, and O(n<sup>2</sup> log(n)) time for this iteration.  The final complexity is O(n<sup>2</sup> log(n)) time and O(n) space.

If you are allowed to sort the input data, you could consider it O(1) space.

---

**The gas-up Problem.  A number of cities are arranged on a circular road.  Each city can give you a certain amount of gas, and you have a vehicle with unlimited gas capacity.  An ample city is one from which you could start, and travel all the way around the circle, filling the given amount of gas at each city.  Determine if there exists an ample city, and find it if so.**

The way I approach this problem is a little different than how the book approaches it.  Regardless of whether or not there exists an ample city, we can first think about just simulating driving around the circle, without stopping if our gas goes negative.  (Later we can adjust our starting point to solve the negative gas issue.)  By the time we get back to the city we started at, we can break the result into two cases:
 - Currently we have as much or more gas than we started with.  This means there must exist an ample city.  This also means that there must exist a global minimum of our fuel level (though it may be negative), which occurs exactly at some city X.  Therefore if we choose to start our circuit at city X, we will have 0 fuel at city X before refuelling, and thus our global fuel minimum will be 0 upon reaching city X, and so we have proven we can drive a full circuit starting at city X.
 - After simulating the full circle, we have less fuel than we started with.  Since there is unlimited fuel capacity and we refilled at every city, this means there is less fuel available in the cycle than is required to drive the cycle.  In other words, there does not exist an ample city.

And this is the algorithm, it covers both the initial problem and the variant in the book.  If you don't like the idea of fuel going negative, think of starting with a billion gallons of fuel in your tank or something, and then your initial drive around the circle is an experiment to see if you lose, gain, or maintain fuel on each cycle.

The time complexity is O(n), as we just make iteration around the circle, and the space complexity is O(1) to track the minimum fuel level and city.

---

**Find the largest square under the skyline.**

Note: I actually found the largest rectangle under the skyline problem to be very tricky.  It's not super intuitive (to me at least) that this problem could be solved in linear time.  I may consider doing a write-up on that problem here, as I found the book's explanation somewhat confusing.  That being said, let's move on to the largest square problem.

Assuming we have the algorithm for the largest rectangle, computing the largest square is easy.  If we check each rectangle candidate as in the rectangle algorithm, but now trim it down to the largest inscribed sqaure (given rectangle of side lengths a, b, the largest inscribed square has size min(a, b)<sup>2</sup>), we will have checked every possible largest square candidate, and so are guaranteed to find the solution.

Time and space complexities are equal to that of the largest rectangle algorithm - O(n) for both.

---
