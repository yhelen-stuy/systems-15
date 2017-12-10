#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    int semkey= ftok("./", 'B');
    int semdescriptor = semget(semkey, 1, 0644);
    if (semdescriptor == -1) {
        fprintf(stderr,"UH RROR: %s\n", strerror(errno));
        return 1;
    }
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = SEM_UNDO;
    semop(semdescriptor, &op, 1);

    int shmkey= ftok("./", 'A');
    int shmdescriptor = shmget(shmkey, sizeof(int), 0600);
    if (shmdescriptor == -1) {
        fprintf(stderr,"E RROR: %s\n", strerror(errno));
        return 1;
    }

    int *length = (int *) shmat(shmdescriptor, 0, 0);

    int f = open("story.txt", O_RDWR | O_APPEND, 0644);
    if (f == -1) {
        fprintf(stderr,"ERROR: %s\n", strerror(errno));
        return 1;
    }

    lseek(f, -(*length), SEEK_END);

    char *buffer = (char *)malloc(*length + 1);
    read(f, buffer, *length);
    printf("Last contribution:\n");
    printf("%s\n", buffer);
    printf("What would you like to add?\n");

    char line[256];
    fgets(line, sizeof(line), stdin);

    write(f, line, strlen(line));
    close(f);

    *length = strlen(line);
    shmdt(length);

    op.sem_op = 1;
    semop(semdescriptor, &op, 1);

    return 0;
}
