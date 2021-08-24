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

---

