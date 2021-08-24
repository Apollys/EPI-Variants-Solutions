## Chapter 4 - Primitive Types - Variants Solutions

---

**First, a note about bit manipulation questions**

Clever bit manipulation optimizations are a very niche programming skill that 99.9999+% of the time will be completely useless.  Outside of textbook questions, never in my life have I used any bit manipulations other than simple bit-masking - and that's coming from someone who's done a lot of computer graphics and computer vision programming, often with pretty tightly optimized algorithms.  In the modern programming world, compilers and low-level libraries take care of all this stuff, and programmers need to focus more on the mid-level logic (rather than super low-level optimization).

Secondarily, talking about time and space complexity with respect to these operations the way the book does really doesn't make sense.  Our integers are of a fixed size, and for that reason specifically, we can make assumptions like "addition of two integers takes constant time" or "an integer can be stored in constant space".  If you start to talk about the width of an integer as a variable, you *have* to throw away all these assumptions, which means saying something like "Compute the result of some operation on integer x in O(1) time and space" is clearly impossible - you can't even store x in O(1) space or read x in O(1) time.

So what does the book really mean when they say "compute this result in O(1) time and space"?  They mean that they don't want you to loop through the bits, but they want you to use [bit fiddling tricks](https://graphics.stanford.edu/~seander/bithacks.html) instead (this link is just one of probably many resources available on the subject).  If you can't derive these yourself, or don't see the point in stressing yourself out trying to solve this type of question, I would recommend just writing *any* algorithm to solve the questions without worrying about the "time complexity" requirements the book asks for.  After all, if you're writing an algorithm for a fixed-size integer, which you are, then your algorithm is constant time and space.  And if integers could be arbitrary sizes, there cannot exist constant time and space solutions.

That being said, below I'll try to outline the solutions I think the book authors wanted us to arrive at.  Nonetheless, I would like to re-emphasize that all of them are O(n) time and space if you allow the integer size *n* to vary arbitrarily.  And if you don't allow *n* to vary arbitrarily, any algorithm is O(1) time and space.

**Bitwise operations: right propogate the rightmost set bit**

Our input value is going to look something like: `...10000`, where we don't care about anything to the left of the rightmost `1`.

Our goal is to transform this into: `...11111`.  Since we're trying to avoid looping through bits, we can ask ourselves, how do we mathematically transform a bunch of zeros into a bunch of ones with a single operation?  Perhaps this will spark you to notice that `1000 - 1 = 0111`.

Now what happens to the rest of the bits if we do this `- 1` operation?  Suppose we write our input value as `x = abc10000`.  Then `x - 1 = abc01111` -- the left bits are completely unchanged by this minus one operation.  So we can get our desired result by combining these two values with a bitwise or: `x | (x-1) = abc11111`.

---

**Bitwise operations: compute x mod a power of two**

So we will be given a modulus that is some power of two, i.e. looks something like `00010000` in binary, and a value which could be anything.  To compute this modulus, we can simply ignore all the bits from the first 1 in the modulus's binary representation and leftwards, as those represent the modulus, then twice the modulus, four times the modulus, etc.  We just need to keep bits in the value in positions to the right of the 1 in the modulus.

In order to implement this, let's use the trick we used above: if `modulus = 0001 0000`, then `modulus - 1 = 0000 1111`, and exactly gives us a bit mask of the bits we care about.  Thus, our result is just `(modulus - 1) & value`.

---

**Bitwise operations: test if x is a power of 2**

This is a classic bitwise manipulation trick.  Again, think about what happens when you subtract one from things that look like `0001 0000` in binary.

Let's look at some examples:
```
x     = 0001 0000
x - 1 = 0000 1111

x     = 0001 0110
x - 1 = 0001 0101
```

Notice that when x was a power of two, that bit that was originally set in x must always flip to a zero in x - 1.  On the other hand, if x is not a power of two, that will never happen, (though we can see zero is a distinct case we may have to deal with separately, as it has no ones at all).

So if we bitwise-and `x` with `x - 1`, there will never be anything in common when x is a power of two.  But if x has more than one `1` in its binary representation, only one of those `1`s can change by subtracting 1, and thus this bitwise-and result will give us some nonzero value.

There's one last case to consider: what if x has zero `1`s in its binary representation?  Well then we can return false because x = 0.

In summary, to test if x is a power of two, check if x is nonzero, and then check if `x & (x-1)` is zero.  This can all be combined on one line of code with no branches for optimal performance.

---

**Bitwise operations: find the value with the same number of `1`s in the binary representation that is as close as possible to the given value.  Use O(1) time and space.**

As I've said before, either you assume a fixed maximum integer size, in which case every algorithm is O(1) time and space, or you assume arbitrary integer size, in which case it takes O(n) time *guaranteed* to just hold and read the given value.  In any case, let's try to do this without looping through the bits, which I would guess is what the book authors want.

We will start with what they deduced in the example O(n) solution: the answer is the given value with the rightmost differing consecutive bits swapped.  Another more useful way to think about this is that we need to find the rightmost `0` bit, or the rightmost `1` bit, and the solution will involve one of these two options.  How can we do that?

Well, if we add 1 to our value, we will be guaranteed to turn the rightmost zero into a 1. We will also flip all the bits to the right of that rightmost zero, and leave all bits to the left completely unchanged:
```
        x = 1111 0111
    x + 1 = 1111 1000
x ^ (x+1) = 0000 1111
```

Let's call this last line our mask: it will always be a sequence of zeros followed by a sequence of ones.  This means if we add 1 to it, we will get a power of two: a number with exactly one 1 in binary:

```
            x = 1111 0111
         mask = 0000 1111
     mask + 1 = 0001 0000
(mask+1) >> 1 = 0000 1000  // gives us the location of the rightmost zero in x
```

Now that we've got a mask identifying the position of the zero we want to flip, the rest is straightfoward.  We flip the zero to a one by xor-ing `x` with the (post-right-shift) mask.  And then we flip the adjacent 1 by right shifting the mask again and xor-ing it with `x`.

But remember this is only half the solution.  What if we had a value like `x = 0001 0000`?  Then our answer lies in finding the rightmost `1` bit and doing the same pair of bit toggles.  Conveniently, finding the rightmost `1` bit and the rightmost `0` bit are equivalent problems - if you have the solution to one, you just complement the input and perform the same solution to get the solution to the other.  So we thought we were halfway done, but actually we were 95% of the way done.  Now all we have to do is find a way to chose which solution to use in which cases.

An easy method is if `x`'s rightmost bit is 0, we look for the rightmost 1, and otherwise (when `x`'s rightmost bit is 1) we look for the rightmost 0.  Either way, we toggle the two adjacent bits using xors with the mask as described above.

Note: the book also gave a formula for extracting the lowest set bit in x: `x & ~(x - 1)`.  You can definitely use this formula instead, and it would actually be fewer operations in total.  However, in case you didn't have that formula handy, I wanted to show when felt like a more natural derivation (to me anyway).  It's also important to realize that these bit fiddling problems will usually have many different solutions - and for almost everyone, any solution is good.  There are much more valuable things to do with your time than worrying about counting CPU instructions for some bit twiddling algorithm that's going to be super fast either way.

---

**Write a primitive division algoriothm for signed integers - only addition, subtracting, and shifting allowed**

Given the solution for unsigned integers, we can just compute the sign bit, then convert the values to positive integers and reuse the previous solution.  We negate the quotient in the end if the computed sign bit is 1:

```c++
int Divide(int x, int y) {
    bool quotient_negative = (x < 0) ^ (y < 0);
    int unsigned_quotient = DividePositive(std::abs(x), std::abs(y));
    return quotient_negative ? -unsigned_quotient : unsigned_quotient;
}
```

---

**Given four points in the plane, check if they are the vertices of a rectangle.**

We must start by thinking about what it means to be a rectangle.  It may be easier to think conversely, what sets of vertices do *not* form a rectangle?  Well, if the angles aren't all right angles, it's not a rectangle.  And that's the only scenario in which it's not a rectangle.  So we just need to check if all angles are right angles.

There's one little subtlety though, we need to make sure we choose a valid ordering of our points.  For example, suppose we had the rectangle ABCD:

```
A-----B
|     |
D-----C
```
but our function was called as `IsRectangle(A, C, B, D)` or `IsRectangle(A, B, D, C)`.  We would have to reorder the points to a clockwise or counterclockwise sequence first.  We can do this by picking a vertex with minimum y coordinate first, and sorting all points by angle relative to that point.  (This is a nice trick to have in your belt, as it applies to much more general scenarios like the class Convex Hull problem).  The reason we picked minimum y as our anchor point is that all angles relative to it will be positive, so we don't have to worry about any angle wraparound issues.

Now that we have the points in order, we need to check that adjacent pairs of sides form right angles.  Adjacent pairs of sides are formed by the two vectors from a vertex v to the vertices before and after v.  To check if two vectors are perpendicular, we can check if their dot product is zero.  (Note: you might have thought here to use the formula for included angle between two vectors: `cos(theta) = (v • w) / (|v||w|)`, which is a good instinct.  But notice here as a slight optimization - when theta = 90 degrees, the cosine is zero, therefore `(v • w) / (|v||w|) = 0` and so we can skip the division by the product of magnitudes as well as the inverse cosine operation with a little cleverness.)

---

**Check if two rectangles (not necessarily axis-aligned) intersect.**

In the most general case, under what conditions do two rectangles in a 2d plane intersect?

Firstly, they intersect if any of their sides intersect.  Does this cover everything? Is there another case where they intersect?

Yes. What if one rectangle is completely inside another rectangle?  Then they also intersect, but no edges intersect.

And this is guaranteed to cover all cases.  For two rectangles R1 and R2 to intersect, they must have some common region.  Let R1 be the smaller of the two rectangles (i.e. area(R1) <= area(R2)).  Then that common region will either be all of R1 or not all of R1.  If it is all of R1, then R1 is completely contained in R2.  If it is not all of R1, then there is some region of R1 that lies outside R2, but also some region of R1 that lies inside R2. Therefore the borders of R1 and R2 must intersect.

So our algorithm needs two components:
 1) Check if two line segments intersect
 2) Check if a rectangle is completely contained inside another rectangle

To solve subproblem 1, we start by checking if the two lines (not segments) intersect using basic algebra.  Then we can check if the point of intersection is on both line segments.  If the lines are the same (have infinitely many intersection solutions), we can skip this second step.

To solve subproblem 2, we need to determine if all the vertices of R1 lie inside R2.  To check if a point lies inside a rectangle, we need a way to see which side a point is relative to a line segment determined by a vector.  Then p lies inside rectangle ABCD if and only if `orientation(p, AB) == orientation(p, BC) == orientation(p, CD) == orientation(p, DA)`.

If we start thinking about how to write this function, we'll see it will actually be easier to write the function signature as `orientation(p1, p2, p3)`, taking the three points as input.  Then we can just compute the cross product of the vectors `p1->p2` and `p2->p3`, recalling that the sign of the cross product gives us the orientation of the vectors (remember your right hand rule).  Note that we could have also used `p1->p3` as the second vector.

---
