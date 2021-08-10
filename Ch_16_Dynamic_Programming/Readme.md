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

