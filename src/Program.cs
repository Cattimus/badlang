enum Token_Type
{
	invalid = -1,
	openbracket,
	closebracket,
	separator,
	identifier,
	literal,
	obj,
	arr
};

//token for lexer prior to parsing
class Token
{
	Token_Type t;
	string raw_data;

	public Token()
	{
		t = Token_Type.invalid;
		raw_data = "";
	}
};

static class Lexer
{
	static List<Token> process(ref string input)
	{
		List<Token> l = new List<Token>();
		return l;
	}
};