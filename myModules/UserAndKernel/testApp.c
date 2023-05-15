#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int fd = 0;
    fd = open(argv[1], O_RDWR);
    if(fd == -1) {
        printf("Failed in opening file!\n");
        return -1;
    }

    int ret = 0;
    char buf[30];
    ret = read(fd, buf, 30);
    if(ret == -1) {
        printf("Failed in reading file!\n");
        return -1;
    }

    ret = write(fd, buf, 30);
    if(ret == -1) {
        printf("Failed in writing file!\n");
        return -1;
    }

    ret = close(fd);
    if(ret == -1) {
        printf("Failed in closing file!\n");
        return -1;
    }

    return 0;
}
