#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Filter_Node Filter_Node;
typedef struct Filter Filter;
typedef struct DFA DFA;

typedef struct Filter
{
	const char* accepted;
	const char* rejected;
	const char* fail;
	int reject_all;
}Filter;

typedef struct Filter_Node
{
	int accept;
	Filter filter;
	Filter_Node* next;
}Filter_Node;

Filter new_filter()
{
	Filter f;
	f.accepted = NULL;
	f.rejected = NULL;
	f.fail = NULL;
	f.reject_all = 0;
	return f;
}

Filter_Node new_node()
{
	Filter_Node n;
	n.accept = -1;
	n.next = NULL;
	n.filter = new_filter();
	return n;
}

//check if string contains character
int contains(const char* str, char c)
{
	if(str == NULL)
	{
		return 0;
	}

	int len = strlen(str);
	for(int i = 0; i < len; i++)
	{
		if(str[i] == c)
		{
			return 1;
		}
	}

	return 0;
}

//evaluate a filter
int eval_filter(Filter f, char c)
{
	if(f.reject_all)
	{
		return 0;
	}

	if(f.accepted)
	{
		if(contains(f.accepted, c))
		{
			return 1;
		}
	}

	if(f.rejected)
	{
		if(contains(f.rejected, c))
		{
			return 0;
		}
	}

	if(f.fail)
	{
		if(contains(f.fail, c))
		{
			return -1;
		}
	}

	//if we've reached this point in the function, we should accept the input
	if(!f.accepted)
	{
		return 1;
	}

	return 0;
}

typedef struct DFA
{
	Filter_Node* filter;
	int* accepted_states;
	int accepted_count;
}DFA;

//we want to avoid using dynamically allocated memory where possible,
//so we will assume these are allocated on the stack or otherwise allocated by a future helper function
DFA create_DFA(Filter_Node* filter, int* accepted_states, int accepted_count)
{
	DFA dfa;
	dfa.filter = filter;
	dfa.accepted_states = accepted_states;
	dfa.accepted_count = accepted_count;
	return dfa;
}

int contains_int(int* accepted_states, int count, int state)
{
	for(int i = 0; i < count; i++)
	{
		if(state == accepted_states[i])
		{
			return 1;
		}
	}

	return 0;
}

//we will assume our string is null-terminated
int match(DFA* dfa, const char* str)
{
	int state = 0;
	int len = strlen(str);
	Filter_Node* f = dfa->filter;
	int i = 0;
	for(i = 0; i < len; i++)
	{
		char c = str[i];
		Filter_Node* fc = &f[state];
		int fc_matched = 0;
		do
		{
			int r = eval_filter(fc->filter, c);
			if(r == -1)
			{
				return 0;
			}

			if(r)
			{
				state = fc->accept;
				fc_matched = 1;
				break;
			}
			else
			{
				fc = fc->next;
			}
		}while(fc);

		//break out if we don't find a valid match by this point
		if(!fc_matched)
		{
			break;
		}
	}

	if(contains_int(dfa->accepted_states, dfa->accepted_count, state))
	{
		return i;
	}

	return 0;
}

int main() 
{
	Filter_Node literal_filter[4];

	for(int i = 0; i < 4; i++)
	{
		literal_filter[i] = new_node();
	}

	literal_filter[0].filter.accepted = "0123456789";
	literal_filter[0].accept = 1;

	literal_filter[1].filter.accepted = ".";
	literal_filter[1].accept = 2;
	literal_filter[1].next = &literal_filter[3];

	literal_filter[2].filter.accepted = "0123456789";
	literal_filter[2].accept = 2;

	literal_filter[3].filter.accepted = "0123456789";
	literal_filter[3].accept = 1;

	int literal_accepted[2] = {1,2};

	DFA literal;
	literal.accepted_count = 2;
	literal.accepted_states = literal_accepted;
	literal.filter = literal_filter;

	char str[] = "191.231L123123";
	printf("matching string for literal: %.*s\n", match(&literal, str), str);


	Filter_Node comment_filter[9];

	for(int i = 0; i < 9; i++)
	{
		comment_filter[i] = new_node();
	}

	comment_filter[0].filter.accepted = "/";
	comment_filter[0].accept = 1;

	comment_filter[1].filter.accepted = "*";
	comment_filter[1].accept = 3;
	comment_filter[1].next = &comment_filter[6];

	comment_filter[2].filter.reject_all = 1;

	comment_filter[3].filter.accepted = "*";
	comment_filter[3].accept = 4;
	comment_filter[3].next = &comment_filter[7];

	comment_filter[4].filter.accepted = "/";
	comment_filter[4].accept = 5;
	comment_filter[4].next = &comment_filter[8];

	comment_filter[5].filter.reject_all = 1;

	//create path from 1 to 2
	comment_filter[6].filter.rejected = "*";
	comment_filter[6].accept = 2;

	//create path from 3 to 3
	comment_filter[7].filter.rejected = "*";
	comment_filter[7].accept = 3;

	//create path from 4 to 3
	comment_filter[8].filter.rejected = "/";
	comment_filter[8].accept = 3;

	int comment_accepted[] = {5};

	DFA comment;
	comment.accepted_count = 1;
	comment.accepted_states = comment_accepted;
	comment.filter = comment_filter;

	char comment_str[] = "/*This\nis\na\nmultiline comment\nand\nit works*/";
	printf("matching string for multiline comment: %.*s\n", match(&comment, comment_str), comment_str);
}