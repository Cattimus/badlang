#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum _token_type
{
	minus,
	plus,
	number
}token_type;

typedef struct _token
{
	token_type t_type;
	size_t start;
	size_t end;
}token;

typedef struct _token_arr
{
	token* arr;
	size_t size;
}token_arr;

void free_token_arr(token_arr* to_free)
{
	free(to_free->arr);
	to_free->arr = NULL;
}

//allocate memory and quit safely on failure
void* alloc_nofail(size_t size)
{
	void* ptr = calloc(1, size);
	if(ptr == NULL)
	{
		fprintf(stderr, "badlang: Memory allocation failure\n");
		exit(-1);
	}

	return ptr;
}

//reallocate memory and quit safely on failure
void* realloc_nofail(void* original, size_t size)
{
	void* ptr = realloc(original, size);
	if(ptr == NULL)
	{
		fprintf(stderr, "badlang: Memory realloc failure\n");
		exit(-2);
	}

	return ptr;
}

int is_whitespace(char c)
{
	if(c == '	' || c == ' ' || c == '\n')
	{
		return 1;
	}

	return 0;
}

//valid literals may be comprised of any values [0-9]
int is_literal(char c)
{
	return (c - '0' < 10) && (c - '0' > -1);
}

//tokenize a string
token_arr tokenize(const char* input)
{
	size_t input_max = strlen(input);

	//allocate space for our tokens
	token_arr tokens;
	tokens.size = 0;
	tokens.arr = (token*)alloc_nofail(input_max * sizeof(token));

	//tokenize data
	for(size_t i = 0; i < input_max; i++)
	{
		char c = input[i];
		if(is_whitespace(c))
		{
			continue;
		}

		else if(is_literal(c))
		{
			token t;
			t.t_type = number;
			t.start = i;
			t.end = i;

			//find end of literal
			while(i < input_max && is_literal(c))
			{
				i++;
				t.end = i;
				c = input[i];
			}

			tokens.arr[tokens.size] = t;
			tokens.size++;
		}

		else if(c == '+')
		{
			token t;
			t.t_type = plus;
			t.start = i;
			t.end = i+1;
			tokens.arr[tokens.size] = t;
			tokens.size++;
		}

		else if(c == '-')
		{
			token t;
			t.t_type = minus;
			t.start = i;
			t.end = i+1;
			tokens.arr[tokens.size] = t;
			tokens.size++;
		}

		//we've encountered an invalid token
		else
		{
			fprintf(stderr, "badlang: Invalid token encountered\n");
			exit(-3);
		}
	}

	//resize token array to appropriate size
	tokens.arr = realloc_nofail(tokens.arr, sizeof(token) * tokens.size);

	return tokens;
}

//AST node structure
typedef struct _node
{
	token_type t_type;
	union
	{
		int number;
	}value;

	struct _node* left;
	struct _node* right;
}node;

//parse number from input token and string
int parse_node(token* token, char* str)
{
	//copy substring
	size_t len = token->end-token->start;
	char* substr = (char*)alloc_nofail((len+1) * sizeof(char));
	strncpy(substr, str+token->start, len);

	//convert to integer
	int num = atoi(substr);

	//free memory
	free(substr);

	return num;
}

//assemble AST from tokens
node* make_ast(token_arr tokens, char* str)
{
	token* tok = &tokens.arr[0];

	//create root of tree and initialize with the first token
	if(tokens.arr[0].t_type != number)
	{
		return NULL;
	}

	//assign root node
	node* root = (node*)alloc_nofail(1 * sizeof(node));
	root->t_type = tok->t_type;
	root->value.number = parse_node(tok, str);

	node* cur = root;

	//loop over the rest of the tokens
	for(size_t i = 1; i < tokens.size; i++)
	{
		tok = &tokens.arr[i];

		//assign values to node
		node* next = (node*)alloc_nofail(1 * sizeof(node));
		next->t_type = tok->t_type;

		if(next->t_type == number)
		{
			next->value.number = parse_node(tok, str);
		}

		if(cur->t_type == number && (next->t_type == minus || next->t_type == plus))
		{
			cur->left = next;
			cur = cur->left;
		}

		else if((cur->t_type == minus || cur->t_type == plus) && next->t_type == number)
		{
			cur->right = next;
		}

		else if((cur->t_type == minus || cur->t_type == plus) && (next->t_type == minus || next->t_type == plus))
		{
			cur->left = next;
			cur = cur->left;
		}
	}

	return root;
}

//recursively free tree
void free_ast(node* root)
{
	if(root == NULL)
	{
		return;
	}

	if(root->left != NULL)
	{
		free_ast(root->left);
		root->left = NULL;
	}

	if(root->right != NULL)
	{
		free_ast(root->right);
		root->right = NULL;
	}

	free(root);
	root = NULL;
}

int main() 
{
	char str[] = "15 + 21 - 45 + 2000 - 1500";
	token_arr tokens = tokenize(str);

	node* tree = make_ast(tokens, str);
	free_ast(tree);
	free_token_arr(&tokens);
	return 0;
}