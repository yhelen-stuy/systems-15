#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main( int argc, const char *argv[] ) {
    int descriptor;
    int val;
    int key;
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
    key= ftok("./", 'B');

    if (!strcmp(argv[1], "-c")) {
        //else
        //returns id of reated semaphore with the same key
        descriptor = semget(key, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
        if (descriptor == -1) {
            fprintf(stderr,"UH RROR: %s\n", strerror(errno));
            return 0;
        }
        arg.val = 1;
        val = semctl(descriptor, 0, SETVAL, arg.val);
        if (val == -1) {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));
            return 0;
        }
        printf("Sema FOR created: %d\n",descriptor);

        }

    }
    else if (!strcmp(argv[1], "-v")){
        descriptor = semget(key, 0, 0);
        val = semctl(descriptor, 0, GETVAL);
        if (val == -1) {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));
            return 0;
        }
        else {
            printf("SEMA VAL: %d\n", val);
        }
        
    }
    else if (!strcmp(argv[1], "-r")) {
        descriptor = semget(key, 0, 0);
        val = semctl(descriptor, 0,  IPC_RMID);
        if (val == -1) {
            fprintf(stderr, "ERROR: %s\n", strerror(errno));
            return 0;
        }
        else {
            printf("RE SEM OVED: %d\n", val); 

        }
    }
    else {
        printf("Winvalid command!\n");
    }
}
