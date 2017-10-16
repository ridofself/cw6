/* test */

#include <assert.h> /* assert.h */
#include <stdio.h> /* printf */
#include <string.h> /* strcmp strlen */
#include <malloc.h> /* malloc, free */

static unsigned int test_count;

void test_test()
{
	assert(1);
	test_count++;
}

#include "./name.c"

/* this test suite assumes at least a 20 char NAME_SIZE_MAX */

void test_name_new()
{
	unsigned int i;
	char* truncName;
	char* newName = "New Name";
	unsigned int count = name_count;
	assert(!strcmp(newName, name_new(newName)));
	assert(name_count == count +1);
	name_delete(newName);
	test_count++;

	newName = malloc(sizeof (char) * NAME_SIZE_MAX +1);
	for ( i=0; i<NAME_SIZE_MAX +1; i++ ) newName[i] = 'x';
	truncName = name_new(newName);
	assert(strcmp(newName, truncName));
	name_delete(truncName);
	test_count++;

	free(newName);
}

void test_name_delete()
{
	char* noSuchName = "Does Not Exist";
	char* deleteThisName = "Name To Delete";

	deleteThisName = name_new(deleteThisName);
	assert(!name_delete(deleteThisName));
	test_count++;

	assert(name_delete(noSuchName) == -1);
	test_count++;
}

void test_name_check()
{
	unsigned int i;
	char* reuseName;
	char* longName;
	char* goodName = "Good New Name";
	char* badName = "ill3g@l ch@r$";

	assert(name_check(badName) == -1);
	test_count++;

	longName = malloc(sizeof (char) * NAME_SIZE_MAX +1);
	for ( i=0; i<NAME_SIZE_MAX +1; i++ ) longName[i] = 'x';
	assert(name_check(longName) == -2);
	test_count++;

	reuseName = name_new("Cannot Reuse");
	assert(name_check(reuseName) == -3);
	name_delete(reuseName);
	test_count++;

	assert(name_check(NULL) == -4);
	test_count++;

	assert(name_check("") == -4);
	test_count++;

	assert(!name_check(goodName));
	assert(!name_check(goodName)); /* checking has no side effects */
	test_count++;
}

void test_name_change() /* proof of concept, no corresponding function */
{
	char* oldName = "name to be changed";
	char* newName = "changed to this name";
	char* name = name_new(oldName);
	if ( !name_check(newName) ) strcpy(name, newName);
	assert(!strcmp(name, newName));
	test_count++;

	name_delete(name);
}

#include "./agent.c"

void test_agent_new()
{
	char* name = "New Agent Name";
	unsigned int count = agent_count;
	struct agent* newAgent = agent_new(name);
	assert(newAgent);
	assert(agent_count = count +1);
	assert(!strcmp(name, newAgent->name)); 
	test_count++;

	assert(!agent_new(name)); /* name used */
	test_count++;

	agent_delete(newAgent);
}

void test_agent_delete()
{
	char* name = "Delete Agent";
	struct agent* delAgent = agent_new(name);
	assert(!agent_delete(delAgent));
	test_count++;

	assert(agent_delete(NULL) == -1);
	test_count++;

	assert(agent_delete(delAgent) == -2);
	test_count++;

	assert(!name_check(name)); /* name should be available again */
	test_count++;
}

#include "./user.c"

void test_user_new()
{
	char* name = "New User Name";
	unsigned int count = user_count;
	struct user* newUser = user_new(name);
	assert(newUser);
	assert(user_count == count +1);
	assert(!strcmp(name, newUser->name)); 
	test_count++;

	assert(!user_new(name)); /* name used */
	test_count++;

	user_delete(newUser);
}

void test_user_delete()
{
	char* name = "Delete User";
	struct user* delUser = user_new(name);
	assert(!user_delete(delUser));
	test_count++;

	assert(user_delete(NULL) == -1);
	test_count++;

	assert(user_delete(delUser) == -2); /* already deleted */
	test_count++;

	assert(!name_check(name)); /* name should be available again */
	test_count++;
}

void test_agent_team_join()
{
	struct agent* rogueAgent = malloc(sizeof (struct agent*));

	unsigned int oldTeamCount;
	struct user* user = user_new("has team");
	struct agent* joiner = agent_new("joins team");
	struct agent_team* team = malloc(sizeof (struct agent_team));

	assert(agent_team_join(NULL, team) == -1); /* cannot be null */
	test_count++;

	assert(agent_team_join(joiner, NULL) == -2); /* cannot be null */
	test_count++;

	assert(agent_team_join(rogueAgent, team) == -5); /* unlisted agent */
	test_count++;

	oldTeamCount = team->count;
	assert(!agent_team_join(joiner, team)); /* successful join */
	test_count++;

	assert(team->count == oldTeamCount +1); /* team count incremented */
	test_count++;

	user->team = team;
	assert(user->team == team); /* team properly assigned to a user */
	test_count++;

	oldTeamCount = team->count;
	team->count = AGENT_TEAM_MAX;
	assert(agent_team_join(joiner, team) == -3); /* team is full */
	team->count = oldTeamCount;
	test_count++;

	assert(agent_team_join(joiner, team) == -4); /* already joined */
	test_count++;

	agent_delete(joiner);
	free(rogueAgent);
	user_delete(user); /* also frees team (but not agents) */
}

void test_agent_team_leave()
{
	struct agent* rogueAgent = malloc(sizeof (struct agent*));

	unsigned int oldTeamCount;
	struct agent* leaver = agent_new("leaves team");
	struct agent_team* team = malloc(sizeof (struct agent_team));
	agent_team_join(leaver, team);
	oldTeamCount = team->count;
	assert(!agent_team_leave(leaver, team));
	test_count++;

	assert(team->count == oldTeamCount -1); /* count is decremented */
	test_count++;

	assert(agent_team_leave(NULL, team) == -1); /* cannot be NULL */
	test_count++;

	assert(agent_team_leave(leaver, NULL) == -2); /* cannot be NULL */
	test_count++;

	assert(agent_team_leave(leaver, team) == -3); /* no such member */
	test_count++;

	assert(agent_team_leave(rogueAgent, team) == -3); /* no such member */
	test_count++;

	free(team);
	free(rogueAgent);
	agent_delete(leaver);
}

void test_all()
{
	test_test();
	test_name_new();
	test_name_delete();
	test_name_check();
	test_name_change();

	test_user_new();
	test_user_delete();

	test_agent_new();
	test_agent_delete();
	test_agent_team_join();
	test_agent_team_leave();
}

int main()
{
	printf("testing...\n");
	test_all();
	printf("\tall of %d tests passed\n", test_count);
	return 0;
}
