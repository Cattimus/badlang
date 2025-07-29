#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int (*filter)(char);

typedef enum
{
	invalid,
	add,
	subtract,
	multiply,
	divide,
	assign,
	whitespace,
	separator,
	identifier,
	literal,
	digit,
	decimal,
	backslash,
	letter,
	underscore
}Token;

Token classify[127];

typedef struct
{
	int initial_state;
	int state_count;
	int* accepted_states;
	int accepted_state_count;
	filter** transitions; //array of filters for different states
	int* transition_count; //how many transitions each state has
}DFA;

DFA create_DFA(int initial_state, int max_state, int* accepted_states, int accepted_state_count)
{
	DFA dfa;
	dfa.initial_state = initial_state;
	dfa.state_count = max_state;
	dfa.accepted_state_count = accepted_state_count;
	dfa.accepted_states = (int*)calloc(accepted_state_count, sizeof(int));
	memcpy(dfa.accepted_states, accepted_states, accepted_state_count);
	dfa.transition_count = (int*)calloc(max_state, sizeof(int));

	//holy syntax batman (allocating the array to store filter function pointers)
	dfa.transitions = (filter**)calloc(max_state, sizeof(filter*));
	return dfa;
}

//free dynamically allocated memory
void destroy_DFA(DFA* dfa)
{
	if(dfa->accepted_states != NULL)
	{
		free(dfa->accepted_states);
		dfa->accepted_states = NULL;
	}

	if(dfa->transition_count != NULL)
	{
		free(dfa->transition_count);
		dfa->transition_count = NULL;
	}

	if(dfa->transitions != NULL)
	{
		for(size_t i = 0; i < (unsigned int)dfa->state_count; i++)
		{
			if(dfa->transitions[i] != NULL)
			{
				free(dfa->transitions[i]);
				dfa->transitions[i] = NULL;
			}
		}

		free(dfa->transitions);
		dfa->transitions = NULL;
	}
}

int add_filter(DFA* dfa, int state, filter transition)
{
	//allocate new memory
	if(dfa->transitions[state] == NULL)
	{
		dfa->transition_count[state]++;
		dfa->transitions[state] = calloc(1, sizeof(filter));
	}
	else
	{
		filter* temp = NULL;
		dfa->transition_count[state]++;

		temp = realloc(dfa->transitions[state], dfa->transition_count[state] * sizeof(filter));
		//failure to realloc array
		if(temp == NULL)
		{
			return 0;
		}

		dfa->transitions[state] = temp;
	}

	dfa->transitions[state][dfa->transition_count[state]-1] = transition;
	return 1;
}

int get_token(DFA* dfa, const char* str)
{
	size_t len = strlen(str);
	int state = dfa->initial_state;
	size_t i = 0;
	for(i = 0; i < len; i++)
	{
		char c = str[i];
		int is_accepted = 0;
		//go through each transition until we hit one that is acceptable
		for(size_t j = 0; j < (size_t)dfa->transition_count[state]; j++)
		{
			int temp_state = dfa->transitions[state][j](c);
			if(temp_state)
			{
				state = temp_state;
				is_accepted = 1;
				break;
			}
		}

		if(!is_accepted)
		{
			break;
		}
	}

	for(size_t j = 0; j < (size_t)dfa->accepted_state_count; j++)
	{
		if(state == dfa->accepted_states[j])
		{
			return i;
		}
	}

	return 0;
}

void init_classifiers()
{
	classify['\n'] = whitespace;
	classify['\t'] = whitespace;
	classify[' '] = whitespace;

	classify['0'] = digit;
	classify['1'] = digit;
	classify['2'] = digit;
	classify['3'] = digit;
	classify['4'] = digit;
	classify['5'] = digit;
	classify['6'] = digit;
	classify['7'] = digit;
	classify['8'] = digit;
	classify['9'] = digit;
	classify['.'] = decimal;

	//separator
	classify[';'] = separator;

	//operators
	classify['*'] = multiply;
	classify['+'] = add;
	classify['/'] = divide;
	classify['-'] = subtract;
	classify['='] = assign;

	classify['/'] = backslash;
	classify['_'] = underscore;

	//characters
	for(char i = 'a'; i < 'z'; i++)
	{
		classify[(int)i] = letter;
	}
	for(char i = 'A'; i < 'Z'; i++)
	{
		classify[(int)i] = letter;
	}
}

int comment_0(char c)
{
	if(c == '/')
	{
		return 1;
	}
	return 0;
}

int comment_1(char c)
{
	if(c == '\n')
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

int main() 
{
	init_classifiers();

	int accepted_states[] = {2};
	DFA comment = create_DFA(0, 2, accepted_states, 1);
	add_filter(&comment, 0, comment_0);
	add_filter(&comment, 1, comment_1);
	printf("%d\n", get_token(&comment, "//this is a comment\n //this is another comment\n"));
	destroy_DFA(&comment);
}