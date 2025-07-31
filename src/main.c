#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Filter_Node Filter_Node;
typedef struct Filter Filter;
typedef struct DFA DFA;

typedef struct Filter
{
	const char* accepted;
	const char* rejected;
	const char* fail;
}Filter;

typedef struct Filter_Node
{
	int accept;
	Filter filter;
	Filter_Node* next;
}Filter_Node;

Filter new_filter()
{
	Filter f;
	f.accepted = NULL;
	f.rejected = NULL;
	f.fail = NULL;
	return f;
}

Filter_Node new_node()
{
	Filter_Node n;
	n.accept = -1;
	n.next = NULL;
	n.filter = new_filter();
	return n;
}

//check if string contains character
int contains(const char* str, char c)
{
	if(str == NULL)
	{
		return 0;
	}

	int len = strlen(str);
	for(int i = 0; i < len; i++)
	{
		if(str[i] == c)
		{
			return 1;
		}
	}

	return 0;
}

//evaluate a filter
int eval_filter(Filter f, char c)
{
	if(f.accepted)
	{
		if(contains(f.accepted, c))
		{
			return 1;
		}
	}

	if(f.rejected)
	{
		if(contains(f.rejected, c))
		{
			return 0;
		}
	}

	if(f.fail)
	{
		if(contains(f.fail, c))
		{
			return -1;
		}
	}

	//if we've reached this point in the function, we should accept the input
	if(!f.accepted)
	{
		return 1;
	}

	return 0;
}

typedef struct DFA
{
	Filter_Node* filter;
	int* accepted_states;
	int accepted_count;
}DFA;

//we want to avoid using dynamically allocated memory where possible,
//so we will assume these are allocated on the stack or otherwise allocated by a future helper function
DFA create_DFA(Filter_Node* filter, int* accepted_states, int accepted_count)
{
	DFA dfa;
	dfa.filter = filter;
	dfa.accepted_states = accepted_states;
	dfa.accepted_count = accepted_count;
	return dfa;
}

int contains_int(int* accepted_states, int count, int state)
{
	for(int i = 0; i < count; i++)
	{
		if(state == accepted_states[i])
		{
			return 1;
		}
	}

	return 0;
}

//we will assume our string is null-terminated
int match(DFA* dfa, const char* str)
{
	int state = 0;
	int len = strlen(str);
	Filter_Node* f = dfa->filter;
	int i = 0;
	for(i = 0; i < len; i++)
	{
		char c = str[i];
		Filter_Node* fc = &f[state];
		int fc_matched = 0;
		do
		{
			int r = eval_filter(fc->filter, c);
			if(r == -1)
			{
				return 0;
			}

			if(r)
			{
				state = fc->accept;
				fc_matched = 1;
				break;
			}
			else
			{
				fc = fc->next;
			}
		}while(fc);

		//break out if we don't find a valid match by this point
		if(!fc_matched)
		{
			break;
		}
	}

	if(contains_int(dfa->accepted_states, dfa->accepted_count, state))
	{
		return i;
	}

	return 0;
}

int main() 
{
	Filter_Node filters[4];

	for(int i = 0; i < 4; i++)
	{
		filters[i] = new_node();
	}

	filters[0].filter.accepted = "0123456789";
	filters[0].accept = 1;

	filters[1].filter.accepted = ".";
	filters[1].accept = 2;
	filters[1].next = &filters[3];

	filters[2].filter.accepted = "0123456789";
	filters[2].accept = 2;

	filters[3].filter.accepted = "0123456789";
	filters[3].accept = 1;

	int accepted_states[2] = {1,2};

	DFA dfa;
	dfa.accepted_count = 2;
	dfa.accepted_states = accepted_states;
	dfa.filter = filters;

	char str[] = "191.231L123123";
	printf("matching string for literal: %.*s\n", match(&dfa, str), str);
}