# Systems Programming Assignments

This repository contains several practical C projects developed as part of a **Hardware and Operating Systems** university course.  
The projects demonstrate key system programming concepts, including **process management**, **multithreading**, **file handling**, and **inter-process communication (IPC)**.

## Projects

### 1. User Management System
Manage and verify users in a secured environment using processes and inter-process communication.

- **check_user.c**:  
  - Forks a child process to check if a specific username exists in a file (`users.txt`).
  - Parent process reads the child's exit status and prints whether the user exists.

- **add_user.c**:  
  - Checks if a username already exists.
  - If not, uses `execv` to append the username to the `users.txt` file.

---

### 2. Multithreaded Text Processing
Efficient processing of a large text file using multithreading and synchronization.

- **WordCounterAndFilter.c**:
  - **Task 1:** Count appearances of the words `"was"`, `"is"`, `"this"`, `"the"`, and `"are"`.
  - **Task 2:** Filter out all lines containing the word `"value"` and write the rest to `filtered.txt`.
  - **Technologies:** `pthreads`, mutexes/semaphores for safe updating of shared counters and file writing.
  - **Result:** Final word count is printed and saved into `results.txt`.

---

### 3. Robot Communication in a Smart Factory
Implement a producer-consumer model using pipes and FIFOs for real-time communication.

- **Pipes Communication (Single Program)** - `pipe_robot.c`:
  - Producer reads numbers from the user and sends them via a pipe.
  - Consumer reads the numbers, computes cumulative SUM and AVG, and prints after each new number.

- **FIFOs Communication (Separate Programs)**:
  - **fifo_producer.c**:  
    - Reads numbers from the user and sends them to a FIFO.
  - **fifo_consumer.c**:  
    - Reads numbers from the FIFO, computes cumulative SUM and AVG, and prints after each new number.

---

## Technologies Used
- C
- Linux system calls
- Process management (`fork`, `execv`, `exit`, etc.)
- Inter-process communication (Pipes, FIFOs)
- Multithreading (`pthreads`)
- File handling and synchronization

## Course Context
Assignments completed for the **Hardware and Operating Systems** course, focusing on hands-on system-level programming techniques.

---

