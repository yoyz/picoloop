#include <stdio.h>
#include <stdlib.h>

void main()
{
srandom(getpid());
printf("%ld\n",random());
printf("%ld\n",random());
printf("%ld\n",random());
printf("%ld\n",random());
}
