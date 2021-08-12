## Chapter 18 - Graph - Variants Solutions

---

**Given a 2d array of boolean values representing a black and white grid, find the black region that contains the most points.  Regions are defined as groups of like-color elements connected by up/down/left/right adjacency (diagonals do not count).**

Walk through the 2d array until we hit a black square.  Check if it is visited (keep a separate 2d array of visited state for each element in the input 2d array).  If it is visited already, continue to the next square.  If it is not visited, start a BFS/DFS from this node:
 - Initialize `current_region_size = 0`
 - Search for adjacent same-color non-visited nodes from each node
 - Whenever we visit a node, update its visited state inte visited matrix and increment `current_region_size`
 - When search is complete, update a `max_region_size` variable

Since each node has at most 4 potential neighbors, and we only visit each node once, the time complexity is O(n), where n is the number of squares in the 2d input array.  Space complexity is also O(n) to store the visited state for each square.

---

**Design an algorithm that takes a 2d black and white grid and an index pair (i, j), and sets the value specified by (i, j) to black.  It then returns the size of the largest black region *after the pixel (i, j) has been changed to black* (note: book is ambiguous on this).  Optimize runtime for many repeated calls to this algorithm.**

When optimizing for repeated calls to an algorithm, the first thing we should be thinking about is what we can precompute.  In this case, we could precompute all the black regions.  Now how would we store this information?  Well, we could give each region an id, and then we'd just need an id to size hash map.  If we just make our id values 0, 1, 2, ..., then we can use an array instead of a hash map.  Now we just need to store the id of each pixel in the input array (which we do in another 2d array of the same size), and we can look up the region id and region size for any point in O(1) time.

Now let's think about what happens when we flip a white pixel to black:
 - If it ends up conneting two previously separate regions, we will have to merge these regions
 - If it just attaches to some single existing region, it can simply take that region's id and increment that region's count by 1
 - If it has no adjacent regions at all, it will start a new region of size 1

We distinguish these cases by the number of distinct region ids among adjacent black pixels.  The interesting case to look at is the first case: multiple different regions border this pixel, so it will end up joining these regions.  Suppose it's joining regions r1, r2, and r3.  In this case, we will have to update all the region id fields for all pixels in two of these regions to match that of the third.  The best we can do is have the maximum size region swallow up the other two.  So suppose r1 is the smallest region, r2 next, and r3 the biggest region.  Then we would set all points in r1 and r2 to have id r3.  Then we increment the size of r3 by the size of r1 plus the size of r2.  (We don't really need to worry about deleting or zeroing out the sizes of r1 and r2, as no points have id r1 or r2 anymore, we will never look up the size of r1 or r2 again.)

So it seems like there's one thing we're missing in our current implementation: a way to quickly get all the pixel indices that belong to r1 and r2.  That suggests we should store each region as a point set which we can look up by region id somewhere - we could do this in an array of pointers-to-hash-set, where the array indices are the region ids.  A hash map also works, but is unnecessary since we can make the region ids consecutive integers starting at 0.

Let's try to walk through the full algorithm super precisely now.  Let's call the input 2d array `pixel_matrix`, and the point to set black `(i, j)`.
 - Initialize a 2d array `region_id_matrix` of size equal to the input array size.  Start as all zeros.
 - Initialize an empty `std::vector<std::unique_ptr<std::unordered_set<size_t, size_t>>> region_sets_vector`.
 - Initialize `max_region_size = 0`.
 - Precompute all black regions following the algorithm structure of the previous question, updating `max_region_size` as we go.
   - For each new region, its id will equal `region_sets_vector.size()`.  We also create a pointer to an empty region set at the start of each region, and as we go, the coordinates of each new pixel encountered in that region will be inserted into the hash set.  The pointer to this hash set is pushed back to the region_sets_vector at the end of processing this region.
 - Precomputation is done, now we are ready to handle function calls to set `(i, j)` to black and return new largest black region size:
   - If `(i, j)` is already black, do nothing and return `max_region_size`.
   - Set the pixel black in `pixel_matrix`.  Check how many distinct regions border the pixel `(i, j)` (by looking up region id in `region_id_matrix` for each adjacent black pixel).
     - If exactly one, this pixel just joins that region.  Update this point's region in in `region_id_matrix`.  Add this point to the associated region set in `region_sets_vector`.  Update `max_region_size` using the size of this regions set in `region_sets_vector`.
     - If zero, this region becomes a new region.  Create a new region as we did in the precomputation step. 
     - If more than one, merge all but the largest region into the largest region, then add the point to this largest region as done in the "one" case above.  In order to merge region r1 into r2, we do the following:
       - Use `region_sets_vector` to get all the points in r1, update their ids to r2 in `region_id_matrix`.  Move points in `region_sets_vector` from r1 to r2.  Update max_region_size.  Note: this is the only non-trivial operation in terms of time complexity out of all the cases - time complexity here is O(size of all but largest region).
   - Return `max_region_size`.

The details get a little bit complicated, but in terms of computational complexity this algorithm is actually quite fast.  In every case except for the region merge case, everything happens in O(1) time.  For a two-region merge, the time complexity is O(size of smaller region), and for a general region merge it's linear in the size of all but the largest region.  Region merges should happen relatively rarely, especially merging multiple large regions, so this is quite efficient.  In total given m calls to this algorithm, and calling number of pixels in the grid n, this algorithm will process all m calls in O(m + n) time\*.  The space complexity is just O(n).

\*(The exact math on this is a little tricky, but remember that a region can only merge into a bigger region. Then that merge will create a new region of size more than twice as big as the region that just merged into it.  This means the sequence of region sizes for regions that get merged, if we just consder 2-region merges, is going to be bounded from below by the 2<sup>n</sup> sequence.  Remembering that the sum of powers of two from 0 to k is just 2<sup>k+1</sup>, or in other words just two times the last term, this means our region merge complexity is at worst O(n) in the two-merge case.  Similar arguments can be made for the other cases, relying on the more general result that the infinite geometric series always converges when r < 1. In the two-merge case, the analogy would have been that the sum of the infinite geometric series for r = 1/2 is 1.  Everything else besides region merging is just O(1) per call, so we get an additional O(m) added to the time complexity.)

---

**Determine if an undirected graph contains a cycle.**

The book gives a sort of fancy algorithm based on 3-coloring nodes in the tree for their example problem.  I prefer to just think of maintaining a visited set of nodes separately.  And this is the more general solution, as in general we can't just add fields into a graph or modify the nodes as we desire.  So I would do it as follows (this is for an undirected graph):
 - Initialize empty visited set.
 - Iterate through nodes in the graph until you find a new unvisited node.  Start a BFS/DFS from here, marking nodes as visited as you encounter them.  If you ever encounter an already visited node, there is a cycle.
 - Once the search from this node is exhausted, keep iterating trhough nodes in the graph until you find the next unvisited node, and repeat above step.
 - If search finishes without finding any cycles, there is no cycle.

The main difference between cycle detection in directed versus undirected graphs can be demonstrated with a diamond-shaped graph, where edges are directed downwards:
```
   A
  / \
 B   C
  \ /
   D
```

So here we have A --> {B, C} --> D, but there are *not* arrows from D back to B and C.  So if we did a search from A, went to B and marked it as visited, went to D and marked it as visited, then recursed back up our DFS to A again, now taking the C path, marking C as visited, and then encountering D, we would see that D is already visited.  This does not mean there is a cycle in the directed graph case, but it does in the undirected graph case.

So in the case of a directed graph, you actually have to come back to a node on your current "active path" of the DFS in order to have detected a cycle.  Thus, we would need an `active_set` and a `visited_set` to implement this algorithm in the directed graph case.  This is why the book uses three colors for nodes - one represents unvisited, one represents active, and one represents visited (but not active).

Time complexity is O(|V| + |E|) as before, and space complexity is O(|V|) for the DFS stack and the visited set.

---

**Given a connected undirected graph, determine if the graph remains connected if any one edge is removed.**

First, let's think about what it means for the graph to remain connected if any one edge is removed.  This definitely has something to do with cycles, as they allow us to remove an edge but still have another path.  See if you can figure out the characterization exactly.

If every edge of the graph belongs to a cycle, then any edge can be removed and the graph stays connected.  Note: it is not sufficient that every node belong to a cycle, consider two cycles joined by a single edge:
```
   A         E
  / \       / \
 B   C --- F   G
  \ /       \ /
   D         H
```

If edge CF were removed, this graph would become disconnected, even though every vertex clearly belongs to a cycle.

So we want to run our cycle detection algorithm, but with slight modification: 1) our graph is connected, so we only need one starting point, and 2) we don't stop when we find a single cycle.  In fact, when we find a cycle, we want to retrace that cycle and store all the edges in that cycle in some sort of `edges_with_cycle_set`.  Then we continue the DFS as we were doing before.  We only stop when the size `edges_with_cycle_set` reached the number of edges in the input graph, or when we've exhausted our DFS.

Actually, there's another way we can stop early, if we think what our algorithm is actually trying to detect.  It's kind of the opposite of the algorithm above - we want to keep going if we find a cycle, but we want can actually stop immediately if we know for certain there is an edge that doesn't belong to a cycle.  And this happens if our DFS ever reaches a "dead end" (after which it would normally start unwinding backwards and then explore another path).  If this happens, we can return false immediately.

The time complexity is that of DFS, O(|V| + |E|), and space complexity is the O(|E|) again, for the `edges_with_cycle_set` as well as the DFS stack depth.

---

**\["Making wired connections"]  Check if a graph is a bipartite graph.  A bipartite graph is a graph whose vertices can be divided into two disjoint and independent sets U and V such that every edge connects a vertex in U to one in V.**

Note: I have simplified the wording of the problem significantly from that in the book, because I had no idea what they were even asking when I first read the question (I had to read the solution to reverse engineer the solution.)  Also, I realize after writing this up that this is not a variant, but a sample problem.  Oh well, I hope that my explanation is clearer than that in the book.

let's start with some simple examples.  Say I had a linked list graph - a line of connected nodes.  Well, I could just put every even-index node in one set and every odd-index node in the other:
```
0 -- 1 -- 2 -- 3 -- 4 -- 5 -- 6 -- 7 -- 8 -- 9 -- 10 ...

can be redrawn as:

0   2   4   6   8   10
 \ / \ / \ / \ / \ /  \
  1   3   5   7   9     ...

which is clearly a bipartite graph (the two sets are the top and bottom lines)
```

For a binary tree, we could kind of do the same thing: every even-level node goes in one group, and every odd-level node goes in another.

So when we're in this kind of scenario where we keep thinking of examples that work, try to think of the simplest possible counterexample.  Maybe any counterexample works to start, then simplify it if possible.  To construct a counterexample, we can do the following:
```
Start with one node in each set, and connect them:

A
|
B

Now, in order to break the definition of a bipartite graph, we need to drawn an edge between vertices in the same group.
That means we need two vertices in one group, so let's add a C to group A.

A   C
|
B
  
Now let's create our counterexample by connecting A to C:

A — C
|
B

Woops! This is still a bipartite graph, because B and C can belong to one group, and A to the other.  So let's draw another line from B to C:

A — C
 \ /
  B
```

Success! We have constructed the simplest possible non-bipartite graph.  So can we generalize a pattern by examining what specific properties of this example force it to be non-bipartite?  Let's try.

The A-B connection forces A and B to be in different groups.  Since we are only allowed two groups, let's think of this in terms of parity: A must have opposite parity as B.  Now the B-C connection forces B and C to be in different groups, meaning it forces C to have the opposite parity of B, which transitively means C has the same parity as A.  And if there were two more nodes in this connected sequence, D would have the same parity as B and opposite of A, and E would have the same parity as A.  In general, we can see the pattern, if we have a path with an odd number of nodes on it, the last node in the path must have the same parity as the first node in the path.  That means drawing an edge from the first to the last node would immediately make the graph non-bipartite (as it would force the first and last nodes to have opposite parity in order for the graph to be bipartite, leading to a contradiction).

So this is how we deduce that odd-length cycles force a graph to be non-bipartite.  But are there other situations we might be worried about?  Or if a graph contains no odd-length cycles, does that allow us to conclude it's bipartite straight away?  Well, let's once again try to construct a counterexample: a non-bipartite graph, but this time without any odd-length cycles.  We've kind of exhausted all the possibilites for 3-vertex graphs, so let's move on to 4 vertices.

Suppose we have started with a 3-vertex bipartite graph, and we're trying to add a new node (and its edges) to this graph in order to make a non-bipartite graph.  Let's define the vertex sets of this graph to be L and R for left and right.  If we add a vertex and connect it to some node in L, that forces the new vertex to be in R in order to maintain the bipartite property.  Similarly, connecting it to some node in R would force it to be in L.  Therefore, we must connect this new vertex to at least one node in L and at least one node in R to make a non-bipartite graph.

Let's say we connect our new node z to node x in L, and node y in R.  Focus first on the connection to x in L.  Is it possible that we could move x to R to allow node z to be a member of set L, and thus maintain the bipartite property of the graph?  Sure, we could just flip the parity of x and if x connects to any other nodes, flip the parity of all of those nodes, and so on... But now remember that we also connected our new node z to y in R.  That means that if y was one of the nodes in the chain reaction of flips starting from x, it would have gotten flipped to set L, and then z and x are back in the same set.  So the condition for constructing a non-bipartite graph in general is that there exists some node z connected to two nodes x and y, and there exists some chain of connetions from x potentially through other nodes to y.  Now if that chain had an even number of edges, x and y would be in the same set and adding z would be no problem. So the final criterion is that there are an odd number of edges (which means an even number of nodes) on the path from x to y.

You may have realized by now, putting all this together, that having an even-length path from x to y, plus having another node z connected to both x and y, is precisely the characterization of having an odd-length cycle.  Thus we have proven that a graph is non-bipartite exactly when it has at least one odd-length cycle.

* * *

To write this up as an algorithm, first let's split the given graph into its connected components and handle them each seaparately.  If any connected component is non-bipartite, the graph is non-bipartite.  Otherwise, the graph is bipartite.

On each connected subgraph, start a BFS or DFS search anywhere, assigning parity (representing L or R set) to the nodes as you go (opposite of their previous neighbor).  Keep track of visited nodes in a visited set.  If at any point in the search we reach a previously visited node, check if its parity (L/R set) is compatible with the current node.  If not, return false.  If the search completes without returning false, return true.

So it turns out actually writing the algorithm was much easier than the mathematical proof that a graph is bipartite if and only if it has an odd-length cycle.  In fact, the odd-length cycle fact never even came up in our algorithm, which is probably why the book made such a big deal about odd-length cycles in its *Hint*.  The proof is quite cool and instructive though, so it was good to go through.

---

**Compute a shortest addition chain exponentiation sequence to evaluate x<sup>n</sup>.**

Or, to simplify out the exponentiation part of the question, we need the shortest integer sequence that reaches *n* starting from 1, such that each value in the sequence is the sum of two previous values (not necessarily distinct, i.e. twice a previous value is allowed as well).  The example in the book gives the sequence \[1, 2, 3, 5, 10, 15].

TODO: I've noticed that looking for multiples of 3 tends to be a good strategy here.  Investigate if there's some tricks based on that which can work here.  (For example: 15, 27, 30, all work better by splitting into 2/3 + 1/3 than using powers of 2.  Even 31 works better as 30 + 1 and splitting the 30 into thirds.)

---
