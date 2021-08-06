## Chapter 15 - Recursion - Variants Solutions

---

**Towers of Hanoi note: The book (at least my version) has a confusing error in which they label the pegs P0, P1, P2 in the diagrams, and P1, P2, P3 in the questions.  I will use the notation P1, P2, P3 for all my discussion here.**

---

**Solve the Towers of Hanoi problem without recursion.**

Firstly, if you haven't read it already, please see my walkthrough on general [recursive to iterative algorithm translation](https://github.com/Apollys/EPI-Variants-Solutions/blob/main/Ch_09_Binary_Trees/Readme.md#recursive-to-iterative-algoirthm-translation---nonrecursive-binary-tree-traversals).  Using this, we can translate this recursive algorithm into an iterative algorithm and call it a day.

However, you might be looking for a more direct approach since this problem is relatively simple.  Let's look at what happens in some simple examples.

If move one ring onto P3, then we have to move the second smallest ring onto P2, after which we can stack the smallest back on P2.  So by starting with a move to P3, we built a stack of the two smallest rings on P2.  From here, we could move the third smallest ring onto P3 (now empty). And by stacking P2 onto P3 (using the method before), we now get a stack of three rings on P3.  Then the fourth smallest ring could go on P2, and we would get a stack of 4 on P2.

So the important observation here is that we will always get odd-numbered stacks of rings on the peg we first moved to, and even-numbered stacks of rings on the peg we save for the second move.  So if we label our pegs `P_first = n is odd ? P2 : P3` and `P_second = n is odd ? P3 : P2` then our algorithm starts as follows:

```
Let's call smallest ring = R1, second smallest ring = R2, ...
and PF = P_first, PS = P_second

R1: P0 -> PF
R2: P0 -> PS, R1: PF -> PS
R3: P0 -> PF, R1: PS -> P0, R2: PS -> PF, R1: P0 -> PF
R4: P0 -> PS, [Cumulative solution up to the previous line to move R1-R3 to PS using P0]*
              *with adjusted peg numbers for the moves
...
```
Our observation is that since we're storing the solution as we go, we can use that solution that we've stored again to move the *k-1* blocks on top of the *k*th block, however, we have to be careful to shuffle the poles that we are using.  So I would keep track of this by categorizing the poles as "start pole", "target pole", and "auxiliary pole" at each step, and then when you reuse the previous sequence, you map start pole to start pole, target pole to target pole, and auxiliary pole to auxiliary pole.

So the high level idea is: 1) compute the starting pole, 2) start iterating, at each step k moving the next ring (kth smallest ring) from P0 onto the other empty pole, 3) reuse the solution up to the move before the last move with remapped pole numbers to move the stack of k-1 rings onto ring k, 3) when k = n, done.

---

**Solve the Towers of Hanoi problem where each operation must involve P3.**

We just have to make sure we cover all our base cases and recursive cases correctly.

Base case: move k = 1 ring from start_pole to target_pole.
 - If {start_pole, target_pole} contains P3, move ring from start_pole to target_pole
 - Otherwise, move ring from start_pole to P3 to target_pole

Recursive case: move k rings from start_pole to target_pole, given solutions for all values less than k.
 - Move top k-1 rings from start_pole to P2 recursively
 - Move kth ring from start_pole to P3
 - If target_pole = P3, Move k-1 rings from P2 to P3
 - Else:
   - Move k-1 rings from P2 to P1 recursively
   - Move kth ring from P3 to P2 = target_pole
   - Move k-1 rings from P1 to target_pole recursively

For this type of problem, make sure to be very scrutinous in the verification of your own logic - it's easy to miss a case or assume something you can't assume in this type of abstract inductive/recursive solution.  Note that I *was* allowed to assume that I can move k-1 poles from any pole to any other pole without checking the P3 condition (because this is a recursive step), but I had to be very careful when moving just the kth ring to make sure it always used P3.

---

**Cyclic transfer Towers of Hanoi - each transfer must be from P1 to P2, P2 to P3, or P3 to P1.**

Again, try to be very rigorous in your logic as out outline the base and recursive cases:

Base case: move k = 1 ring from start_pole to target_pole.
 - If direct move is legal, move from start_pole to target_pole
 - Otherwise, move from start_pole to other_pole to target_pole

Recursive case: move k rings from start_pole to target_pole, given solutions for all values less than k.
 - If direct transfer from start_pole to target_pole is legal, do:
   - Move k-1 rings from start_pole to other_pole recursively
   - Move kth ring from start_pole to target_pole
   - Move k-1 rings from other_pole to target_pole recursively
 - Otherwise (direct transfer from start_pole to other_pole is legal, and transfer from other_pole to target_pole is legal), do:
   - Move k-1 rings from start_pole to target_pole recursively
   - Move kth ring from start_pole to other_pole
   - Move k-1 rings from target_pole to start_pole recursively
   - Move kth ring from other_pole to target_pole
   - Move k-1 rings from start_pole to target_pole recursively
 
---

**Solve the Towers of Hanoi problem where direct transfer from P1 to P2 is not allowed.**

In very similar veins to the previous, we analyze the base and recursive cases carefully:

Base case: move k = 1 ring from start_pole to target_pole.
 - If direct move is legal, move from start_pole to target_pole
 - Otherwise, move from start_pole to other_pole to target_pole

Recursive case: move k rings from start_pole to target_pole, given solutions for all values less than k.
 - If direct transfer from start_pole to target_pole is legal, do:
   - Move k-1 rings from start_pole to other_pole recursively
   - Move kth ring from start_pole to target_pole
   - Move k-1 rings from other_pole to target_pole recursively
 - Otherwise (any direct transfer is legal except start_pole to target_pole), do:
   - Move k-1 rings from start_pole to target_pole recursively
   - Move kth ring from start_pole to other_pole
   - Move k-1 rings from target_pole to start_pole recursively
   - Move kth ring from other_pole to target_pole
   - Move k-1 rings from start_pole to target_pole recursively

We notice we get almost the exact same algorithm as in the previous problem.  The constraints are just slightly relaxed here because "If direct transfer from start_pole to target_pole is legal" is more often true.

---

**Solve the Towers of Hanoi, but the only requirement is that the bottom ring be larger than the rest on each pole.**

This one is a bit tricky.  You may be wondering if this relaxed condition actually helps you get fewer moves, and I would really recommend you cut out 4-5 strips of paper of increasing length (to represent your rings) and walk through some scenarios by moving the rings around on your desk.  It will really bring the problem to life, and should allow you to convince yourself that this relaxed restriction really does help.

Note: it's not necessarily obvious whether the goal should be to get the rings to the target pole in any order, or in the starting largest-smallest order.  However, either way, the solution we get here is an improvement over the original Towers of Hanoi solution.

Let's walk through the solution for n = 5 rings, labeling the rings R1...R5 from smallest to largest:
 - Move R1 from P1 to P2
 - Move R2 from P1 to P3, move R1 from P2 to P3
 - Move R3 from P1 to P2, and "flip" everything from P3 to P2
 - ...
 - On the kth iteration, move the ring k from P1 to the empty pole
   - Move all rings from the other pole onto the pole with ring k, flipping their order upside down
   - Note: this will always be possible, because other pole contains rings 0 through k-1, all smaller than ring k
 - ...
 - If the goal is just to get all the rings to the target ring in any legal order, then as soon as we've completed n iterations of this process, we are done
 - If we want the rings to end up in the same order they started in, largest to smallest from bottom to top, we need to add one more step after the nth ring is moved to the target pole:
   - At this point, ring n-1 is at the bottom of the other pole's stack.  If we observe a little more closely though, we'll notice that ring n-2 is on top. And ring n-3 is on the second from bottom, ring n-4 is on the second from the top, and so on...
   - So all we need to do is repeatedly flip this stack onto the open pole and grab the last ring to place on our target pole
   - Once this is complete, we'll have all n rings on the target pole in the nice clean starting order!

The one thing we've glossed over here is which pole to move the first ring to - this first move actually determines the entire course of the algorithm.  Like we observed before, there is an alternating parity at play here.  All odd rings will end up as the base of wherever we move R1 to, and all even rings will end up as the base of wherever we move R2 to, so if n is odd, we make our first move to the target pole, and if n is even, our first move goes to the other pole.

---

**Towers of Hanoi - 2n disks of n different sizes, 2 of each size.**

This is exactly the same as the original Towers of Hanoi problem, if you think of each move as moving a pair of blocks.

---

**Towers of Hanoi - 2n disks, n white and n black.  Black on top of white is allowed, white on top of black is not allowed.  Clarifying assumption: each ring is a different size, and the normal rules of Towers of Hanoi also apply here (the book's working of this problem is rather vague).*

Let's first think about what the problem condition entails: "You cannot place a white disk directly on top of a black disk."  This means that as soon as we place a black disk on a pole, there can never be *any* white disks on top of it.  So the initial configuration will be n white disks on the bottom, and n black disks on top.

From here, we know we could use the basic Towers of Hanoi solution to move those top n black rings anywhere we want.  Just for the sake of experimentation, let's suppose we moved them all to P3, and then moved the smallest white ring to P2.  We notice that again here, we could move the n black rings anywhere we want.  For example, we could stack them on P2, move the second smallest white ring to P3, then move the black rings as a block *again* to P1, and move the smallest white ring from P2 to P3.  Now if we moved the black rings back to P3, we'd have a stack of the n+2 consecutive smallest rings.  But how do we generalize this?

Again, I highly encourage you to cut out some strips of paper and play with this for a few minutes.  4 black and 4 white rings would be plenty.

What we realize from our experimentation is that no matter what the configuration of the white rings is, we can always move the black rings anywhere we want using the basic Towers of Hanoi solution.  Now comes the second bigger flash of insight:  If you wanted to make an arbitrary move of some white ring from one pole to another, assuming the move is valid with respect to the white rings only, how could you do that?

Let's label our three poles start_pole, target_pole, and other_pole.  Suppose we want to move a white ring from start_pole to target_pole, and assume that move would be valid if there were no black rings in the game.  Then, we can make that move by moving all the black rings to other_pole via the basic Towers of Hanoi solution (do nothing if they're already on other_pole), then making the white ring move.  The only assumption this relies upon is that we keep the black rings in a single block - but that's necessary anyway for ever moving a white ring.  So we have a solution that effectively ignores the black rings.

Now if only we knew what the optimal sequence of white ring moves were, that would give us the answer...

But we do!  That's just the basic Towers of Hanoi solution again of course, since we're ignoring black rings now.

So all we need to do to put everything together is 1) compute the optimal sequence of moves for the n white rings, and then 2) intersperse each move with the optimal sequence for moving the n black rings to the unused ring that's not part of the white ring move.  And that's our solution.

---

**Towers of Hanoi with 4 pegs**

So how can we make use of this extra pole?  Let's start brainstorming some ideas:

To move k rings from P1 to P2, we could first move k-2 rings out of the way instead of just k-1, lets place them on P4.  Then we can move the k-1'th ring to P, freeing us up to move the k'th ring to P2.  Now we just reverse the process by moving the k-1'th ring from P3 to P2, and then the remainig k-2 rings from P4 to P2.

But is this our only option?  Could we have made a different choice somewhere?  Well of course, we could choose to move any number of rings to P4 on the first step.  Let's say we chose to move *i* rings to P4 on the first step.  Then we move *k - 1 - i* rings to P3 on the next step, leaving just the kth ring left.  Now we move the kth ring to our target pole and stack everything back up.  So we've effectively split the problem into two recursive problems of size *i* and *k - 1 - i*, and final number of steps will be dictated by the sum of the number of steps in the two subproblems.

What will be more efficient, splitting into two problems of similar sizes, or into one small and one large problem?  Intuitively, we might think it makes sense to balance the size of the subproblems as much as possible. (Mathematically, this would be the optimal choice for any superlinear function, while the most imbalanced case would be optimal for a sublinear function. For linear time functions, all choices would be equal.)  Let's look at the numbers to make sure we get the right solution:

```
Rings -- Moves
2	------ 3
3	------ 7
4	------ 15
5	------ 31
6	------ 63
7	------ 127
8	------ 255
9	------ 511
10	---- 1023
```

As we can see, the function for the optimal number of moves is certainly superlinear - it's exponential in fact.  The exact formula is just num_moves = 2<sup>n</sup> - 1.  So we want the most balanced division of subproblem size for an optimal solution to the 4 peg problem.

In fact, this observation generalizes directly to the optimal solution for the Towers of Hanoi problem with an arbitrary number of pegs.

---

**Compute all possible character sequences associated with a phone number without using recursion.**

Once again, we already know how to convert a general recursive algorithm to an iterative algorithm, so we can always fall back on that if needed.  However, we can also try to more directly approach this problem.

As they mention in the beginning, you could use 7 nested for-loops to do this iteratively.  Now I really have a problem with them calling this the "brute-force" method - this is actually the most efficient possible method, there's nothing brute-force-ey about it. But the problem is it's not extensible.  What if I said phone number don't have 7 digits, but n digits? Now we're in trouble.

So what we want to do in the more general case is keep track of the currently active prefix set, and then for each new digit, append all possibilities for that character to each element in our prefix set.  So it would be something like this:

```c++
// Note: take this as more of psuedo-code, I haven't tested it or checked for edge cases,
// it's meant to outline the main idea of the algorithm and clarify how the iteration works.

std::unordered_set<std::string> prefix_set{""};
for (int digit : input_number) {
    std::vector<char> char_options = digit_map[digit];
    std::unordered_set<std::string> next_prefix_set{}
    for (const std::string& prefix : prefix_set) {
        for (const char c : char_options) {
            next_prefix_set.insert(prefix + c)
        }
    }
    prefix_set = std::move(next_prefix_set);
}
```

---

**Compute the number of configurations of n non-attacking queens on an n x n chessboard.**

Once again the "compute all the answers" question strikes, and so we can safely jump straight to brute force, at least as a starting point.  We'll enumerate all possible placements of the queens, which is equivalent to enumerating all possible permutations of length n (since we know a priori that all queens must be on their own row and their own column).  Check if each is a solution, and if so add to solution set.

There are slight optimizations to be had here with respect to reflective and rotational symmetry - any position can be rotated to one of four distinct positions, reflected vertically, horizontally, both, or neither, (4 options), giving up to 16 variations of any one position (note that not all will be unique).  You could try to find a "canonical representation" for each of these up-to 16 positions, or even better, enumerate your positions such that you generate exactly one position from each of these equivalence sets.

However, I don't find this optimization particularly interesting or useful, so I'm not going to bother with it for now.  Perhaps later if I am so motivated, I will come back and add it.

A more interesting variant of this problem is to think about how you might find any configuration of n non-attacking queens.  In practice, there are many interesting methods such as hill climbing with random restart, and other stochastic search processes that perform very well on this problem.

---

**Compute the smallest number of queens that can be placed to attack every empty on an n x n chessboard.  (Queens do not have to attack/defend each other.)**

Let's start working our way up through some examples:

```
n = 2     n = 3     n = 4      n = 5

Q x       x x x     Q X X X    ???
x x       x Q x     X x x x
          x x x     X x Q x
                    X x x x
```

It's pretty clear that our solutions here for n = 2 to 4 are optimal.  Now try drawing out some examples of n = 5 to see if you can cover all the squares without adding another queen from the n = 4 solution.  With a little experimentation, you'll see that it can't be done.

I haven't tried to write up a formal proof of this (again because I don't think this problem is very valuable), but I do remember spending a reasonable amount of time drawing up a bunch of examples for chessboards up to n = 8 and convincing myself that there's nothing better to do than just trivially adding a queen to the corner each time you increment n past 3.

Thus, my solution is *max(1, n - 2)*.  This is not a programming question at all, more of a geometry/logic puzzle, which is why I won't spend too much time on it here.

---

**Compute a placement of 32 knights, or 14 bishops, or 16 kings, or 8 rooks on an 8x8 chessboard in whic no two pieces attack each other.**  (Note I added an "or" to the wording in my book, which was very unclear.)

Okay this is not a programming question at all.  If you want to skip this question entirely, go ahead, that's what I did.  But since I'm making a solutions reference guide now, I figured I'll give the answers now.

**32 Knights.**  Observation: knights only attack opposite-colored squares from their current square.  Thus, if we place knights on all the white squares, there's no way they can attack each others.

**14 Bishops.**  Bishops don't attack the squares next to them.  So if we start by placing a row of bishops along the top edge of the board, we notice the bottom edge of the board is unattacked except for the two corners.  So we place bishops in all non-corner squares of the bottom row and that gives us 6 + 8 = 14 bishops.

**16 Kings.**  Kings attack a ring of squares around themselves, so we just need to place the kings on a spaced grid for optimal placement.  Of course we start in the corner so we can get as many kings as possible on the board.  This gives us a grid of 4 x 4 = 16 kings.

**8 Rooks.**  Rooks don't attack diagonally, so just place them along a diagonal. It happens the longest diagonal has length 8, so we're done.

---
