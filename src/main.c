#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main() 
{
	return 0;
}