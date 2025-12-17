# CLI To-Do List Manager

This project is a robust and memory-safe implementation of a command-line to-do list manager written in **C**. It supports dynamic task creation, priority-based sorting, and file persistence.

## Features
* **Create and Manage**: Create, read, and modify lists stored in local `.txt` files.
* **Priority Sorting**: Sorts tasks based on priority level (1-2-3) through a **Quick Sort** algorithm.
* **Data Persistence**: Automatically saves state of to-do list to CSV-formatted `.txt` files.
* **Robust Input Handling**: Safely handles user input to avoid buffer overflows.

## Technical Implementation
* **Language**: C
* **Memory Management**: Implements **Dynamic Memory Allocation** (`malloc`/`realloc`/`free`) via geometric resizing for indefinite list scaling.
* **Algorithms**: Implements **Quick Sort** ($O(n \log n)$) for task prioritization. 
* **Safety**: Executed with **Valgrind** to ensure zero memory leaks.

## Getting Started
### Dependencies
* **GCC Compiler**: Must have a C compiler, like GCC, installed.
* **Make**: Optionally, have Make utility installed for easier compile.

### Installing
Clone the repository and compile the source code:

```bash
git clone https://github.com/mcmullenleo/cli_task_manager.git
cd cli_task_manager

make
# Or, if compiling manually
gcc main.c -o todo
```
### Executing Program

```bash
gcc --version
# If installed, run the executable
./todo
```
