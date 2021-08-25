### Chapter 24 - Honors Class - Selected Problems

---

**Note: I will not be including all problems here, as the book already has writeups of the solutions.  I want to include my own solutions to interesting problems, or simply give my own solution to problems that I think the book overcomplicated in their own explanation.**

---

**Compute the maximum product of all entries but one.**

The book makes this very complicated with all sorts of different algorithms and case analyses.   However, this problem is really quite simple if you have the correct perspective:

Start with the product of all elements except the one with smallest absolute value.  This will be the answer if it's positive, but if it's negative, we need to switch one positive value for one negative value.  In other words, we have two options:
 - All values except the smallest magnitude positive number
 - All values except the smallest magnitude negative number

Simply compute these two products and return the max of them.  Zero can be handled uniformly without any extra code, as long as you consistently treat zero as either positive or negative.

---
