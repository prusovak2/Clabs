#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    fork();
    fork();
    fork();
    fork();
    sleep(5);
}
