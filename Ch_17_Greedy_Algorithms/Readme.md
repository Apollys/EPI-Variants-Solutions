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

