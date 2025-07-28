#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	backslash
}Token;

Token classify[127];


int main() 
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
}