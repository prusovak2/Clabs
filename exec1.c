#include <unistd.h>

int main()
{
	execl("/usr/bin/vim", "vim", "/etc/passwd", NULL);
}
