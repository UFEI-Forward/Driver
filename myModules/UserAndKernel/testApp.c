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

    char buf[30];
    int ret = 0;
    ret = read(fd, buf, sizeof(buf));
    if(ret == -1) {
        printf("Failed in reading file!\n");
        return -1;
    }
    printf("%s\n", buf);

    char buf2[] = "I'm the info from user space!";
    ret = write(fd, buf2, sizeof(buf2));
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
