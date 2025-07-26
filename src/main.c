#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum _token_type
{
	invalid,
	number,
	plus,
	minus,
	whitespace,
}token_type;

//this is how we catch invalid tokens
token_type symbols[127];

void init_symbols()
{
	//whitespace symbols
	symbols['\n'] = whitespace;
	symbols[' ']  = whitespace;
	symbols['\t'] = whitespace;

	//number symbols
	symbols['0'] = number;
	symbols['1'] = number;
	symbols['2'] = number;
	symbols['3'] = number;
	symbols['4'] = number;
	symbols['5'] = number;
	symbols['6'] = number;
	symbols['7'] = number;
	symbols['8'] = number;
	symbols['9'] = number;

	//operator symbols
	symbols['+'] = plus;
	symbols['-'] = minus;
}

typedef struct _substr
{
	char* str;
	size_t len;
	size_t cursor;
}substr;

typedef struct _token
{
	token_type t_type;
	size_t start;
	size_t end;
}token;

//get the bounds of our token
void get_token(substr* str, token* t)
{
	size_t c = str->str[str->cursor];
	size_t start = str->cursor;
	size_t end = str->cursor;

	//pure whitespace isn't a symbol
	if(symbols[c] == whitespace)
	{
		str->cursor++;
		return;
	}

	//grab all symbols until we encounter whitespace
	while(symbols[c] != whitespace && symbols[c] != invalid && end < str->len)
	{
		end++;
		c = str->str[end];
	}

	//if we hit an invalid character
	if(symbols[c] == invalid)
	{
		fprintf(stderr, "Invalid symbol detected.\n");
		exit(-1);
	}

	str->cursor = end + 1;
	t->start = start;
	t->end = end;
}

int main() 
{
	init_symbols();
	char str[] = {"19 + 21 + 118 + 34 - 43 + 21\n"};

	substr to_pass;
	to_pass.str = str;
	to_pass.len = strlen(str);
	to_pass.cursor = 0;

	token tokens[30];
	size_t tok_cursor = 0;
	while(to_pass.cursor < to_pass.len - 1)
	{
		token* cur = &tokens[tok_cursor];
		get_token(&to_pass, cur);
		if(cur->end - cur->start > 0)
		{
			tok_cursor++;
		}
	}
}