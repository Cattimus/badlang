#include <stdio.h>
#include <string.h>

typedef struct _substr
{
	const char* str;
	size_t len;
	size_t offset;
}substr;

//filters
int is_num(const char c)
{
	return c >= '0' && c <= '9';
}

int is_whitespace(const char c)
{
	return c == ' ' || c == '\n' || c == '\t';
}

int is_plus(const char c)
{
	return c == '+';
}

int is_minus(const char c)
{
	return c == '-';
}

//lexer functions
int tokenize_number(substr* input)
{
	size_t index = input->offset;
	while(is_num(input->str[index]))
	{
		index++;
	}

	if(!is_whitespace(input->str[index]))
	{
		//invalid token
		return 0;
	}
	
	//bounds check to make sure we're not outside of string
	if(input->offset < index)
	{
		input->offset = index + 1;
		if(input->offset >= input->len)
		{
			input->offset--;
		}
		return 1;
	}

	return 0;
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