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
	tokens.arr = (token*)calloc(input_max, sizeof(token));

	//check that memory was initialized
	if(tokens.arr == NULL)
	{
		fprintf(stderr, "Unable to allocate space for tokens\n");
		free(tokens.arr);
		exit(-1);
	}

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
			fprintf(stderr, "Error - Unrecognized token: %c\n", c);
			free(tokens.arr); //free dynamically allocated memory
			exit(-1);
		}
	}

	//resize token array to appropriate size
	void* temp = realloc(tokens.arr, sizeof(token) * tokens.size);
	if(temp == NULL)
	{
		fprintf(stderr, "Error - Unable to resize token array\n");
		free(tokens.arr);
		exit(-1);
	}

	return tokens;
}

int main() 
{
	char str[] = "15 + 21 - 45 + 2000 - 1500";
	token_arr tokens = tokenize(str);

	for(size_t i = 0; i < tokens.size; i++)
	{
		token* t = &tokens.arr[i];

		switch(t->t_type)
		{
			case number:
				printf("Token type: number\n");
				break;

			case minus:
				printf("Token type: minus\n");
				break;

			case plus:
				printf("Token type: plus\n");
				break;
		}

		printf("Token start: %d\n", t->start);
		printf("Token end: %d\n", t->end);
		printf("Token: %.*s\n", t->end-t->start, str+t->start);
		printf("\n");
	}

	free_token_arr(&tokens);
	return 0;
}