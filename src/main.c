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

int main() 
{
	init_symbols();
	char str[] = {"19 + 21 + 118 + 34 - 43 + 21"};
	for(size_t i = 0; i < strlen(str); i++)
	{
		if(symbols[str[i]] == invalid)
		{
			fprintf(stderr, "invalid symbol detected: %c\n", str[i]);
			exit(-1);
		}
	}
}