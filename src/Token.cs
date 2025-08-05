namespace main;

enum Token_Type
{
	Invalid = -1,
	OpenBracket,
	CloseBracket,
	OpenSquareBracket,
	CloseSquareBracket,
	Separator,
	String,
	Number,
	Obj,
	Arr,
	Bool,
	Null,
};

//token for lexer prior to parsing
class Token
{
	public Token_Type t;
	public string? raw_data;

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

	public override string ToString()
	{
		string to_return = "";

		switch (t)
		{
			case Token_Type.Invalid:
				to_return += "Invalid: ";
				break;

			case Token_Type.OpenBracket:
				to_return += "OpenBracket: {";
				break;

			case Token_Type.CloseBracket:
				to_return += "CloseBracket: }";
				break;

			case Token_Type.OpenSquareBracket:
				to_return += "OpenSquareBracket: [";
				break;

			case Token_Type.CloseSquareBracket:
				to_return += "CloseSquareBracket: ]";
				break;

			case Token_Type.Separator:
				to_return += "Separator: ";
				break;

			case Token_Type.String:
				to_return += "String: ";
				break;

			case Token_Type.Number:
				to_return += "Number: ";
				break;

			case Token_Type.Bool:
				to_return += "Bool: ";
				break;

			case Token_Type.Null:
				to_return += "Null: ";
				break;
		}

		if (raw_data != null)
		{
			to_return += raw_data.ToString();
		}

		return to_return;
	}
};