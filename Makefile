CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -Iinclude

PAGESIM_TARGET = pagesim
PAGESHELL_TARGET = pageshell
SYSCALL_TARGET = syscall_demo
PROCESS_TARGET = process_demo
SCHEDULER_TARGET = scheduler_demo
IPC_TARGET = ipc_demo
SIGNAL_TARGET = signal_demo
FIFO_TARGET = fifo_demo
JOB_CONTROL_TARGET = job_control_demo
ADDRESS_TRANSLATION_TARGET = address_translation_demo
MEMORY_LAYOUT_TARGET = memory_layout_demo
COW_TARGET = cow_demo
ALLOCATION_TARGET = allocation_demo
MMAP_TARGET = mmap_demo
MEMORY_PROTECTION_TARGET = memory_protection_demo
PAGE_FAULT_ANALYSIS_TARGET = page_fault_analysis_demo
PAGE_TABLE_TARGET = page_table_demo

PAGESIM_SOURCES = src/main.c \
                  src/memory.c \
                  src/replacement.c \
                  src/tlb.c

PAGESHELL_SOURCES = src/shell_main.c \
                    src/shell.c

SYSCALL_SOURCES = src/syscall_demo.c
PROCESS_SOURCES = src/process_demo.c

SCHEDULER_SOURCES = src/scheduler_main.c \
                    src/scheduler.c

IPC_SOURCES = src/ipc_demo_main.c \
              src/ipc_demo.c

SIGNAL_SOURCES = src/signal_demo_main.c \
                 src/signal_demo.c

FIFO_SOURCES = src/fifo_demo_main.c \
               src/fifo_demo.c

JOB_CONTROL_SOURCES = src/job_control_demo_main.c \
                      src/job_control_demo.c

ADDRESS_TRANSLATION_SOURCES = src/address_translation_main.c \
                              src/address_translation.c \
                              src/memory.c \
                              src/replacement.c \
                              src/tlb.c

MEMORY_LAYOUT_SOURCES = src/memory_layout_main.c \
                        src/memory_layout.c

COW_SOURCES = src/cow_demo_main.c \
              src/cow_demo.c

ALLOCATION_SOURCES = src/allocation_demo_main.c \
                     src/allocation_demo.c

MEMORY_PROTECTION_SOURCES = src/memory_protection_main.c \
                            src/memory_protection.c

PAGE_FAULT_ANALYSIS_SOURCES = src/page_fault_analysis_main.c \
                              src/page_fault_analysis.c \
                              src/memory.c \
                              src/replacement.c \
                              src/tlb.c

PAGE_TABLE_SOURCES = src/page_table_demo_main.c \
                     src/page_table_demo.c \
                     src/memory.c \
                     src/replacement.c \
                     src/tlb.c

MMAP_SOURCES = src/mmap_demo_main.c \
               src/mmap_demo.c

all: $(PAGESIM_TARGET) $(PAGESHELL_TARGET) $(SYSCALL_TARGET) $(PROCESS_TARGET) $(SCHEDULER_TARGET) $(IPC_TARGET) $(SIGNAL_TARGET) $(FIFO_TARGET) $(JOB_CONTROL_TARGET) $(ADDRESS_TRANSLATION_TARGET) $(MEMORY_LAYOUT_TARGET) $(COW_TARGET) $(ALLOCATION_TARGET) $(MMAP_TARGET) $(MEMORY_PROTECTION_TARGET) $(PAGE_FAULT_ANALYSIS_TARGET) $(PAGE_TABLE_TARGET)

$(PAGESIM_TARGET): $(PAGESIM_SOURCES)
	$(CC) $(CFLAGS) $(PAGESIM_SOURCES) -o $(PAGESIM_TARGET)

$(PAGESHELL_TARGET): $(PAGESHELL_SOURCES)
	$(CC) $(CFLAGS) $(PAGESHELL_SOURCES) -o $(PAGESHELL_TARGET)

$(SYSCALL_TARGET): $(SYSCALL_SOURCES)
	$(CC) $(CFLAGS) $(SYSCALL_SOURCES) -o $(SYSCALL_TARGET)

$(PROCESS_TARGET): $(PROCESS_SOURCES)
	$(CC) $(CFLAGS) $(PROCESS_SOURCES) -o $(PROCESS_TARGET)

$(SCHEDULER_TARGET): $(SCHEDULER_SOURCES)
	$(CC) $(CFLAGS) $(SCHEDULER_SOURCES) -o $(SCHEDULER_TARGET)

$(IPC_TARGET): $(IPC_SOURCES)
	$(CC) $(CFLAGS) $(IPC_SOURCES) -o $(IPC_TARGET)

$(SIGNAL_TARGET): $(SIGNAL_SOURCES)
	$(CC) $(CFLAGS) $(SIGNAL_SOURCES) -o $(SIGNAL_TARGET)

$(FIFO_TARGET): $(FIFO_SOURCES)
	$(CC) $(CFLAGS) $(FIFO_SOURCES) -o $(FIFO_TARGET)

$(JOB_CONTROL_TARGET): $(JOB_CONTROL_SOURCES)
	$(CC) $(CFLAGS) $(JOB_CONTROL_SOURCES) -o $(JOB_CONTROL_TARGET)

$(ADDRESS_TRANSLATION_TARGET): $(ADDRESS_TRANSLATION_SOURCES)
	$(CC) $(CFLAGS) $(ADDRESS_TRANSLATION_SOURCES) -o $(ADDRESS_TRANSLATION_TARGET)

$(MEMORY_LAYOUT_TARGET): $(MEMORY_LAYOUT_SOURCES)
	$(CC) $(CFLAGS) $(MEMORY_LAYOUT_SOURCES) -o $(MEMORY_LAYOUT_TARGET)

$(COW_TARGET): $(COW_SOURCES)
	$(CC) $(CFLAGS) $(COW_SOURCES) -o $(COW_TARGET)

$(ALLOCATION_TARGET): $(ALLOCATION_SOURCES)
	$(CC) $(CFLAGS) $(ALLOCATION_SOURCES) -o $(ALLOCATION_TARGET)

$(MEMORY_PROTECTION_TARGET): $(MEMORY_PROTECTION_SOURCES)
	$(CC) $(CFLAGS) $(MEMORY_PROTECTION_SOURCES) -o $(MEMORY_PROTECTION_TARGET)

$(PAGE_FAULT_ANALYSIS_TARGET): $(PAGE_FAULT_ANALYSIS_SOURCES)
	$(CC) $(CFLAGS) $(PAGE_FAULT_ANALYSIS_SOURCES) -o $(PAGE_FAULT_ANALYSIS_TARGET)

$(PAGE_TABLE_TARGET): $(PAGE_TABLE_SOURCES)
	$(CC) $(CFLAGS) $(PAGE_TABLE_SOURCES) -o $(PAGE_TABLE_TARGET)



$(MMAP_TARGET): $(MMAP_SOURCES)
	$(CC) $(CFLAGS) $(MMAP_SOURCES) -o $(MMAP_TARGET)

run: $(PAGESIM_TARGET)
	./$(PAGESIM_TARGET)

shell: $(PAGESHELL_TARGET)
	./$(PAGESHELL_TARGET)

syscall-demo: $(SYSCALL_TARGET)
	./$(SYSCALL_TARGET)

process-demo: $(PROCESS_TARGET)
	./$(PROCESS_TARGET)

scheduler: $(SCHEDULER_TARGET)
	./$(SCHEDULER_TARGET)

ipc-demo: $(IPC_TARGET)
	./$(IPC_TARGET)

signal-demo: $(SIGNAL_TARGET)
	./$(SIGNAL_TARGET)

fifo-demo: $(FIFO_TARGET)
	./$(FIFO_TARGET)

job-control-demo: $(JOB_CONTROL_TARGET)
	./$(JOB_CONTROL_TARGET)

address-translation: $(ADDRESS_TRANSLATION_TARGET)
	./$(ADDRESS_TRANSLATION_TARGET)

memory-layout: $(MEMORY_LAYOUT_TARGET)
	./$(MEMORY_LAYOUT_TARGET)

cow-demo: $(COW_TARGET)
	./$(COW_TARGET)

allocation-demo: $(ALLOCATION_TARGET)
	./$(ALLOCATION_TARGET)

memory-protection: $(MEMORY_PROTECTION_TARGET)
	./$(MEMORY_PROTECTION_TARGET)

page-fault-analysis: $(PAGE_FAULT_ANALYSIS_TARGET)
	./$(PAGE_FAULT_ANALYSIS_TARGET)

page-table-demo: $(PAGE_TABLE_TARGET)
	./$(PAGE_TABLE_TARGET)



mmap-demo: $(MMAP_TARGET)
	./$(MMAP_TARGET)

test: $(PAGESIM_TARGET) $(ADDRESS_TRANSLATION_TARGET)
	./tests/test_pagesim.sh
	./tests/test_address_translation.sh

valgrind: $(PAGESIM_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(PAGESIM_TARGET)

clean:
	rm -f $(PAGESIM_TARGET) $(PAGESHELL_TARGET) $(SYSCALL_TARGET) $(PROCESS_TARGET) $(SCHEDULER_TARGET) $(IPC_TARGET) $(SIGNAL_TARGET) $(FIFO_TARGET) $(JOB_CONTROL_TARGET) $(ADDRESS_TRANSLATION_TARGET) $(MEMORY_LAYOUT_TARGET) $(COW_TARGET) $(ALLOCATION_TARGET) $(MMAP_TARGET) $(MEMORY_PROTECTION_TARGET) $(PAGE_FAULT_ANALYSIS_TARGET) $(PAGE_TABLE_TARGET) filesystem_demo thread_sync_demo

.PHONY: all run shell syscall-demo process-demo scheduler ipc-demo signal-demo fifo-demo job-control-demo address-translation memory-layout cow-demo test valgrind clean allocation-demo mmap-demo memory-protection page-fault-analysis page-table-demo filesystem-demo thread-sync-demo


filesystem_demo: src/filesystem_demo.c
	$(CC) $(CFLAGS) src/filesystem_demo.c -o filesystem_demo

filesystem-demo: filesystem_demo
	./filesystem_demo

thread_sync_demo: src/thread_sync_demo.c
	$(CC) $(CFLAGS) -pthread src/thread_sync_demo.c -o thread_sync_demo

thread-sync-demo: thread_sync_demo
	./thread_sync_demo
