/* name.c */

#include <ctype.h> /* isalpha */

#define NAME_SIZE_MAX 20

static char** name_list;
static unsigned int name_count;

static char* name_new(const char* name)
{
	char* newName = malloc(sizeof (char) * NAME_SIZE_MAX +1);
	assert(newName);
	memset(newName, '\0', NAME_SIZE_MAX +1);
	memcpy(newName, name, NAME_SIZE_MAX); /* truncates long names */
	name_list = realloc(name_list, (name_count +1) * sizeof (char*));
	assert(name_list);
	name_list[name_count] = newName;
	return name_list[name_count++];
}

static int name_delete(const char* name)
{
	unsigned int i;
	for ( i=0; i<name_count; i++ )
		if ( !strcmp(name, name_list[i]) )
		{
			free(name_list[i]);
			for ( ; i<name_count-1; i++ ) 
				name_list[i] = name_list[i+1];
			name_count--;
			return 0; /* list collapsed over name */
		}

	return -1; /* no such name */
}

static int name_check(const char* name)
{
	unsigned int i;
	char* checkName;

	if ( !name || !strcmp(name, "") ) return -4; /* no null or empty */

	for ( i=0; i<name_count; i++ )
		if ( !strcmp(name, name_list[i]) ) return -3; /* name taken */

	checkName = name_new(name);
	if ( strcmp(checkName, name) )
		return -2; /* name was truncated because it was too long */

	name_delete(checkName);

	for ( i=0; i<strlen(name); i++ ) /* name has illegal chars */
		if ( !isalpha(name[i]) && name[i] != 32 ) return -1;

	return 0; /* name is OK to use */
}

/* end of file */
