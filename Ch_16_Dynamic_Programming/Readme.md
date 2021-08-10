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
