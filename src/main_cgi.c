/* main_cgi.c */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *data;
	printf("Content-Type: text/plain;charset=us-ascii\n\n");
	data = getenv("QUERY_STRING");
	if ( data == NULL ) printf("<p>null input</p>\n\n");
	else printf("you sent: %s\n\n", data);
	return 0;
}
