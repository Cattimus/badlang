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

State classify(char c)
{
	if(c == ' ' || c == '\n' || c == '\t')
	{
		return whitespace;
	}
	else if(c >= '0' && c <= '9')
	{
		return number;
	}
	else if(c == '+' || c == '-')
	{
		return operator;
	}

	return invalid;
}

int transitions[3][3] = {
	{number,  whitespace, invalid},
	{number,  whitespace, operator},
	{invalid, whitespace, invalid}
};

int is_valid(int transition[3][3], State initial, State accepting, const char* str)
{
	State state = initial;
	size_t len = strlen(str);
	for(size_t i = 0; i < len; i++)
	{
		char c = str[i];
		int j = classify(c);

		if(j == invalid)
		{
			return 0;
		}
	
		state = transition[state][j];

		if(state == invalid)
		{
			return 0;
		}
	}

	return state == accepting;
}

int main() 
{
	printf("%d\n", is_valid(transitions, number, number, "21 + 14 - 19 + 85 + 45 - 92 + 101"));
	return 0;
}