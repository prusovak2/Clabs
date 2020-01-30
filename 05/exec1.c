#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() 
{
execl("/bin/vim", "vim", "/ect/passwd", NULL); 
}