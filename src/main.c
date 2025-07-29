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
	memcpy(dfa.accepted_states, accepted_states, accepted_state_count * sizeof(int));
	dfa.transition_count = (int*)calloc(max_state + 1, sizeof(int));

	//holy syntax batman (allocating the array to store filter function pointers)
	dfa.transitions = (filter**)calloc(max_state + 1, sizeof(filter*));
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
	size_t i = 1;
	for(i = 1; i < len; i++)
	{
		char c = str[i];
		int is_accepted = 0;
		//go through each transition until we hit one that is acceptable
		for(size_t j = 0; j < (size_t)dfa->transition_count[state]; j++)
		{
			int temp_state = dfa->transitions[state][j](c);
			if(temp_state != -1)
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
			return i-1;
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
	return -1;
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

int identifier_0(char c)
{
	Token s = classify[(size_t)c];
	switch(s)
	{
		case letter:
		case digit:
		case underscore:
			return 0;
			break;

		case whitespace:
			return 1;
			break;
		
		case separator:
			return 2;
			break;

		default:
			return -1;
			break;
	}
}

//TODO - we need a way to ensure that there is only one . in a literal
int literal_0(char c)
{
	Token s = classify[(size_t)c];
	switch(s)
	{
		case digit:
			return 0;
			break;

		case whitespace:
			return 2;
			break;

		case separator:
			return 3;
			break;

		case decimal:
			return 1;
			break;

		default:
			return -1;
			break;
	}
}

int literal_1(char c)
{
	Token s = classify[(size_t)c];
	if(s == digit)
	{
		return 0;
	}

	return -1;
}

int main() 
{
	init_classifiers();

	

	//comments
	char comment_str[] = "//this is a comment\nthis is not.";
	int comment_accepted[] = {2};
	DFA comment = create_DFA(0, 2, comment_accepted, 1);
	add_filter(&comment, 0, comment_0);
	add_filter(&comment, 1, comment_1);
	printf("comment: %d\n", get_token(&comment, comment_str));
	printf("comment2: %d\n", get_token(&comment, comment_str+20));
	printf("\n");

	//identifier
	char identifier_str[] = "this_is;my_string = 41;";
	int identifier_accepted[] = {1,2};
	DFA identifier = create_DFA(0, 2, identifier_accepted, 2);
	add_filter(&identifier, 0, identifier_0);
	printf("identifier: %d\n", get_token(&identifier, identifier_str));
	printf("identifier 2: %d\n", get_token(&identifier, identifier_str+8));
	printf("\n");

	//literal
	char literal_str[] = "15.08;1234567;1423l123";
	int literal_accepted[] = {2, 3};
	DFA literal = create_DFA(0, 3, literal_accepted, 2);
	add_filter(&literal, 0, literal_0);
	add_filter(&literal, 1, literal_1);
	printf("literal: %d\n", get_token(&literal, literal_str));
	printf("literal 2: %d\n", get_token(&literal, literal_str+6));
	printf("literal 3: %d\n", get_token(&literal, literal_str+14));
	printf("\n\n");


	//time to actually do the parsing
	//TODO - account for things happening at the end of the string
	char str[] = "//this is my comment\nlars = 18.1942;lars = lars * 15;lars + 24;lars - 15;24 / 18 ";
	size_t len = strlen(str);
	for(size_t i = 0; i < len; i++)
	{
		Token c = classify[(size_t)str[i]];
		switch(c)
		{
			case backslash:
			{
				int sz = get_token(&comment, str+i);
				i += sz;
				if(sz == 0)
				{
					printf("divide: %d\n", 1);
				}
				else
				{
					printf("comment: %d\n", sz);
				}
				break;
			}

			case letter:
			{
				int sz = get_token(&identifier, str+i);
				i += sz;
				printf("identifier: %d\n", sz);
				break;
			}

			case digit:
			{
				int sz = get_token(&literal, str+i);
				i += sz;
				printf("literal: %d\n", sz);
				break;
			}

			case add:
				printf("add: %d\n", 1);
				break;

			case assign:
				printf("assign: %d\n", 1);
				break;

			case subtract:
				printf("subtract: %d\n", 1);
				break;

			case multiply:
				printf("multiply: %d\n", 1);
				break;

			case divide:
				printf("divide: %d\n", 1);
				break;

			case whitespace:
				break;

			case separator:
				break;

			default:
				printf("Invalid character reached, aborting.");
				exit(-1);
				break;
		}
	}


	destroy_DFA(&comment);
	destroy_DFA(&identifier);
	destroy_DFA(&literal);
}