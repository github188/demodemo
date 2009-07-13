#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	printf("Content-type: text/html; charset=gb2312\n\n");
	printf("<html><head><title>hello world CGI</title></head>");
	printf("<body><center>Hello alang!</body></html>");
	return 0;
}

