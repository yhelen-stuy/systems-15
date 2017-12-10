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

int main( int argc, const char *argv[] ) {
    int semdescriptor;
    int shmdescriptor;
    int val;
    int semkey;
    int shmkey;

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short * array;
    } arg;

    if (argc < 2) {
        printf("uhhhhh arguments please?\n");
        return 0;
    }
    //uses the dir and an int to create a unique key
    semkey= ftok("./", 'B');
    shmkey= ftok("./", 'A');

    if (!strcmp(argv[1], "-c")) {
        //else
        //returns id of reated semaphore with the same key
        semdescriptor = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0644);
        if (semdescriptor == -1) {
            fprintf(stderr,"UH RROR: %s\n", strerror(errno));
            return 0;
        }
        arg.val = 1;
        val = semctl(semdescriptor, 0, SETVAL, arg.val);
        if (val == -1) {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));
            return 0;
        }
        printf("Sema FOR created: %d\n",semdescriptor);

        shmdescriptor = shmget(shmkey, sizeof(int), IPC_CREAT | IPC_EXCL | 0600);
        if (val == -1) {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));
            return 0;
        }
        printf("Shmem allocated: %d\n", val);

        int f = open("story.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
        printf("story file created\n");
        close(f);
    }

    else if (!strcmp(argv[1], "-v")){
        char story[5000];

        struct stat sb;
        stat("story.txt", &sb);

        int f = open("story.txt", O_RDONLY, 0644);
        if (f == -1) {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));
            return 0;
        }
        read(f, &story, sb.st_size);

        printf("Story: \n");
        printf("%s\n", story);

        close(f);
    }

    else if (!strcmp(argv[1], "-r")) {
        semdescriptor = semget(semkey, 0, 0);
        val = semctl(semdescriptor, 0,  IPC_RMID);
        if (val == -1) {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));
            return 0;
        }
        printf("RE SEM OVED: %d\n", val); 

        shmdescriptor = shmget(shmkey, sizeof(char) * 256, 0);
        val = shmctl(shmdescriptor, IPC_RMID, 0);
        printf("Shared memory removed\n");

        char story[5000];

        struct stat sb;
        stat("story.txt", &sb);

        int f = open("story.txt", O_RDONLY, 0644);
        read(f, &story, sb.st_size);

        printf("\n\nStory: \n");
        printf("%s\n", story);

        close(f);
    }
    else {
        printf("Winvalid command!\n");
    }

    return 0;
}
