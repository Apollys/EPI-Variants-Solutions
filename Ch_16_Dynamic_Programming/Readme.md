## Chapter 16 - Dynamic Programming - Variants Solutions

---

**Given an array of possible point values that can be earned in a single play, and a target score *s*, compute the number of possible score combinations that yield *s* total score. Use O(s) space.**

Let's review the solution without worrying about the O(s) space first, since the book's explanation is very unfocused and long-winded.

We start by sorting the scores, because earning score *a* followed by score *b* is the same combination as earning score *b* followed by score *a*.  Decreasing order seems the most natural to me, because once your current sum total is too big to add another score of value 5 for example, you definitely wouldn't be able to add any other values larger than 5.  However, either sorting order is fine, it won't affect overall time complexity.

Now that we have our sorted scores, we build up a DP table tracking the number of score combinations that yield each point value from 0 to s.  Each row of the DP table adds one additional option to our possible plays set, starting from the empty set as the top row.  Our initialization is that the left column is all 1s (there is exactly 1 way to make 0), everything else is zero.  We iterate through each play_score in sorted scores which corresponds to iterating through rows in our DP table. We then walk through the columns, computing the number of combinations here as the sum of combinations including this play_score (the value in current row, left play_score columns) and excluding this play_score (the value in previous row directly above the current cell).

Just writing it like this, it should be pretty clear that the space requirement of this algorithm is simply two rows of our DP table: we never access anything other than the previous row and the current row.  Whenever we initialize a new current row, remember that it should start with the values \[1, 0, 0, 0, ...\], since we need the base case: "there is one way to make zero".

As a nice little trick, at the end walking through each row we can use `previous_row = std::move(current_row)` in C++ to avoid any excess data copying.

Of course, the size of previous_row and current_row are O(s) and so we have reached the goal of using O(s) space.

---

**Compute the number of score *sequences* that achieve the target score *s*.  Same problem as above but now \[1, 2\] and \[2, 1\] would could as two distinct ways of achieving 3 points.**

My first instict on seeing this problem is that if I knew the actual score combinations in the above solution (rather than just the count) I could compute this answer directly.  For example, if I knew the score combinations to make 9 are either {3 3s} or {3 2s, 1 3}, this problem is then reduced to the math problem we usually see in high school of the form "How many ways are there to arrange the letters in *mississippi* into unique words?"  And the answer here lies in the observation that if we first count the number of total permutations, then look at *s* (ignore all other letters for now). Since *s* occurs 4 times in the word, we are counting each unique 4! times rather than just once (all possible permutations of 4 s's in any given unique word).  So we divide our total count by 4!, and then move on to do this for each distinct letter in the word.

Let's think about the complexity of this algorithm.  Now we have to store combinations instead of counts in our DP table - a combination has size O(num_play_scores) since it represents the count of each possible play_score.  Unfortunately, we can't just store one combination for each total score, we have to store all possible combinations.  So the space complexity will be O(num_combinations * num_play_scores * s).  This will also bump up our time complexity to that same expression.

So we've found a solution, but increased our time complexity by a factor of num_combinations and increased our space complexity by an even larger factor.  Is this justifiable? Is there a better solution?

I think some degree of space complexity increase is intuitively justifiable - it seems that no matter what our algorithm is, it will need to store some information about the score combinations.  If I just tell you, there are 10 unique sequences to make a score of x using {2, 3}, now if you are allowed to use 5s as well, how many sequences will there be to make the score x + 5?  You can't answer that because it depends how long those sequences are.  If a sequence has length 2 for example, you could add a 5 at the beginning, in the middle of the two values, or at the end, which would give 3 new unique sequences.  However, given a sequence of length 20, you could add the 5 at any of 21 different locations, creating 21 new unique sequences that sum to x+5.

This might suggest to us a slight optimization.  It looks like we don't need to keep track of the combinations themselves, just the lengths are sufficient.  A little extra detail will need to be handled though, because when multiple of the same value occur, the redundant permutations need to be eliminated.  That means really we need a count of instances of current play_score for each score combination in current_row, as well as the length of each as we said before.  This is still just O(num_combinations) for each cell in our DP table, so we will get an optimization out of this.  The DP table has O(s) width and we only need two rows at any given time, so our space complexity is O(s * num_combinations).  The time complexity will remain unchanged, because we still need to process each of the  num_combinations combinations for each row and column in our DP table.

For now, I'm going to say this seems like a decent solution.  I'm not sure if it's optimal; perhaps thinking about the problem from another point of view could yield some clever insight.  However, for now it really seems like we need to track additional information to go from combinations to sequences, and so the time and space compleixty increases by a factor of num_combinations each seems quite justifiable.

---

**Given the scores for two teams, compute the number of distinct scoring sequences that result in the given target scores.**

Using the solution to the previous problem, we can compute the number of unique sequences that generate a target score, and the lengths of each sequence (which we computed as an intermediate result in our DP table) for each team separately.  This gives us two sets of sequences, S1 = {sequences summing to team 1's target score} and S2 = {sequences summing to team 2's target score}.

The problem is now reduced to computing the number of ways each pair of sequences in S1, S2 can be uniquely interleaved, and then summing the total result.  Note that we don't have to worry about different pairs of sequences somehow generating the same interleaving (which is what allows us to simply compute the total sum as the final result).  Note also this is not a standard sequence interleaving problem, because the elements from the two sequences are always considered distinct from each other (for example, Team 1 scores 2 is always different from Team 2 scores 2), so there is less work to be done in terms of tracking duplicates.

In fact, we don't care about the sequences themselves at all, only the lengths of each.  Thus, we just need to do a one-time computation to answer the question: "how many interleavings are there of sequences of lengths up to L1, L2?" for L1 = max length of team 1's sequences, L2 = max length of team 2's sequences.  This suggests using a DP table on lengths for team 1 and team 2's sequences.  The update step in the algorithm will be: either you end with the last value in sequence_1 or the last value in sequence_2.  In the first case, there are DP\[i-1, j\] options, and in the second case there are \[i, j-1] options, and we just add these two results together.

We may be tempted to optimize the space complexity here by just saving two rows/columns to get O(min(L1, L2)) space complexity, but remember we want the intermediate results as well.  So if we did this, we would need to make sure to store the intermediate results we will actually need later in a hash table.  For now, let's assume this optimization is unnecessary, but we can keep it in mind if we need to reduce space complexity later.

Let's analyze the total runtime complexity and space complexity.  First, we start with computing the lengths of all the sequences for each team's target score: this takes O(num_combinations_1 * target_score_1 + num_combinations_2 * target_score_2) space, and that expression times num_play_scores time complexity.  Next we compute the DP table for the interleavings problem, which takes O(max_sequence_length) = O(max(target_score_1, target_score_2)) time and space.  Finally, for each of the score combination pairs between team 1 and team 2, we look up the number of interleavings in our DP table and add the result to a running total.  This will take no more time complexity than anything we've done up to this point, as we are just looking up values in our DP table.  And this final step's space complexity is O(1) of course.

So the final time and space complexities equal that of the previous algorithm, and we can be quite certain these are optimal.

---

**Compute the maximum number of times the lead could have changed, given final scores for each team as above.**

It seems this is just a direct reduction to the previous problem.  If we knew the possible score sequences, we could just check each one for the number of times the lead changed.

However, is there any way to compute the sequence of lead changes directly without computing all sequences?  The problem is, if you start to try to generate sequences, you will very likely not end up with the target total score.  In order to ensure you do hit the target total score, you need the sequences (or at least the counts of each play_score that you have to work with), which brings us right back to the previous solution.

So how do we best adapt the previous solution to work here?  We need to save not just the lengths of each of the num_combinations score combinations that lead to the target score (per team), but now the actual counts of each value, as we did in the preliminary solution to the initial problem.  This will add a factor of O(num_play_scores) to our space complexity, which seems completely justifiable.  Now we want to compute the number of lead changes that occur for each possible sequence pairing.  This means we can use a similar DP table as before, but it's going to need to track number of lead changes rather than number of interleavings, which means it will need to be reset and recomputed for each new sequence pair.  So the time complixty for this step will go up to O(num_sequences_1 * num_sequences_2 * max_length_sequence_1, max_length_sequence_2).  This is just the time complexity of iterating through all pairs of sequences, so this is the best we can do for this type of algorithm.  (To improve, we would need a way to generate the solution sequence directly, or a small subset of candidate solution sequences.)

---

**Compute the Levenshtein distance using O(min(a, b)) space and O(ab) time.**

The standard DP algorithm given in the book for computing Levenshtein distance only uses the previous and current row and any given time, and of course we can pick the rows to correspond to the shorter of the two words since everything is symmetrical, so the space complexity is in fact O(min(a, b)) as required.

---

**Find a longest common subsequence given two sequences of characters.**

This is very similar to computing the Levenshtein distance, in fact it is slightly simpler to do the update logic.  Let's consider two words "aXbXaXc" and "XbXcXaXc", where X's represent random distinct unmatching characters (an X in one word does *not* match an X in the other word).  Our initial table will look like this:

```
  | _  a  X  b  X  a  X  c
--|------------------------
_ | 0  0  0  0  0  0  0  0
X | 0
b | 0
X | 0
c | 0
X | 0
a | 0
X | 0
c | 0
```

This represents the fact that the longest common subsequence between two empty strings has length zero.  From here, we consider adding one more character onto one or both of the words, and seeing how the information we've computed so far in our DP table can give us the longest common subsequence length of the new pair of words:
 1) If the last character of both of the words are equal, the LCS (longest common subsequence) is the LCS of the two words without the last character, plus the last character.  This means the length would be the length in the square up-left in the DP table plus 1.
 2) If the last characters are not equal, we can use the results we already know for the LCS of each word and the other word excluding its last character.  This gives us two options, the square upwards and the square leftwards in the DP table.  Note that in this case we do not add 1, we are simply using that exact same subsequence.
 3) To combine all the options, we pick the best (max length) of the three, and store than in the current spot in the DP table.

Filling out the table, you should get the following result.  I added an asterisk where case 1 above occurs:

```
  | _  a  X  b  X  a  X  c
--|------------------------
_ | 0  0  0  0  0  0  0  0
X | 0  0  0  0  0  0  0  0
b | 0  0  0  1* 1  1  1  1
X | 0  0  0  1  1  1  1  1
c | 0  0  0  1  1  1  1  2*
X | 0  0  0  1  1  1  1  2
a | 0  1* 1  1  1  2* 2  2
X | 0  1  1  1  1  2  2  2
c | 0  1  1  1  1  2  2  3*
```

So the longest common subsequence between these two words has length 3.  But how do we retrieve the subsequence itself?  Well that's why I've added asterisks to the case 1 elements: these indicate the positions in which a character was added.  In other words, following the path backwards and writing down the characters that correspond to each asterisk will give us the LCS (in reverse).  Starting at the 3, we write down 'c', and move up-left.  From there, we either move up or left (since they tie in length), and then at the next stop move up or left, whichever we didn't before (because the other option gives us a shorter length).  At this 2* element, we add 'a' to our running sequence and move up-left.  From here, we move some combination of 3 steps up and 1 step left to arrive at the 1* element.  At this point, we append 'b' to our running sequence, move up-left to 0, and terminate (since we're at 0).

The sequence we've written down is "cab", reversing it gives "bac", which we return as the longest common subsequence of "aXbXaXc" and "XbXcXaXc".

Notice that we actually used the whole table at the end to reverse engineer the LCS from the LCS lengths.  This means if we only save the current and previous rows of the table, we're going to have to store the actual subsequence rather than just the length. Since the subsequence has length O(max(length_1, length_2)), this doesn't end up saving us any space and just complicates things.

So our final time and space complexities are both O(length_1 * length_2).

---

**Compute the minimum number of characters to delete from a given string that would make it a palindrome.**

This can be reduced to finding the length of the longest common subsequence between the given string and its reversed string.  Once we have the length of the LCS, the number of characters to be deleted is the length of the input string minus the length of the LCS.

Now we don't need to actually track the LCS, only its length, so we can reduce the space complexity to O(n). Time complexity is O(n<sup>2</sup>) still.

---

**Given a string and a regular expression, what is the string in the langauge defined by the regular expression that is nearest to the given string?  Nearest is defined as minimum Levenshtein distance.**

This is very similar in nature to the basic Levenshtein distance DP algorithm given in the book.  However, you need to cover all the expression type cases that a regular expression can cover, such as character ranges/sets (for example: \[a-ZA-Z]), wildcard characters ("." matches any character), and repetitions (usually represented by "\*").  Ironing out all the details can be a bit tricky and laborious: I recommend walking through a simple example such as: `word = "Abc01", regex = "[a-z]*[0-9]+`.  Note: I would start by converting all `+` items to `*` items, so `regex = "[a-z]*[0-9][0-9]*"` to give yourself one less complexity to deal with.  Each row of your DP table will correspond to adding a new letter from the input word, so there will be length(word) + 1 = 6 rows (the first row corresponds to the empty string).  Each column of your DP table corresponds to adding the next "expression unit" from your regex, so there will be 3 + 1 = 4 columns (first column corresponds to empty regex, regex units are: `[a-z]*, [0-9], [0-9]*`.

Fill out the table, and as you go track whether you were adding a character, deleting a character, or changing a character at each spot.  Once you fill out the table, the bottom right element should have Levenshtein distance value 1.  Tracing back through the table, you should get the following path: `'1' matches with [0-9]*, '0' matches with [0-9], 'c' matches with [a-z]*, 'b' matches with [a-z]*, 'A' was a mismatch so it needs to be deleted`.  Reversing this sequence and applying insertion/deletion operations where needed, we get the string "bc01".

So I've roughly explained how to walk through the algorithm by hand.  As for implementing it in code, there are a lot of little checks and cases that need to be covered, so I'm not sure how valuable it would be unless you really want to.  For this one, after walking through the very simple example above, I would suggest writing out a slightly more complicated example and walking through that one by hand.  After that, you should be feeling pretty comfortable with how this algorithm works.  If you want to implement it in code afterwards, you can, but if you feel your time is more valuably spent working on other problems, that would be completely logical as well.

---

**Given two strings s1 and s2, and a target string t, determine if the characters of s1 and s2 can be interleaved to obtain the target string t.**

First note, though it won't affect the worst-case time complexity, if the lengths of s1 and s2 don't sum to equal the length of the target string, we can return false immediately.

Once again, we'll implement a dynamic programming table very similar to what we've done in the previous problems.  The rows will correspond to one input string, and the columns to the other.  Let's use the example they give in the book, s1 = "gtaa", s2 = "atc", and the target sequence is "gtataa".  (If you're wondering why they're using these weird random letters, it's because ACGT are the letters that encode DNA sequences, and a lot of sequence matching algorithms have historic roots in DNA sequence algorithms.)

Our starting DP table setup will look like this:

```
  | _ g t a a
--|-----------
_ | *
a | 
t | 
c | 

target_sequence = "gtataac"
```

The `*` indicates where we "are" currently, which is the top-left corner at the start.  From there, we move either right or down in the table to the character that matches the next character in our target sequence.  For this example, this yields:

```
  | _ g t a a
--|-----------
_ | > > v
a |     v
t |     > > v
c |         *

target_sequence = "gtataac"
```

Since we reached the bottom right corner, we were able to create an interleaving of the target sequence and we can return true.  If instead we get stuck (cannot move right or down) and thus cannot reach the bottom right, we return false.

There's one detail that this example oversimplifies.  It may seem from this case that we don't even need the DP table, we could just track two numbers representing our current position in each string, and thus walk through this table without actually using more than O(1) memory.  Is there a problem with this?

Hint: consider the case `s1 = ab, s2 = abxy, target_sequence = abxyab`.

```
  | _ a b                       | _ a b 
--|-------                    --|------- 
_ | > > X                     _ | v 
a |                           a | v
b |                           b | v
x |                           x | v
y |                           y | > > *

target_sequence = "abxyab"
```

As we see on the left, if we start by matching "ab" from s1, we will get stuck at the position of the character "x" in the target sequence: our only option is "a" but these characters don't match.  However, as shown on the right, there would have been a solution if we started by matching "ab" from s2 instead.

This means it's not quite as straightforward as the original example made it seem.  Our choices now affect the options that we will have available for us in the future, and thus we do need to keep track of the information stored in the DP table.  Of course, we are only ever using the values directly up and directly left of a given square to compute the value in that square, so we can reduce the space complexity to O(min(s1_length, s2_length)).  Note that we don't actually need to track the index into the target sequence for each cell in the DP table, because its indices tell us how many characters have been used so far, and thus where we are in the target sequence.  Specifically, the box and index i, j in the DP table corresponds to matching character i+j in the target sequence.

Time complexity is naturally O(s1_length * s2_length).

---

**Compute the number of paths from the top-left to the bottom right of an m x n array.  Use O(min(m, n)) space.**

As were quite familiar with by now, since we're only using data in current and previous row at any given time, and since the algorithm is symmetrical with respect to m and n, we acheieve space complexity O(min(m, n)) by only keeping current and previous row in memory (and swapping rows/columns first if rows are bigger than columns).

---

**Solve the same problem as above in the presence of obstacles, specified by a 2D boolean array where true represents the presence of an obstacle.**

Same as above but if a square has an obstacle we set its num_paths to 0 rather than the sum of up and left num_paths.

Space complexity is still O(min(m, n)), and time complexity is O(mn).

---

**A fisherman is in a rectangular sea.  Given the value of the fish at each point (i, j) in the sea, compute the maximum value of fish a fisherman can catch on a path from top-left to bottom-right square.  The fisherman can only move down or right.**

Same DP idea as before, but now we're tracking fish value instead of number of ways.  The update step will be `max_value[i, j] = fish_value[i, j] + max(max_value[i-1, j], max_value[i, j-1])`.

Again, O(mn) time and O(min(m, n)) space.

---

**Solve the same problem as above when the fisherman can begin and end at any point.  Fish values may be negative.**

First note: if fish values couldn't be negative, the optimal solution would just be the solution to the previous problem.

With this insight in mind, the only extra case we have to handle is fish values being negative.  When would this give us a worse solution than starting at the top-left and ending at the bottom-right?

It would give us a worse solution if at some point the fish value at our current square was more negative than the total positive fish values accumulated so far, so our cumulative fish value becomes negative.  At this point, we would be better off not using this path, but starting at the next square instead.  So our update will be `max_value[i, j] = fish_value[i, j] + max(0, max_value[i-1, j], max_value[i, j-1])`, where we've added in a max(0, ...) to just say, if all our options are negative, start a new path here instead.

The final answer will be the largest value in our `max_value` DP array, which we can compute as we fill it (so we cana void storing the full m x n array in memory).

Still O(mn) time and O(min(m, n)) space.

---

**Call a decimal number monotone if the digits are (non-strictly) increasing as you read from left to right. Given a positive integer k, compute the number of length-k decimal numbers that are monotone.**

Construct a DP table where rows correspond to length and columns to the final digit (from 0 to 9).  The initial DP table will look like this:

```
             Last Digit
     | 0  1  2  3  4  5  6  7  8  9
-----|------------------------------
   1 | 1  1  1  1  1  1  1  1  1  1
L  2 | 
e  3 | 
n  4 | 
g  5 |
t  6 | 
h  7 |
   8 | 
   ...
```

Note the initial row in the DP table is saying that there is exactly one decimal sequence of length one ending with each possible digit.

We can then fill in the DP table by computing cumulative sums across the rows.  I.e. the number of decimal digits of length 2 that end in 7 equals the sum total number of length-1 decimal digits that end in any digit up to 7 (for each of those, we simply append a 7 to the end).

The time complexity will be O(10 * length) = O(k) and since we only ever use the current and previous row at a given time, the space complexity is O(2 * 10) = constant space.

---

**Compute the number of strictly monotone decimal numbers of length k for a given k.**

Same as above but our cumulative sum up to digit 7 would tell us how many numbers end in 8 (i.e. 7+1) instead.  Everything else is identical.

---

**\[Example problem, not variant] Given a 2D array of integers and an integer sequence, determine if the integer sequence can be found in the 2D array.  You may start anywhere in the grid and must travel to an adjacent (non-diagonal) entry at each step.**

I am including this in here because the book's solution doesn't really make sense - it's overly complilcated and not optimal in terms of space complexity.  (In my version of the book, their solution has space complexity O(mnl), time complexity O(mnl) and uses a complicated triplet hash map for caching results.  Perhaps since I bought the book they've updated it.)

Suppose we were searching for the sequence `[1, 2, 3, 8, 9, 7, 9]` in the following grid:
```
1 2 3
2 9 4
3 8 9
```

The first thing we might notice is that the grid contains no 7s, so we know the answer is false right away.  This gives us a nice early out, but of course doesn't reduce worst-case complexity.  Let's change the `7` in the sequence to a `1` so we don't have that early out.  Now our sequence is: `[1, 2, 3, 8, 9, 1, 9]`.

We start by looking for any 1s.  Any square with a 1 will be a potential starting point.  There is exactly one square - the top left corner - so this is our one starting point.  The next value we need to match is a 2, so we look at all of the neighbors of the 1 at the top left to search for 2s.  Both the squares to the right and below match the value 2, so our set of current candidate squares are those two 2s.  Now, for eacah of those 2s, we look for a neighboring 3, of which there are two in total.  We continue in this way matching one 8, two 9s, and then... zero 1s so all of our candidate solutions have failed and we return false.

Hopefully it's clear walking through this example that all we're doing is a breadth-first search.  I have no idea why the book brought in this suffix caching for mismatched values and all sorts of extraneous code that just makes the algorithm less efficient.

For our BFS, the space complexity is just the maximum size of the current frontier set (also called fringe set, or active node set), which is potentially up to O(mn) if the grid dimensions are m x n.  The time complexity is O(mnl) in the worst case, if the fringe set grows up to size O(mn) and stays that large for O(l) steps.  So we saved a factor of l on space complexity over the book solution, and got the same time complexity.

---

**Solve the same problem as above, with the added restriction that each element in the grid may only be used at most once in the sequence.**

So sometimes adding restraints to problem makes the problem more challenging because you have to think of clever ways to overcome those restrictions.  And other times it just removes the space for cleverness and there's nothing better to do than the basic, seemingly inefficient thing.  I think this particular question is a case of the latter.

We need to track what's been visited for each partial sequence in the array, and for each of them it's going to be different.  This means we could try a BFS solution like before but each BFS frontier node would also have to track the visited state of the grid, and furthermore, we would end up with very few repeated subproblems, so even the idea of a BFS doesn't seem very efficient.  (An example of a repeated subproblem would be a 3x3 grid in which you could either snake columnwise or rowwise to the bottom right corner.  If you try to draw out more examples of cases that could lead to repeated subproblems, you will find they are very sparse.  In fact, for 3x3 grids I think there's only one other, plus its reflection.)

So we might as well do DFS to save memory, since repeated subproblems will be sparse.  We just do a basic DFS, tracking visited nodes as we go, and resetting nodes to unvisited as we unwind the call stack.  Space will be O(mn) for visited status plus O(l) for function call stack (if implemented recursively) or some data structure to track the current active path (if implemented iteratively).  The time complexity will be a little trickier to compute: in general it shouldn't be very large, but we can think of some pathological examples that make it a very challenging problem.  For example, suppose the grid is just a massive grid of zeros, but with the values 1 and 2 sprinkled extremely sparsely here and there.  And suppose our target sequence is a bunch of zeros followed by a 1 followed by a bunch of zeros then a 2 followed by a bunch more zeros.  No matter where we start or what path choices we make, we're going to almost always match everything except at the points where we have a 1 and a 2 in our sequence.  (The reason I included 1 and 2 was so we weren't tempted to think we could start at the 1 and work our way outward, which we could do efficiently f the sequence were `[... 0, 0, 0, 1, 0, 0, 0,...]`.)

So at each step we could have up to 4 valid neighbords, and we try up to l steps (where l is the length of the target sequence).  Also, we have a total of *mn* possible starting points, so this yields a loose upper bound on the runtime complexity of O(mn * 4<sup>l</sup>).  I definitely wouldn't be surprised if there's a sharper upper bound we could derive for this question, but I think that's not really the most important thing to worry about anyway.  The performance of this algorithm is so extremely dependent on input cases, that in practice you would want to analyze your input data and work on tailoring the algorithm to those scenarios.

There is an effective initial early-out check that we can do for this problem, that we haven't mentioned yet.  Its utility will of course depend on the types of input cases the algorithm receives, as discussed above.  Suppose our sequence was `123242` but our grid only had two 2s.  We know right away the answer is false - this sequence can't possibly be in the grid since we can't re-visit visited squares.  So we return false immediately.  This check is cheap, so there's no reason not to implement it on top of whatever algorithm we employ.

---

**Enumerate all solutions to the same problem as above.  (Find the sequence in the grid, you cannot use an element in the grid more than once.)**

Oh boy, another "find all the answers" question!  This means no room for cleverness, just perform a brute force trial and error, and record every success.  Imagine for example the grid were just a giant grid of all zeros, and the sequence a length mn/10 sequence of all zeros.  The solutions are just going to be every path we can legally draw through this grid, and so there's really no optimizing this out.  The easiest way to implement this would be to re-use the previous solution, but just not stop on success (instead record the result and continue).

At the very least, we do still have our early out if there are no solutions because the grid doesn't contain enough of some value.  But in terms of worst case complexity, this is just going to be brute force: time and space complexity will equal the final solution size.  As we reasoned about in the previous problem above, we can deduce a loose upper bound for this at O(mn * 4<sup>l</sup>).

---

**Knapsack Problem - O(w) space.**

As we've seen many times by now, the DP algorithm outlined in the book only uses two rows at a time, so the required space complexity is O(row_size) = O(w).

---

**Knapsack problem - optimal space complexity.**

So if you've gone through the knapsack problem and understand the solution as presented in the book, at some point you probably should have been very worried that it seems to be potentially *worse* than the brute force algorithm - enumerate all item combinations and check their value.  For example, suppose I had only three items with weights are 400, 600, and 900, and values 2, 3, and 4 respetively, and suppose my knapsack capacity is 1000.  This algorithm as presented will compute 3000 values in a DP table just to tell me to grab the first two items, which I can see plainly with just a couple calculations in my head.

So if this isn't something you've though about yet, take a moment to think about it now.  How can such a famous computer science problem with a smarty-pants DP algorithm seem to give us a solution that's absurdly worse than the brute force algorithm?  Is there a way we can rectify this?

...

Think about trying to reduce the weight values that we compute.  Rather than having a table with 1000 columns, is there a way we could have a table with just a few columns?

If you think about it, all we need are the weights that items could actually sum up to.  There's no reason to consider 400, 401, 402, and 403 separately, for instance, because we can clearly see that the solutions for all of those weights will be the same.  There's no combination of items that sums to 401, 402, or 403.  So we start by just creating a list of possible item weight sums up to our knapsack capacity.  We can start by sorting all the weights to give us a nice early out from this subalgorithm.  Ideally, we would like to generate the list of sums in sorted order, but from my own experimentation that seems pretty hard to do, so I think it's reasonable to just generate the list first and then sort it after.

Once we have the list of the possible weight sums of items, these become our column labels in our DP table.  Now the number of columns equals the number of distinct weight sums, and the rest of our algorithm will be the same as before, so the final space complexity is O(num_weight_sums) and the time complexity is O(num_item_combinations + num_weight_sums * num_items), where the first term indicates the number of item combinations that yield sums less than or equal to the knapsack capacity (even though our DP table doesn't have separate columns for them, we still had to check them in the first place).  So finally, we have an algorithm whose worst-case time complexity is not beaten by brute force (note: it's still not better either).  For cases in which many combinations of items will sum to the same weight sums though, we get an algorithm that's much more efficient than brute force.

---

**Solve the fractional knapsack problem - the theif can take any fraction of an item he wants, and it will have that same fraction of its original value.**

Now the greedy algorithm is optimal.  We just sort items by value/weight, and start by taking as much as we can of the most valuable per unit weight item, then if we have space left take as much as we can of the next item, and so on.

Time complexity is O(n log(n)) to sort, space complexity is O(n) to store the sorted list.

---

**Divide the spoils faily.  Two thieves have stolen items and wish to split them into two groups with minimum difference in value of each group.**

We can rephrase this as "select a subgroup of items that maximizes their sum values, given their value must be less than or equal to total_value / 2".  This is just a version of the knapsack problem, in which values in this problem correspond to both weights and values in the original knapsack problem (since the values here are both the constraint and the quantity to be maximized).

You could also solve it directly by generating the sum combinations of values (as we discussed for weights in the knapsack problem above) and tracking the largest value_sum found.  This gives O(num_item_combinations) time and O(n) space (to store which items are used in the optimal solution).

(A little more precisely on the time complexity: if we sort first by value, we can then get an early out once item combinations sum to a value greater than half the total value.  This gives time complexity O(n log(n) + num_item_combinations_up_to_half_value).)

---

**Solve the above problem, but with the additional constraint that the thieves must have the same number of items.**

As above, we can just generate the item combinations directly.  This time, we can only count a combination as a potention solution when the number of items equals n/2, at which point we stop and generate the next item combination.

Same time and space complexity as above.  You could also limit the time complexity by O(n choose n/2) if you prefer to write it that way - it will be a little looser of a bound most of the time, but also more general.

---

**Determine if a tie is possible in the US presidential election between two candidates.**

This is the divide the spoils fairly problem, but we're looking for the solution where the "value" of each of the two subgroups is exactly half the total value.

Same time and space complexities as the divide the spoils problem.

---

**Minimum palindromic decomposition - given a string, break it into the concatenation of a sequence of substrings, such that each substring is a palindrome and the total number of substrings is minimized.**

We're going to use a dynamic programming approach that's very similar to that used in the previous problem.  First let's not that in the previous problem the book used prefix strings as keys into a hash map to store intermediate results, but this is completely unnecessary.  We could just use a single integer corresponding to the index.  Furthermore, since this integer is an index, we can use a vector - there's no need for a hash map at all.  So we will keep this in mind as we tackle this problem to make the solution simpler.

If we start thinking down the route of computing the result up to some index *i*, and then at each step extending that *i* by 1, we may run into what seems to be a snag.  Given that `s[0, i]` is a palindrome, there's no easy way to determine if `s[0, i+1]` is a palindrome.  Incrementally increasing word sizes doesn't work well with palindromes.  So we need to bust out a classic old trick that should always be on your mind when you hear the word palindrome, it's a trick that stems from the question: how do you compute the longest palindrome that is a substring of a given string?

The trick is to use a middle-out algorithm, as it mimics the structure of the palindrome property.  For each possible center point, we move outwards in both directions together, checking that the new left and right characters match, and as soon as there's a mismatch we get an early out and move on to the next center point.  This yields an O(n<sup>2</sup>) algorithm with lots of early-outs in most cases.  (Contrast this to the naive approach, which would be for each start and endpoint, check if it's a palindrome, yielding O(n<sup>3</sup>) time complexity with zero early-out opportunities.)

So we can do a one-time O(n<sup>2</sup>) precomputation that covers all possible palindrome substrings of the given string.  Specifically, we could store an array that tells us the length of the longest palindrome starting at each possible index.  Armed with this array, our DP algorithm just needs to fill out a 1-D DP array tracking the number of strings in the minimum palindromic decomposition up to the current index.  At each index i, we look up the longest palindrome that starts at i+1, and in the dp array at the index corresponding to the end of this palindrome we store the current value plus 1: `dp[i + length] = dp[i] + 1`.  Once i reaches the end of the input string, we're done.

The time complexity is O(n<sup>2</sup>) for precomputation and O(n) for the dynamic programming algorithm, so O(n<sup>2</sup>) in total. The space complexity is O(n) to store the length of longest palindrome at each step, and O(n) to store the DP array, so O(n) in total.

Now you might be thinking, wait we're not done!  We only saved the number of strings in the minimuim palindromic decomposition, but didn't get the decompostion itself!  Can you think of a very minor adjustment to our algorithm which allows us to recover the decomposition without increasing time or space complexity?

All you need to do is track the index that you came from at each update step in the dp array.  So for example, when you do `dp[i + length] = dp[i] + 1`, also add a line `prev_index[i + length] = i`.  Now you can recover the sequence of substrings by retracing your steps backwards from the end.  Note that it was important that we saved just the previous index, not the whole substring s\[i+1 : i+length].  You might have thought the latter would be more convenient, but that turns an O(1) operation into an O(n) operation (copying a whole string), which is inside an O(n) loop already, and so would increase the time complexity of this section of our algorithm from O(n) to O(n<sup>2</sup>).  In general this is something to be cautious of, though in this particular case we already did an O(n<sup>2</sup>) precomputation, so it wouldn't have increased the runtime complexity (but still could definitely have an impact ont he algorithm performace in practcie).

---

**Pretty printing - given a sequence of words and a maximum line length, determine where to place line breaks to minimize the sum of squares of number of blank spaces at the end of all lines except the last line.**

Note: I am not sure the explanation for the DP algorithm given to the previous problem in the book makes sense.  They say "if in the optimal placement for the ith word, the last line consists of words j, j+1, ..., i, then the first j-1 words must be placed optimally" (this seems unclearly worded, I'm not sure exactly what they mean even).  However if we look at their example, all we need to do is make the next words "g" and "h", and then the "c" and "d" from the second line would have to bump back up to the first line to give an optimal solution.  TODO: check their code and see if it handles it correctly. Maybe their explanation is just bad, but the code words properly.

* * *

Your instinct might be now that we don't care about the last line anymore, a greedier approach should be the way to go.  Let's try to think about this in more formal logic.

Suppose we have 3 lines of text when we put as many words on each line as possible.  The last line doesn't count towards our cost function (the sum of squares of blank spaces at the end of all but the last line), so our "messiness" or cost equals b_0<sup>2</sup> + b_1<sup>2</sup> (where b_i = number of blanks at the end of line i).  What would happen if we moved a word from the end of line 0 to the beginning of line 1?

Let's start with the easy case: suppose line 1 can still fit all the words it fit before, plus the new word.  So now our cost function becomes: (b_0 + l)<sup>2</sup> + (b_1 - l)<sup>2</sup> (where l is the length of the word we move plus 1).  It looks like depending on the b_i and l values, this could actually be better.  For example, suppose line 0 was originally completely full, and line 1 had four blank spaces at the end.  This yields a total messiness cost of 4<sup>2</sup> = 16.  Now suppose the last word of line 0 had length 1, so after moving the last word from line 0 to line 1, both lines have 2 blank spaces at the end, yielding a messiness cost of 2<sup>2</sup> + 2<sup>2</sup> = 8.  Hence we conclude in this case a pure greedy algorithm is not necessrily optimal.

Now let's look at the tougher case: suppose moving the word from line 0 to line 1 caused line 1 to overflow, so one or more words had to move from line 1 to line 2.  This means that the number of characters moved from line 0 to line 1 must be greater than b_1.  So originally we had cost b_0<sup>2</sup> + b_1<sup>2</sup>, and now we have cost at least (b_0 + b1 + (positive number))<sup>2</sup>  (this is just from the first line, we assume the second line is filled completely to get a minimum cost bound).  Since we know for positive numbers (a + b)<sup>2</sup> is always greater than a<sup>2</sup> + b<sup>2</sup>, this new cost is always greater than the original cost, and so pushing the word onto the next line could never be the optimal choice.

So extending this argument inductively, pushing words onto the next line could only ever be beneficial if it wouldn't cause the next line to overflow.  We might ask, but what about in case 2 above if there were more lines, and pushing the word from line 0 to line 1 had increased the cost for lines 0 and 1, but decreased the cost for line 2, and by more than it increased the cost for lines 0 and 1 combined?  But this case is kind of like the two line case, where you can think of the line in the middle (line 1) as just a "middleman" line between 0 and 2.  This isn't a formal proof, but should offer some justification as to why it makes sense that pushing words from line 0 to 1, then 1 to 2 operates in a similar way to just pushing words from one line to the next.

Using this idea we have, we can write an algorithm that starts off greedily filling all the lines as much as possible, and then checks whether or not words can be moved to the next line to decrease the cost, only in cases where it wouldn't cause overflow.  The only issue is: when do we stop?  If you just do one top to bottom iteration, it's possible that moving a word from line 5 to 6 freed up space in the end of line 5, so now a word from line 4 and move to line 5.  This suggests iterating in reverse order from bottom-to-top instead.

The time complexity would be O(n) if n is the number of characters in the input (as we just perform a fixed number of passes over the data). If we just work with the data as strings, the space complexity will also be O(n).  However, we can track words as just integers (word length) rather than strings to avoid string copy overhead and reduce the space complexity to O(w), where w is the number of words in the input.

TODO: Test this algorithm and verify it produces the optimal results in all cases.

* * *

**The DP Solution**

So that was a very math/logic-based approach to solving this problem.  If you're thinking, there's no way I could have come up with that myself on the spot, especially in a high-pressure interview scenario, don't worry.  You can definitely fall back on a DP solution.

TODO: write up this solution, once I determine whether or not the book's explanation and algorithm make sense.

---

**Suppose the messiness cost function is simply the sum of the number of blanks at the end of each line (including the last).  Solve the messiness minimization problem.**

Let's take a quick digression into discussing the functionality of sum-of-squares versus sum-of-absolute-values loss/cost functions.  Take a moment to try to answer this question for yourself: Why are sum of squares cost functions so popular?  What's wrong with just summing up the values?

Suppose that a bunch of people are waiting to login to a server to play a game.  We want to design an algorithm to maximize user satisfaction, which means minimizing some combined metric of all user wait times.  Suppose we just used a sum-of-values cost function.  You try to login into the server, and wait for an hour, two hours, all day just to login.  You're thinking, I've been waiting forever, something must be wrong with the algorithm!  But there's nothing wrong with the algorithm it's optimizing around the cost function it has.  That cost function doesn't differentiate between making someone who's alredy been waiting for 12 hours wait another 30 minutes, versus making someone who hasn't waited at all yet wait for 30 minutes.

And this is why we introduce the sum of squares cost function: it gives increasing weight to increasingly large errors.  This is based on the mathematical fact that (for positive values) (a + b)<sup>2</sup> > a<sup>2</sup> + b<sup>2</sup>.  (You can prove this by just expanding the expression (a + b)(a + b) = a<sup>2</sup> + b<sup>2</sup> + 2ab.)  So a sum of squares error function tries to force all errors to be reasonably low, while a sum of values error function will just find the easiest terms to optimize and minimize them all the way to zero, while completely abandoning the hardest terms to optimize and letting them go super high.

So after all this digression, we return to the messiness optimization problem and ask what would happen if we just used a sum of values cost function?  Well there's no longer this penalty that we get for reducing the cost for the current term but increasing the cost for some other term that will come into play later.  Moving a word to the next row will *always* increase our current row cost by the length of the word, and will at *best* reduce the cost of future terms by that same amount.  So there's no reason to ever move a word onto the next line if it fits in the current line.

In other words, the simply greedy algrithm of filling the lines up as much as possible before each line break is optimal.  Time complexity is O(n) and space complexity is O(1) since we're just computing the cumulative messiness value, not storing the actual placement of all the words.  We could store the indices of each line break to get a more useful solution if we wanted, and this would only take O(num_lines) space.

---

**Find a longest nondecreasing subsequence of a given array.**

We can use the dynamic programming example given in the book, but track the previous index along with the length in our DP array (or use two arrays, one for each).  Then we can retrace our steps at the end to get the (reversed) longest nondecreasing subseqence.

Complexities are the same as the example: O(n<sup>2</sup>) time and O(n) space.

---

**Find a longest alternating subsequence of a given array. (Alternating means a\[i] < a\[i+1] for even i and a\[i] > a\[i+1] for odd i.)**

We can start by viewing this problem as an extension of the previous problem.  So we use the same algorithm as finding the longest nondecreasing subsequence, but now our comparison operator depends on the length of the sequence that we look up in the DP array.  So suppose we are currently at the value 10.  If the DP array gives us a length of 4 ending at index j, we have to check that the value in the input array at position j is less than the value at the current index in order for it to count as a valid subsequence candidate.  If, however, the length in the DP array were 5, the comparison operator would be flipped.

Retrieving the sequence at the end will be identical to the above problem.

Time and space complexities are still the same: O(n<sup>2</sup>) time and O(n) space.

However, perhaps counterintuitively, this problem is actually *easier* to solve than the above problem.  In fact, using dynamic programming here is rather akin to using a sledgehammer to drive in a small nail.  Forget the title of this chapter, forget the previous problems we've just worked on.

Approach this problem as a new problem, with a clean slate, and see if you can find a more efficient solution.

* * *

Okay, let's start by writing up an example sequence.  When designing our example, we want to make sure we cover cases where the sequence alternates back and forth immediately, as well as cases where there are runs of consecutive increasing or decreasing values.  Additionally, we make sure to cover the case where we have repeated consecutive elements, to make sure we handle equality correctly.
```
[4, 2, 7, 8, 8, 9, 9, 3, 2, 3, 5, 4]
```
If we start walking through the array from the left, we encounter `4, 2`.  By the problem definition, we are looking for an increasing pair of values to start the sequence, so we know for sure we can't start with these two values.  But if we wanted to keep one of them to start our sequence, which should we pick?

We select `2`, because it is lesser, and therefore less restrictive when it comes to finding a subsequent value that is greater.  And number greater than 4 is certainly greater than 2, but not vice versa. So we don't lose any possible solutions by throwing away the 4, and just holding onto the 2.

Next we encounter a 7, which satisfies the criteria we are looking for, so we keep the 7.  Let's call this place we're keeping these values our working solution vector. Our working solution vector currently contains: `[2, 7]`, and we are now looking for a value less than 7.  The next value is 8, which is not less than 7.  But, because it's not less than 7, that means finding a value less than this new value 8 is guaranteed to be easier, or as easy (if the values were equal), as finding a value less than 7.  So we replace the back element of our working solution vector with this new value, giving: `[2, 8]`.  Next we come across another 8, and this is where the equality case comes in.  By our logic above, we can handle this case uniformly with the previous case, so we just replace the back element (8) with the new element (also 8) as before.  (It may seem trivial now, but making note of these things will be a big help when it comes to writing clean and simple code.)

We continue in this way, replacing the 8 with a 9, the 9 with a 9, and then finally we find a 3, which is in fact less than the 9.  Temporary solution vector becomes `[2, 9, 3]` and we're looking for a value greater than 3 now.

Looking more generally at this process we've walked through, we can see why this problem is actually simpler than the longest non-decreasing subsequence case.  There is always a best choice now, which also never sacrifices any future options.  In other words, we have a greedy algorithm that's guaranteed to find the optimal solution.  A cool way of looking at this is that we have broken up the full input sequence into alternating runs of increasing and decreasing values, and then pulled out one representative - the final element - of each of these runs to constitute our ultimate solution. Looking at it this way, it's clear that we can't have missed anything, our solution must be optimal, and it must be valid.

The time complexity is O(n), as we just walk through the input array once.

The space complexity is exactly the size of the solution, or O(n) in general.

I have written up the C++ code for this algorithm here: [longest_alternating_subsequence.cpp](https://github.com/Apollys/EPI-Variants-Solutions/blob/main/Ch_16_Dynamic_Programming/longest_alternating_subsequence.cpp)

Bonus: if you wanted to eliminate the restriction that the alternating sequence must start with an increasing pair of elements (i.e., allow it to start decreasing also), how would you approach this?

The obvious way is just with two passes, run the above algorithm once starting by looking for increasing first, and once starting by looking for decreasing first.  But this is quite unnecessary, can you do it in one pass?

We can just start by looking for the first pair of non-equal consecutive elements.  Based on their ordering, we set our boolean indicating whether we're looking for increasing or decreasing values next, and continue with the algorithm exactly as described above.

---

**Find a longest weakly alternating subsequence of a given array.**

Naturally, we start by considering applying the above algorithm to this problem.  However, there is a potential worry that arises - when we encounter a pair of adjacent equal values, we could either flip searching for ascending/descending, or not, and these two choices then yield completely different future situations.  It seems the greedy approach could terminate a decision path that may eventually lead to the optimal solution.  Let's investigate with an example:

```
1 2 2 0 1
    ^
```

Here we are at the second 2, currently looking for a weakly descending value.  This 2 could could as either weakly ascending or weakly descending.  Suppose we greedily count it as weakly descending, so now we're looking for weakly ascending values.  The next step in the algorithm looks like:

```
1 2 2 0 1
      ^
```

We encounter a 0, but we're looking for a value greater than or equal to 2.  So the check fails.  But we still use the same trick we used before!  Because the check failed, we know if we use this value (0) instead of the previous value (2), it will make finding the next ascending value easier.  So we utilize this 0 anyway, meaning at the next step when we encounter the 1, it will be checked against 0, pass, and be added to our solution sequence.

So, the same greedy algorithm as above also works in this case.

O(n) time and O(n) space (or more specifically, O(solution size) space).

---

**Define a sequence of length k to be convex if a\[i] < (a\[i-1] + a\[i+1])/2 for 1 <= i <= k-2.  Find the longest convex subsequence of a given sequence.**

The first thing we should do is get an idea of what this definition means.  A sequence is convex if each value is less than the average of its adjacent left and right values.  Visually, this means each point lies below the line segment joining the two neighbors.  This is the discrete analog to a [convex function](https://en.wikipedia.org/wiki/Convex_function):

![ConvexFunction](https://user-images.githubusercontent.com/37650759/142391320-02535b0c-a2cc-40a8-863e-34148c2b0f20.jpg)

With this picture in mind, we might consider starting at the global minimum of the input sequence and working our way outwards from it.  Can you think of a counterexample, a case in which the longest subsequence does not contain the global minimum?

```
longest convex subsequence
|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
11 7 4 2 1 2 4 7 11 0 1
                    ^
              global minimum
```

If we tried to use the global minimum, the longest convex subsequence we could get would be `11 4 1 0 1`.  So this doesn't work.

What if we did something like, at each position, select the leftmost value to the right of the current position that satisfies the convexity requirement?  Well, the problem is, there might be a value farther right that satisfies the convexity requirement, but is less than the value we picked, making the *next* convexity requirement easier to satisfy.  In other words, there's a trade-off: selecting earlier values gives more options for future values, while selecting lesser but later values relaxes the requirement on future values.  Either option could lead to an optimal solution (try to draw a case for each to convince yourself of this), so we can't make a greedy decision like "select the earliest legal next value" or "select the minimum legal next value" -- neither will succeed in general.

With greedy solutions seeming to be doomed for this problem, we switch our attention to dynamic programming.  We might start by thinking to track the longest convex subsequence up to each value in an n-sized DP array.  But remember, the previous values determine the conditions the next value must meet.  So using the longest convex subsequence up to a given value might not produce the longest convex sequence overall, as it may be more restrictive than some shorter convex subsequence - this is the exact problem we discovered with the greedy solution above.  So we need to track more than this.

With tricky dynamic programming problems, there are a couple ways you can go about developing your solution.  One is to try to cleverly guess what the various axes of your DP matrix should represent, and then build the algorithm that fills up this matrix and see if it works.  Another is a more top-down approach: remembering that dynamic programming is motivated by the idea of using recursive solutions while avoiding repeated calculations, we can start by writing a recursive algorithm, and then use that to devise our DP table.

// TODO - finish writing up this solution

---

**Define a sequence to be bitonic if it strictly increases up to some index k, then strictly decreases from k onwards.  Find the longest bitonic subsequence of a given sequence.**

Use the original DP solution to the longest nondecreasing subsequence problem to generate DP arrays for the longest increasing and longest decreasing subsequence problems.  However, the trick is, for the longest decreasing subsequence problem, we want to solve it in reverse so our DP array holds the length longest decreasing subsequence from index i to the end of the array, for each index i in the array.  The longest increasing subsequence array is computed normally, and so holds the length of the longest increasing subsequence from the start to index i, for each index i in the array.

Now we can find the longest bitonic subsequence by iterating through each index in the array, computing the length of the longest bitonic subsequence with peak at index i as the sum of the lengths in the two DP arrays at index i (minus 1, since they share a common value).  Recover the sequence as in previous problems.

Time and space complexities are O(n<sup>2</sup>) time and O(n) space as before.

---

**Define a sequence of points in the plane to be ascending if each point is above and to the right of the previous point.  Find a maximum ascending subset of points in the plane.**

Sort points by y coordinate, and now the problem is reduced to finding the longest increasing subsequence on x coordinates.

Still O(n<sup>2</sup>) time and O(n) space.

---

**Compute the longest nondecreasing subsequence in O(n log(n)) time.**

If your spider senses were tingling this whole time that it seems kind of inefficient to use O(n<sup>2</sup>) time for all these algorithms, good!

A good thing to do when you see a problem and have sketched out a rough algorithm is to then write out what operations you need.  In our case we need the following:
 - Look up largest length associated with end_value <= given value
 - Insert new length and end_value
 - Potentially delete and/or update (end_value, length) pairs by length or end_value (not sure which we will use yet)

This should be *almost* looking like an amazing candidate for a BST, except that first operation which is quite tricky.  However, let's think about that a little more carefully. Why couldn't we just look up the largest value and use that?  Well, because the length of a sequence ending in some large value may actually be less than the length of a sequence ending in some smaller value.  But hold on!  If that were the case, why would that shorter but larger-end-value sequence ever be useful to us?  It wouldn't! It serves only to complicate our algorithm.  So how about we assume we will always throw out a sequence if another sequence catches up to it in length but has a smaller end value?  Then with this in play, the first criteria above becomes:
 - Look up largest (length, value) pair, which we know *must* have both the largest length and the largest value among all items in the data structure

Okay, now a BST seems perfect.  It's still not clear if lengths or values should be our key: let's run through a quick example and hopefully that should answer our question and build a nice picture of how our algorithm will work.

Input array = `[0, 5, 3, 6, 2, 4, 5]`.  Initialize empty BST of (end_value, length) pairs - not sure which is the key yet.
 - Encounter value `0`.  Check BST for largest item with end_value <= 0.  Okay, the BST key needs to be end_value.  No value found, so we start a new sequence.  Insert (end_value = 0, length = 1) into BST.
 - Encounter value `5`.  Search BST for largest item with end_value <= 5.  Find (end_value = 0, length = 1). Insert (end_value = 5, length = 2) into BST.  Note, we didn't overwrite the previous pair, but inserted a new one, because if in the future we find a value that could use that end_value = 0 sequence, we want to make sure we keep that option available.
 - Encounter value `3`.  Search BST for largest item with end_value <= 3.  Find (end_value = 0, length = 1) - now we're glad we saved this one as well rather than overwriting it.  Insert (end_value = 3, length  = 2) into BST.
   - Now we have two sequences in our BST with length 2!  We need a way to detect that and delete the one with higher end_value (because it's never better than the one of same length and lower end value).  How can we do that?  A common trick when working with BSTs is to keep around a sort of inverse hash map - in this case we can keep a hash map from lengths to BST nodes (or lengths to end_values, time complexity will be the same in this case but it is "more" computation).
   - Okay, rewind.  So whenever we inserted a node into our BST, we also needed to insert the (length, BST node) pair into our hash map.  Let's suppose we did that up to this point.
   - So we've just decided we want to insert (end_value = 3, length = 2) into the BST.  Let's rewind to right before we did that.  Look up length = 2 in the hash map -> find end_value = 5 corresponds with this length.  Since our new end_value is 3 which is less than 5, delete this old node from the BST, insert (end_value = 3, length = 2) into the BST, and update the hash map to map length 2 to our newly inserted node.
 - Encounter value `6`.  Search BST for largest item with end_value <= 6.  Find (end_value = 3, length = 2).  Check for length 3 in hash map, doesn't exist, insert (end_value = 6, length = 3) into BST and add newly inserted node into hash map.
 - Encounter value `2`.  Search BST for largest item with end_value <= 2. Find (end_value = 0, length = 1).  Check for length 2 in hash map -> exists with end_value = 3.  Current value is less than 3 so delete corresponding node from BST, insert (end_value = 2, length = 2) into BST, and update hash map with newly inserted node.
 - Encounter value `4`.  Search BST for largest item with end_value <= 4.  Find (end_value = 3, length = 2).  Check for length 3 in hash map, find end_value = 6. Since 6 is greater than 4, delete that node from BST and insert new node (end_value = 4, length = 3). Update hash map with newly inserted node.
 - Encounter value `5`.  Search BST for largest item with end_value <= 5. Find (end_value = 4, length = 3).  Check for length 4 in hash map, not found.  Insert new node in BST (end_value = 5, length = 4). Update hash map with newly inserted node.
 - Done processing array.  Iterate from length of input array down to 1, searching for the first length found in the hash map.  This is the length of the longest nondecreasing subsequence in the input array.

Okay, so we ironed out most of the details.  The BST will take (end_value, length) pairs with end_value as the key.  The hash map will map lengths to BST nodes.  Before each insertion into the BST, we always check if a node alraedy exists with the same length, and keep only the better option.

However, we're still missing a way to recover the sequence in the end.  As you may have noticed by now, given the way these DP algorithms work, it's generally easiest to store an inverse mapping of the sequence that tells you where you came from at each position, and then you can recover the sequence in reverse once the algorithm is complete.  So we can keep an extra array that just stores the index of the previous element in the optimal subsequence, for each position in the array. But in order to know this information as we are running the algorithm, our BST (or an additional hash map) will need to additionally store the previous index for each node, so the data will be (end_value, length, previous_index).  Then whenever we look up the maximum value in our BST, we can store that previous_index in our previous index array so we will be ready to unwind the sequence at the end of the algorithm.

One last thing: we don't quite know where the longest subsequence will end in the array.  It very likely will not be the last element.  So at each step of the algorithm, let's just track max_subsequence_length and longest_subsequence_end_index.  Then at the end we can start our reverse subsequence unwinding at longest_subsequence_end_index.

To analyze the time complexity, we do n steps (process each element in the input array once), and for each step perform various binary tree single-node operations, which gives O(log(n)) time for each of those steps.  At the end we just retrace the longest subsequence path so that takes O(n) time.  This yields O(n log(n)) time complexity.  Space complexity will be O(n) for our various data structures, each of which stores at most a single item per element from the input array.

See my source code for this algorithm here: [longest_nondecreasing_subsequence_optimal.cpp](https://github.com/Apollys/EPI-Variants-Solutions/blob/main/Ch_16_Dynamic_Programming/longest_nondecreasing_subsequence_optimal.cpp)

---
