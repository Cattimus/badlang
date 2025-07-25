#include <stdio.h>
#include <string.h>


//filters
int is_num(char* c)
{
	return *c >= '0' && *c <= '9';
}

int is_whitespace(char* c)
{
	return *c == ' ' || *c == '\n' || *c == '\t';
}

int is_plus(char* c)
{
	return *c == '+';
}

int is_minus(char* c)
{
	return *c == '-';
}

int main() 
{

}