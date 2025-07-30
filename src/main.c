#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct
{
	const char* accepted;
	const char* rejected;
	const char* fail;
}filter;

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
int eval_filter(filter f, char c)
{
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


int main() 
{

}