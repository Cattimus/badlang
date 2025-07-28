#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum _State
{
	number,
	whitespace,
	operator
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

int transition_number(char c)
{
	if(is_whitespace(c))
	{
		return 1;
	}
	else if(is_number(c))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int transition_whitespace(char c)
{
	if(is_operator(c))
	{
		return 1;
	}
	else if(is_whitespace(c))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int transition_operator(char c)
{
	if(is_whitespace(c))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//TODO - this won't work because it only goes one direction
int is_valid(const char* str)
{
	size_t len = strlen(str);
	State state = number;
	for(size_t i = 0; i < len; i++)
	{
		char c = str[i];
		switch(state)
		{
			case number:
			{
				int result = transition_number(c);
				if(result < 0)
				{
					return 0;
				}
				else if(result == 1)
				{
					state += 1;
				}
				break;
			}


			case whitespace:
			{
				int result = transition_whitespace(c);
				if(result < 0)
				{
					return 0;
				}
				else if(result == 1)
				{
					state += 1;
				}
				break;
			}

			case operator:
			{
				int result = transition_operator(c);
				if(result < 0)
				{
					return 0;
				}
				else if(result == 1)
				{
					state -= 1;
				}

				break;
			}
		}
	}
}

int main() 
{
	return 0;
}