#include <stdio.h>
#include <string.h>

typedef struct _substr
{
	const char* str;
	size_t len;
	size_t offset;
}substr;

//filters
char num(const char c)
{
	if(c >= '0' && c <= '9')
	{
		return c;
	}

	return 0;
}

char is_whitespace(const char c)
{
	if(c == ' ' || c == '\n' || c == '\t')
	{
		return c;
	}

	return 0;
}

char is_plus(const char c)
{
	if(c == '+')
	{
		return c;
	}

	return 0;
}

char is_minus(const char c)
{
	if(c == '-')
	{
		return c;
	}

	return 0;
}

typedef enum _token_type
{
	number,
	plus,
	minus
}token_type;

typedef struct _token
{
	token_type t_type;
	union 
	{
		int number;
	}data;

	size_t start;
	size_t end;
}token;

//lexer functions
int tokenize(substr* input, token_type t, char (*filter)(char), char (*end)(char), token* to_return)
{
	size_t index = input->offset;
	size_t tok_start = index;
	while(filter(input->str[index]) && !end(input->str[index]) && index < input->len - 1)
	{
		index++;
	}

	if(!end(input->str[index]))
	{
		return 0;
	}

	size_t tok_end = index;

	//write data to token
	to_return->t_type = t;
	to_return->start = tok_start;
	to_return->end = tok_end;
	return 1;
}

//clear characters(like whitespace)
void clear_characters(substr* input, int (*filter)(char))
{
	size_t index = input->offset;
	while(filter(input->str[index]))
	{
		index++;
	}

	if(input->offset >= input->len)
	{
		input->offset = input->len - 1;
	}
}

int main() 
{

}