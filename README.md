# OS-project-3

## Project Overview

This project demonstrates synchronization using producers and consumers and semaphores for synchronization. The program uses a circular buffer with size 15 to share the data between two threads, a producer and a consumer. This program emphasizes the importance of synchronization in concurrent programming by using semaphores to manage buffer access.

## Requirements:
* This program must be run on a linux system
* All files must be in the same directory
* Libraries:
     * POSIX threads and shared memory
     * Semaphores
     * Standard input and output library

## Compilation Instructions
* Install C compiler if it is not already installed on your device - use the following command: 
  sudo apt install gcc
* On the terminal, navaigate to the folder containing your code by using the cd command to move     between directories
* Compile the program using gcc name_of_program.c -lpthread -lrt -o executableFilename

## Execution Instructions
* Run the executable using the terminal using the following command:
    * ./ExecutableFileName
* The program should print the contents of "mytest.dat"

  ![image](https://github.com/user-attachments/assets/4b83b0cc-86b2-462e-a43a-fd5dddf25246)

## Program Explanation

### Circular Buffer
*  The buffer is a shared resource with 15 slots.
*  The producer writes characters to the buffer and the consumer reads them.
*  Buffer indices, in and out, are managed in a circular manner to prevent overflow.

### Semaphores
* Empty: tracks available slots in the buffer
* Full: Tracks the number of slots available filled with data
* mutex: Ensures mutual exclusion when accessing the buffer

### How it works
* The producer read characters from **mytest.dat** and writes them into the buffer until the end of file.
* The special character ** "*" **, signals the end of input
* The consumer reads characters from the buffer and prints them, stopping when it reaches ** "*" **.

## Known Limitations
* Buffer size- it is limited to 15
* The consumer has a 1 second delay
