#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <err.h>

int main(int argc, char** argv)
{
    int opt;
    while((opt = getopt(argc, argv,"abf:" ))!=-1)
    {
        switch (opt)
        {
            case 'a':
            case 'b':
            {
                printf("%c ", opt);
                fflush(stdout);
                break;
            }
            case 'f':
            {
                printf("%c %s", opt, optarg);
                fflush(stdout);
                break; 
            }
            case '?':
            {
                errx(1, "invalid option");
                break;
            }
        }
    }
}