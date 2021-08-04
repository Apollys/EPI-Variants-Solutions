## Chapter 6 - Strings - Variants Solutions

---

**Alphabet encoding (spreadsheet column id) to integer - "A" corresponds to 0**

This is just pure base 26 to base 10 conversion now.  You can just get rid of the '+1' in the example code, or write your own version if you don't like the cryptic lambda accumulate version (I'm not particularly fond of it, I think a normal for loop would be much more readable).

---

**Integer to alphabet encoding (spreadsheet column id)**

Once again, this is just base conversion, now the other way: base 10 to base 26.  And the off-by-one thing is back.

---

**Telex encoding**

Again, this is just a one-trick question where you have to realize to go backwards from the end.  Not really any different from the previous example.

---

**Roman numeral questions**

These two questions just involve turning the rules for roman numerals into program code, there's not really much space for thought or algorithm design.  Search the rules for roman numerals online if you're not comfortable with them.

---

**IP addresses varaint - placing k periods in a string of arbitrary length.**

From what I understand, the rules here are that we are given a string and a number k, and we must compute all possible ways to insert k periods into that string, under the condition that periods may never be adjacent characters, or first/last characters.

We would handle this recursively, first selecting where to place the first period, then recursing on k-1 periods and the string that starts after the period we just inserted.  We would not actually copy the substring to pass recursively, but just use a start index to prevent excess space complexity.

These type of "list all the ways to do X" problems rarely have any room for optimization beyond brute force, because we can't shortcut by cleverly finding one solution, and we can't shortcut by computing the number of ways mathematically without actually enumerating them.  So time and space complexity are forced to be at least O(number of ways), i.e. the brute force solution.

---
