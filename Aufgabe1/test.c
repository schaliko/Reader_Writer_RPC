#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

// Semaphore-Operationen
void P(int sem_id) {
    struct sembuf sop = {0, -1, 0}; // Down-Operation
    semop(sem_id, &sop, 1);
}

void V(int sem_id) {
    struct sembuf sop = {0, 1, 0}; // Up-Operation
    semop(sem_id, &sop, 1);
}

void reader_process(int id, int mutex, int writer, int reader) {
    for (int i = 0; i < 3; i++) {
        P(mutex); // Sichere Zugriff auf READER-Zähler
        int reader_count = semctl(reader, 0, GETVAL, 0);
        semctl(reader, 0, SETVAL, reader_count + 1);
        if (reader_count == 0) {
            P(writer); // Erster Leser blockiert Schreibzugriff
        }
        V(mutex);

        // Kritischer Bereich
        printf("Reader %d: Reading shared resource\n", id);
        sleep(1);

        P(mutex); // Zugriff auf READER-Zähler
        reader_count = semctl(reader, 0, GETVAL, 0);
        semctl(reader, 0, SETVAL, reader_count - 1);
        if (reader_count == 1) {
            V(writer); // Letzter Leser gibt Schreibzugriff frei
        }
        V(mutex);

        // Unkritischer Bereich
        sleep(1);
    }
}

void writer_process(int id, int writer) {
    for (int i = 0; i < 3; i++) {
        P(writer); // Blockiere Leser und andere Schreiber
        // Kritischer Bereich
        printf("Writer %d: Writing to shared resource\n", id);
        sleep(1);
        V(writer); // Gib Zugriff frei
        // Unkritischer Bereich
        sleep(1);
    }
}

int main() {
    if (key_t sem_key = ftok("./semfile.txt", '1')) < 0) {
        perror("Error in ftok");
        exit(1);
    }

    // Semaphore erstellen
    int writer = semget(sem_key, 1, IPC_CREAT | 0666);
    int reader = semget(sem_key, 1, IPC_CREAT | 0666);
    int mutex = semget(sem_key, 1, IPC_CREAT | 0666);

    // Semaphore initialisieren
    semctl(writer, 0, SETVAL, 1); // Schreiber darf starten
    semctl(reader, 0, SETVAL, 0); // Keine Leser zu Beginn
    semctl(mutex, 0, SETVAL, 1);  // MUTEX ist frei

    pid_t pid;
    int i;

    // Erstelle 5 Leserprozesse
    for (i = 0; i < 5; i++) {
        pid = fork();
        if (pid == 0) {
            reader_process(i + 1, mutex, writer, reader);
            return 0;
        }
    }

    // Erstelle 2 Schreibprozesse
    for (i = 0; i < 2; i++) {
        pid = fork();
        if (pid == 0) {
            writer_process(i + 1, writer);
            return 0;
        }
    }

    // Warte auf alle Kindprozesse
    for (i = 0; i < 7; i++) {
        wait(NULL);
    }

    // Semaphore löschen
    semctl(writer, 0, IPC_RMID, 0);
    semctl(reader, 0, IPC_RMID, 0);
    semctl(mutex, 0, IPC_RMID, 0);

    return 0;
}
