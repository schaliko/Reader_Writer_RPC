#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>


void reader_process(int id){
    for (int i = 0; i < 3; i++) {
        printf("Reader %d: Accessing shared resource (unsynchronized)\n", id);
        sleep(1); // Simulate critical section
        printf("Reader %d: Finished reading\n", id);
        sleep(1); // Simulate non-critical section
    }
}

void writer_process(int id) {
    for (int i = 0; i < 3; i++) {
        printf("Writer %d: Writing to shared resource (unsynchronized)\n", id);
        sleep(1); // Simulate critical section
        printf("Writer %d: Finished writing\n", id);
        sleep(1); // Simulate non-critical section
    }
}

int main() {
    pid_t pid;

    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) {
            reader_process(i + 1);
            return 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        pid = fork();
        if (pid == 0) {
            writer_process(i + 1);
            return 0;
        }
    }


    for (int i = 0; i < 5; i++) { 
        wait(NULL);
    }


    return 0;
}