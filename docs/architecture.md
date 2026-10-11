# PageSim Architecture

## 1. Overview

PageSim is a Linux-based Operating Systems simulator written in C.

The project simulates:

- Virtual memory
- Demand paging
- Page tables
- Physical memory
- Page faults
- Page replacement
- Translation Lookaside Buffer (TLB)

The simulator currently supports four page replacement algorithms:

1. FIFO
2. LRU
3. Optimal
4. Clock / Second Chance

---

## 2. System Architecture

The main memory-translation flow is:

CPU
 |
 v
Virtual Page
 |
 v
TLB
 |
 +------------------+
 |                  |
TLB HIT          TLB MISS
 |                  |
 v                  v
Frame           Page Table
                    |
              +-----+-----+
              |           |
           PRESENT      NOT PRESENT
              |           |
              |       PAGE FAULT
              |           |
              |    Page Replacement
              |           |
              +-----+-----+
                    |
                    v
             Physical Memory
                    |
                    v
                  Frame

---

## 3. Major Modules

### 3.1 Main Module

File:

`src/main.c`

Responsibilities:

- Start the simulator
- Display project information
- Allow the user to select a replacement algorithm
- Define the page reference string
- Start the simulation
- Display results

---

### 3.2 Memory Module

Files:

- `include/memory.h`
- `src/memory.c`

Responsibilities:

- Manage physical frames
- Maintain the page table
- Detect page hits
- Detect page faults
- Load pages into free frames
- Perform page replacement
- Integrate TLB translation
- Display memory statistics

---

### 3.3 Replacement Module

Files:

- `include/replacement.h`
- `src/replacement.c`

Responsibilities:

- Implement page replacement algorithms
- Maintain replacement state
- Select victim frames

Supported algorithms:

- FIFO
- LRU
- Optimal
- Clock / Second Chance

---

### 3.4 TLB Module

Files:

- `include/tlb.h`
- `src/tlb.c`

Responsibilities:

- Cache virtual-page to physical-frame translations
- Perform TLB lookup
- Record TLB hits
- Record TLB misses
- Insert new translations
- Invalidate stale translations
- Display TLB contents
- Display TLB statistics

The TLB currently contains:

`MAX_TLB_ENTRIES = 4`

---

## 4. Data Structures

### Frame

A physical memory frame contains:

- Frame number
- Page number
- Occupied status

---

### Page Table Entry

Each page table entry contains:

- Page number
- Frame number
- Present bit

The present bit indicates whether the page currently exists in physical memory.

---

### TLB Entry

Each TLB entry contains:

- Virtual page number
- Physical frame number
- Valid bit

---

## 5. Page Access Flow

For every page reference:

### Step 1

The simulator receives a virtual page number.

### Step 2

The TLB is searched.

### Step 3

If the TLB contains a valid mapping:

`Virtual Page -> Physical Frame`

the access is a TLB hit.

### Step 4

If the TLB misses, the page table is checked.

### Step 5

If the page is present in physical memory:

- The page-table mapping is used.
- The TLB is updated.
- The access is treated as a page hit.

### Step 6

If the page is not present:

A page fault occurs.

### Step 7

The simulator searches for a free physical frame.

If a free frame exists, the page is loaded there.

### Step 8

If no free frame exists:

The selected page replacement algorithm chooses a victim frame.

### Step 9

The old page is removed from the page table.

Its TLB entry is invalidated.

### Step 10

The requested page is loaded into the selected frame.

The page table and TLB are updated.

---

## 6. Memory Management

PageSim dynamically allocates:

- Page table
- TLB

Both are released when the simulation finishes.

Valgrind is used to verify memory safety.

---

## 7. Build System

The project uses GNU Make.

Main commands:

```bash
make
