/* agent.c */

#define AGENT_TEAM_MAX 6

struct agent
{
	char* name;
	struct stat* stat;
};

struct agent_team
{
	struct agent* member[AGENT_TEAM_MAX];
	unsigned int count;
};

static struct agent** agent_list;
static unsigned int agent_count;

static struct agent* agent_new(const char* name)
{
	struct agent* newAgent;
	if ( name_check(name) ) return NULL;

	newAgent = malloc(sizeof (struct agent*));
	assert(newAgent);
	newAgent->name = name_new(name);
	agent_list = realloc(agent_list, 
		(agent_count +1) * sizeof (struct agent*));
	assert(agent_list);
	agent_list[agent_count] = newAgent;
	return agent_list[agent_count++];
}

static int agent_delete(const struct agent* agent)
{
	unsigned int i;

	if ( !agent ) return -1;

	for ( i=0; i<agent_count; i++ )
		if ( agent == agent_list[i] )
		{
			name_delete(agent_list[i]->name);
			free(agent_list[i]);
			for ( ; i<agent_count -1; i++ )
				agent_list[i] = agent_list[i+1];
			agent_count--;
			return 0; /* agent deleted */
		}

	return -2; /* no such agent listed */
}

static int agent_team_join(struct agent* joiner, struct agent_team* team)
{
	unsigned int i;
	if ( !joiner ) return -1; /* cannot be NULL */
	if ( !team ) return -2; /* cannot be NULL */

	if ( team->count >= AGENT_TEAM_MAX ) return -3; /* team is full */
	
	for ( i=0; i<AGENT_TEAM_MAX; i++ ) /* cannot join twice */
		if ( team->member[i] == joiner ) return -4;

	for ( i=0; i<agent_count; i++ )
		if ( joiner == agent_list[i] )
		{
			team->member[team->count] = joiner;
			team->count++;
			return 0; /* member joined the team */
		}

	return -5; /* unlisted agent */
	
}

static int agent_team_leave(struct agent* leaver, struct agent_team* team)
{
	unsigned int i;
	if ( !leaver ) return -1; /* cannot be NULL */
	if ( !team ) return -2; /* cannot be NULL */

	for ( i=0; i<team->count; i++ )
		if ( team->member[i] == leaver )
		{
			for ( ; i<team->count-1; i++ )
				team->member[i] = team->member[i+1];
			team->count--;
			return 0; /* member left the team */
		}
	return -3; /* not a member of this team */
}

/* end of file */
