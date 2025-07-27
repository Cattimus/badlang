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
	long value;
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

int parse_token(substr* str, token* t)
{
	switch(str->str[t->start])
	{
		case '+':
		{
			if(t->end == t->start + 1)
			{
				t->t_type = plus;
				return 1;
			}
			break;
		}

		case '-':
		{
			if(t->end == t->start + 1)
			{
				t->t_type = minus;
				return 1;
			}
		}
	}

	//if we've reached this part in the function, we've got a pretty good idea it's a number
	for(size_t i = t->start; i < t->end; i++)
	{
		//if we encounter something that's not a digit, we know it's invalid
		if(symbols[(size_t)str->str[i]] != number)
		{
			return 0;
		}
	}

	//we're not checking the output value here but that's fine
	t->t_type = number;
	char* end;
	t->value = strtol(str->str + t->start, &end, 10);
	return 1;
}


//ast node
typedef struct _node
{
	token* t;
	struct _node* left;
	struct _node* right;
}node;

//AST structure
typedef struct _tree
{
	node* nodes;
	size_t count;
	size_t cursor;
}tree;

//free dynamic memory
void free_ast(tree* ast)
{
	if(ast->nodes != NULL)
	{
		free(ast->nodes);
		ast->nodes = NULL;
	}
}

//assemble AST from node array
tree assemble_tree(token* tokens, size_t token_count)
{
	//allocate memory for our tree
	tree ast;
	ast.count = 0;
	ast.cursor = 0;
	ast.nodes = (node*)calloc(token_count ,sizeof(node*));

	node* cur = &ast.nodes[0];
	cur->t = &tokens[0];
	for(size_t i = 1; i < token_count; i++)
	{
		ast.nodes[i].t = &tokens[i];

		if(cur->t->t_type == number)
		{
			if(tokens[i].t_type != number)
			{
				cur->right = &ast.nodes[i];
				cur = cur->right;
			}
		}
		
		else
		{
			if(tokens[i].t_type == number)
			{
				cur->left = &ast.nodes[i];
			}
		}
	}

	return ast;
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
			parse_token(&to_pass, cur);
		}
	}

	tree ast = assemble_tree(tokens, tok_cursor);
	free_ast(&ast);

	return 0;
}