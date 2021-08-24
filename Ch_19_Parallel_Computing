## Chapter 19 - Parallel Computing - Variants Solutions

---

**Threads 1 to n execute a method called `critical`, which can only be executed by one thread at a time.  All threads must have completed executing a function called `rendevouz` before any thread may execute `critical`.  Threads may call `critical` multiple times, but you must ensure all threads complete their kth `critical` execution before any thread executes its (k+1)th call to `critical`.  Design a synchronization mechanism for the threads satisfying these constraints.**

The first thing we need to track is whether or not all threads have completed `rendevouz`.  You could do this by storing a boolean value `rendevouz_complete` for each thread, but a more efficient way would just be to store a count of the number of threads that have completed `rendevouz` (this allows O(1) checking of whether all threads have completed `rendevouz` rather than O(n)).  A thread increments this count when it completes `rendevouz` (if `rendevouz` may be called multiple times per thread, only increment on first `rendevouz` execution).  This increment needs to be protected by a `lock_guard/mutex` pair, as multiple threads may be executing `rendevouz` simultaneously.

Secondly, we need to make sure all threads complete their kth `critical` execution before any thread executes its (k+1)th call to `critical`.  We can do this by keeping a `std::unordered_set` of threads (represented by thread id) that still need to complete their kth `critical` call, initialized to contain all thread ids.  When a thead calls `critical`, we remove it from the set, and once the set is empty we reset to contain all thread ids again.  This once again ensures an amortized O(1) runtime overhead for ensuring the right ordering of `critical` calls, whereas something like a count value for each thread would require O(n) time to check if there are no counts less than the current thread's count.

(Instead of a set, you could also store a count of threads yet to complete kth `critical` call, the current value of k, and the number of `critical` calls per function.  If a function'n number of critical calls is less than k, run `critical` and decrement the count.  Either way, we're storing one integer per thread, so the overhead is the same.  And to me, the logic of using a set is cleaner.)

---

**Implement a synchronization mechanism for the third readers-writers problem: neither the readers nor the writers may starve.**

There may exist an elegant canonical solution to this problem, but the simple solution that came to my mind was this: if we just allow threads to run based on the order in which they were spawned, there is no starvation.  But now where is the parallelization you ask? Well, multiple readers can read together.

So if the order of function calls was: `R1, W2, R3, R4, R5, W6, W7, R8`, first R1 would run and W2 would block since it's a writer; everything after W2 would also be blocked.  Then W2 would run, mark the data as being written, which would block everything else.  Next, we would have R3, which would run and mark the data as being read from.  R4 and R5 would also be able to run, since they are readers as well. W6 (and everything after) would block.  Next W6 would write and W7+ would block.  And so on...

In order to implement this, we need keep track of our threads in a queue, storing the type of each thread (reader or writer).  Writers can only be unblocked if they're at the head of the queue, while readers can be unblocked if there are no writers ahead of them in the queue.

---
