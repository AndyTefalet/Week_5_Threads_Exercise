### Task 3

**Result:**

Running the program outputs a number lower than 20,000 (around ~11,000–15,000) instead of the expected 20,000.

**Why it happens:**

This is a classic **race condition**. Both threads access the shared variable `bar` and run `baz()` at the same time without any synchronization or locks.

Even though `bar++` looks like a single command, under the hood it takes three separate CPU instructions:

1. **Read:** Load `bar` into a register.
2. **Modify:** Add 1 to it.
3. **Write:** Save the new value back to memory.

Because the operations aren't atomic, the threads step on each other's toes. For example:

* Thread A and Thread B both read `bar = 500` at the same moment.
* Both calculate `500 + 1 = 501`.
* Both write `501` back.

Even though two increments happened, the counter only went up by 1. These overlapping writes cause "lost updates," leaving the final count well below 20,000.

### Task 4

**Result:**

Running the program outputs exactly 20,000 every single time.

**Why it works:**

Adding `synchronized` to `baz()` and `getBar()` fixes the race condition by using Java's built-in object lock (monitor).

Now, only one thread can run `baz()` on the `Foo` instance at a time:

* When Thread A enters `baz()`, it grabs the lock.
* If Thread B tries to enter while Thread A is inside, it gets blocked and has to wait in line.
* Once Thread A finishes the `bar++` operation and leaves, it releases the lock, allowing Thread B in.

This ensures all three steps of `bar++` (read, modify, write) execute safely without interruptions, so no increments get lost.

### Task 5

**Result:**

Running the program outputs exactly 20,000.

**Why it works:**

Instead of synchronizing the whole method, we wrapped just `bar++` in a `synchronized(this)` block.

Under the hood, both approaches lock the exact same thing (the current object instance `this`). In our small example, the behavior is pretty much identical since the method only contains one line.

However, using a synchronized block gives us much better control:

* **Finer granularity:** In a real project with larger methods, we don't want to lock everything. We only lock the specific line (critical section) that touches shared memory.
* **Better performance:** The rest of the method can still run in parallel across multiple threads without forcing them to wait in line unnecessarily, which prevents major bottlenecks.

### Task 6

**Result:**

The output prints a completely corrupted number (around ~23M instead of the expected 100,000,000), but runs super fast in just about ~20–30 milliseconds.

**Why it happens:**

Without any locks or `synchronized` keywords, we get a massive **race condition**. All 10 threads are slamming `bar++` simultaneously, constantly overwriting each other's work and losing tens of millions of increments.

So why is it so fast?

* **Zero overhead:** The threads never block, queue up, or wait on a lock.
* **Full parallelism:** All CPU cores run at full throttle without any synchronization slowdowns.

This is the classic multithreading trade-off in action: skipping locks gives you blazing-fast speed, but your shared data gets completely trashed.

### Task 7

**Result:**

The output hits the exact 100,000,000 mark as expected, but it takes way longer to finish (around ~13–14 seconds).

**Why it happens:**

Wrapping `bar++` back inside a `synchronized(this)` block makes the code 100% thread-safe. The race condition is completely gone, and not a single increment is dropped.

However, thread safety isn't free:

* **Heavy contention:** All 10 threads are fighting over a single lock. Instead of running in parallel, they spend most of their time blocked, waiting in line for their turn.
* **Context switching overhead:** Constantly locking, unlocking, and waking up waiting threads burns massive CPU time.

It's the other side of the Task 6 trade-off: our data is completely accurate, but serializing access slows execution down drastically.

This is the week 8 assignment repository.Link to repo:
https://github.com/AndyTefalet/ Week_5_Threads_Exercise
