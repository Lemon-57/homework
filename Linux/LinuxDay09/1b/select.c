#include <func.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDWR);
    ERROR_CHECK(fdr, -1, "open fdr");

    int fdw = open(argv[1], O_RDWR);
    ERROR_CHECK(fdr, -1, "open fdw");

    fd_set rdset;
    fd_set wrset;
    int ret;
    char buf[128];
    while (1)
    {
        FD_ZERO(&rdset);
        FD_ZERO(&wrset);
        FD_SET(fdr, &rdset);
        FD_SET(fdw, &wrset);
        ret = select(fdw + 1, &rdset, &wrset, NULL, NULL);
        if (FD_ISSET(fdr, &rdset))
        {
            bzero(buf, sizeof(buf));
            read(fdr, buf, sizeof(buf));
            puts(buf);
        }
        if (FD_ISSET(fdw, &wrset))
        {
            write(fdw, "hello", 3);
        }
    }
    return 0;
}
