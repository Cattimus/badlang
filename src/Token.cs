namespace main;

enum Token_Type
{
	Invalid = -1,
	OpenBracket,
	CloseBracket,
	OpenSquareBracket,
	CloseSquareBracket,
	Separator,
	Identifier,
	Literal,
	Obj,
	Arr,
	Bool,
	Null,
};

//token for lexer prior to parsing
class Token
{
	Token_Type t;
	string? raw_data;

	public Token()
	{
		t = Token_Type.Invalid;
		raw_data = null;
	}

	public Token(Token_Type t, string? raw_data)
	{
		this.t = t;
		this.raw_data = raw_data;
	}
};