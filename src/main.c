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
	int success_state;
}Filter;

typedef struct Filter_Node
{
	int state;
	Filter filter;
	struct Filter_Node* accept;
	struct Filter_Node* next;
}Filter_Node;

Filter new_filter()
{
	Filter f;
	f.accepted = NULL;
	f.rejected = NULL;
	f.fail = NULL;
	f.success_state = 0;
	return f;
}

Filter_Node new_node()
{
	Filter_Node n;
	n.accept = NULL;
	n.next = NULL;
	n.filter = new_filter();
	n.state = -1;
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
			return f.success_state;
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
		return f.success_state;
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
//we also assume we've read the first character of the string before passing it to eval
int eval_DFA(DFA* dfa, const char* str)
{
	int len = strlen(str);
	Filter_Node* f = dfa->filter;
	for(int i = 0; i < len; i++)
	{
		char c = str[i];
		Filter_Node* fc = f;
		do
		{
			if(eval_filter(fc->filter, c))
			{
				if(fc->accept == NULL)
				{
					return 0;
				}

				f = fc->accept;
			}
		}while(fc->next != NULL);
	}

	if(contains_int(dfa->accepted_states, dfa->accepted_count, f->state))
	{
		return 1;
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
	filters[0].state = 0;
	filters[1].filter.accepted = "0123456789";
	filters[1].state = 1;
	filters[2].filter.accepted = ".";
	filters[2].state = 1;
	filters[3].filter.accepted = "0123456789";
	filters[3].state = 2;

	filters[0].accept = &filters[1];

	filters[1].accept = &filters[1];
	filters[1].next = &filters[2];

	filters[2].accept = &filters[3];
	filters[3].accept = &filters[3];

	int accepted_states[2] = {1,2};

	DFA dfa;
	dfa.accepted_count = 2;
	dfa.accepted_states = accepted_states;
	dfa.filter = filters;

	printf("%d\n", eval_DFA(&dfa, "191231923"));
}