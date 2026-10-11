# PageSim Algorithms

## 1. Overview

PageSim implements four page replacement algorithms:

1. FIFO
2. LRU
3. Optimal
4. Clock / Second Chance

These algorithms are used when physical memory is full and a new page must be loaded.

---

# 2. FIFO — First In First Out

## Concept

FIFO replaces the page that has been in memory for the longest time.

The algorithm maintains a pointer to the next frame that should be replaced.

## Working

Suppose there are three frames:

```text
Frame 0
Frame 1
Frame 2
