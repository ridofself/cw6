/* user.c */

struct user
{
	char* name;
	struct agent_team* team;
};

static struct user** user_list;
static unsigned int user_count;

static struct user* user_new(const char* name)
{
	struct user* newUser;
	if ( name_check(name) ) return NULL;

	newUser = malloc(sizeof (struct user*));
	assert(newUser);
	newUser->name = name_new(name);
	newUser->team = malloc(sizeof (struct agent_team));
	assert(newUser->team);
	user_list = realloc(user_list, 
		(user_count +1) * sizeof (struct user*));
	assert(user_list);
	user_list[user_count] = newUser;
	return user_list[user_count++];
}

static int user_delete(const struct user* user)
{
	unsigned int i;

	if ( !user ) return -1;
	
	for ( i=0; i<user_count; i++ )
		if ( user == user_list[i] )
		{
			free(user_list[i]->team); /* frees users' team */
			name_delete(user_list[i]->name);
			free(user_list[i]);
			for ( ; i<user_count -1; i++ )
				user_list[i] = user_list[i+1];
			user_count--;
			return 0; /* user deleted */
		}

	return -2; /* no such user */
}

/* end of file */
