#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum _State
{
	invalid = -1,
	number,
	whitespace,
	operator,
}State;

int is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

int is_number(char c)
{
	return (c >= '0' && c <= '9');
}

int is_operator(char c)
{
	return (c == '+' || c == '-');
}

int transition(State state, char c)
{
	switch(state)
	{
		case number:
		{
			if(is_whitespace(c))
				return whitespace;
			else if(is_number(c))
				return number;
			else
				return invalid;
			break;
		}

		case whitespace:
		{
			if(is_operator(c))
				return operator;
			else if(is_number(c))
				return number;
			else
				return invalid;
			break;
		}

		case operator:
		{
			if(is_whitespace(c))
				return whitespace;
			else
				return invalid;
		}
	}

	return invalid;
}

int is_valid(const char* str)
{
	size_t len = strlen(str);
	State state = number;
	for(size_t i = 0; i < len; i++)
	{
		char c = str[i];
		state = transition(state, c);
		if(state == invalid)
		{
			return 0;
		}
	}

	return 1;
}

int main() 
{
	printf("%d\n", is_valid("15 + 21 + 84 - 21 + 99"));
	
	return 0;
}