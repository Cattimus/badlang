#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct
{
	const char* accepted;
	const char* rejected;
	const char* fail;
	int success_state;
}filter;

filter new_filter()
{
	filter f;
	f.accepted = NULL;
	f.rejected = NULL;
	f.fail = NULL;
	f.success_state = 0;
	return f;
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
int eval_filter(filter f, char c)
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

typedef struct
{
	filter* filters;
	int* filter_count;
	int filter_row;
	int* accepted_states;
	int accepted_count;
}DFA;

//we want to avoid using dynamically allocated memory where possible,
//so we will assume these are allocated on the stack or otherwise allocated by a future helper function
DFA create_DFA(filter* filters, int* filter_count, int filter_row, int* accepted_states, int accepted_count)
{
	DFA dfa;
	dfa.filters = filters;
	dfa.filter_count = filter_count;
	dfa.filter_row = filter_row;
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
	int state = 0;
	int len = strlen(str);
	int y = dfa->filter_row;
	for(int i = 1; i < len; i++)
	{
		char c = str[i];
		for(int j = 0; j < dfa->filter_count[state]; j++)
		{
			int r = eval_filter(dfa->filters[state * y + j], c);
			if(r == -1)
			{
				return 0;
			}

			if(r)
			{
				state = r;
				break;
			}
		}
	}

	if(contains_int(dfa->accepted_states, dfa->accepted_count, state))
	{
		return 1;
	}

	return 0;
}


int main() 
{
	filter filters[2 * 2];

	for(int i = 0; i < 4; i++)
	{
		filters[i] = new_filter();
	}

	//stinky version of 2d array access because c hates me
	filters[0 * 2 + 0].accepted = "/";
	filters[0 * 2 + 0].success_state = 1;
	filters[1 * 2 + 0].rejected = "\n";
	filters[1 * 2 + 0].success_state = 1;
	filters[1 * 2 + 1].accepted = "\n";
	filters[1 * 2 + 1].success_state = 2;

	int filter_count[] = {1, 2};
	int accepted_states[] = {2};
	const char* str = "//this is a comment\n";
	const char* str2 = "this is not a comment\n";

	DFA dfa = create_DFA(filters, filter_count, 2, accepted_states, 1);
	int result = eval_DFA(&dfa, str);
	int result2 = eval_DFA(&dfa, str2);
	printf("result: %d\n", result);
	printf("result2: %d\n", result2);
}