#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

void P(int sem_id) {
    struct sembuf ops;   

    ops.sem_num = 0; 
    ops.sem_op = -1;
    ops.sem_flg = 0;

    if (semop(sem_id, &ops, 1) < 0) {
        perror("Error in atomic P() operation for picking up forks");
        exit(1);
    }
}

void V(int sem_id) {
    struct sembuf ops;   

    ops.sem_num = 0; 
    ops.sem_op = 1;
    ops.sem_flg = 0;
    if (semop(sem_id, &ops, 1) < 0) {
        perror("Error in atomic V() operation for picking up forks");
        exit(1);
    }
}

void reader_process(int id, int mutex, int writer, int reader){
    for (int i = 0; i < 3; i++) {
        P(mutex); // Sichere Zugriff auf READER-Zähler
        int reader_count = semctl(reader, 0, GETVAL, 0);
        semctl(reader, 0, SETVAL, reader_count + 1);
        if (semctl(reader, 0, GETVAL, 0) == 1) {
            P(writer); // Erster Leser blockiert Schreibzugriff
        }
        V(mutex);


        printf("Reader %d: Reading shared resource\n", id);
        sleep(1); // Simulate critical section

        P(mutex); // Zugriff auf READER-Zähler
        reader_count = semctl(reader, 0, GETVAL, 0);
        semctl(reader, 0, SETVAL, reader_count - 1);
        // reader_count--;
        printf("Reader %d: Finished reading\n", id);
    	if (semctl(reader, 0, GETVAL, 0) == 0) {
            V(writer); // Letzter Leser gibt Schreibzugriff frei
        }
        V(mutex);
        sleep(1); // Simulate non-critical section
    }
}

void writer_process(int id, int writer) {
    for (int i = 0; i < 3; i++) {
        P(writer); // Blockiere Leser und andere Schreiber
        printf("Writer %d: Writing to shared resource\n", id);
        sleep(1); // Simulate critical section
        printf("Writer %d: Finished writing\n", id);
        V(writer); // Gib Zugriff frei
        sleep(1); // Simulate non-critical section
    }
}

int main() {
    pid_t pid;
    key_t writer_key;
    key_t reader_key;
    key_t mutex_key;

    if ((writer_key = ftok("./semfile.txt", 'W')) < 0) {
        perror("Error in ftok");
        exit(1);
    }

    if ((reader_key = ftok("./semfile.txt", 'R')) < 0) {
        perror("Error in ftok");
        exit(1);
    }
    
    if ((mutex_key = ftok("./semfile.txt", 'M')) < 0) {
        perror("Error in ftok");
        exit(1);
    }
    // Semaphore erstellen
    int writer = semget(writer_key, 1, IPC_CREAT | 0666);
    int reader = semget(reader_key, 1, IPC_CREAT | 0666);
    int mutex = semget(mutex_key, 1, IPC_CREAT | 0666);


    if (writer < 0 || reader < 0 || mutex < 0) {
        perror("Error creating semaphores");
        exit(1);
    }

    // Semaphore initialisieren
    semctl(writer, 0, SETVAL, 1); // Schreiber darf starten
    semctl(reader, 0, SETVAL, 0); // Keine Leser zu Beginn
    semctl(mutex, 0, SETVAL, 1);  // MUTEX ist frei

    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) {
            reader_process(i + 1, mutex, writer, reader);
            return 0;
        }
    }

    for (int i = 0; i < 2; i++) {
        pid = fork();
        if (pid == 0) {
            writer_process(i + 1, writer);
            return 0;
        }
    }


    for (int i = 0; i < 7; i++) { 
        wait(NULL);
    }


    return 0;
}
