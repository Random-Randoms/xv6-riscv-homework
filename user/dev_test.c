#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
    int null = open("dev_null", O_RDWR);
    int zero = open("dev_zero", O_RDWR);
    int urnd = open("dev_urandom", O_RDWR);
    int nstt = open("dev_nullstat", O_RDWR);

    char buf[128];
    uint64 nstt_ret = 0;

    if (null < 0 || zero < 0 || urnd < 0 || nstt < 0) fprintf(2, "one of device files has not been opened\n"), exit(-1);

    printf("write to null, bytes written: %d, consumed: %d\n", 6, write(null, "hello", 6));
    printf("try to read from null, bytes arrived: %d\n", read(null, buf, 6));

    printf("\nwrite to zero, should get a mistake, bytes consumed: %d\n", write(zero, "hello", 6));
    read(zero, buf, 4);
    printf("read 4 bytes from zero, should be all balls: %d %d %d %d\n", buf[0], buf[1], buf[2], buf[3]);

    printf("\nread 4 bytes from urandom, bytes arrived: %d, they are: %d %d %d %d\n", read(urnd, buf, 4), buf[0], buf[1], buf[2], buf[3]);
    printf("write not exactly 1 byte to urandom, get error: %d\n", write(urnd, "hello", 6));
    printf("write exactly 1 byte to urandom, they are consumed: %d\n", write(urnd, "hello", 1));
    printf("get new %d bytes with different seed: %d %d %d %d\n", read(urnd, buf, 4), buf[0], buf[1], buf[2], buf[3]);

    printf("\nwrite %d bytes to nullstat, they are consumed: %d\n", 6, write(nstt, "hello", 6));
    printf("read not exactly 8 bytes from nullstat, get error: %d\n", read(nstt, buf, 6));
    printf("read exactly 8 bytes from nullstat, %d bytes arrived\n", read(nstt, &nstt_ret, 8));
    printf("bytes collected by nullstat: %ld\n", nstt_ret);

    close(null);
    close(zero);
    close(urnd);
    close(nstt);
}