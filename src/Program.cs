using System.Text.RegularExpressions;

enum Token_Type
{
	invalid = -1,
	openBracket,
	closeBracket,
	openSquareBracket,
	closeSquareBracket,
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

	public Token(Token_Type t, string raw_data)
	{
		this.t = t;
		this.raw_data = raw_data;
	}
};

static class Lexer
{
	public static List<Token> process(ref string input)
	{
		List<Token> l = new List<Token>();

		foreach (char c in input)
		{
			switch (c)
			{
				case ':':
				case ',':
					{
						l.Add(new Token(Token_Type.separator, c.ToString()));
						break;
					}
				case '{':
					{
						l.Add(new Token(Token_Type.openBracket, c.ToString()));
						break;
					}
				case '}':
					{
						l.Add(new Token(Token_Type.closeBracket, c.ToString()));
						break;
					}
				case '[':
					{
						l.Add(new Token(Token_Type.openSquareBracket, c.ToString()));
						break;
					}
				case ']':
					{
						l.Add(new Token(Token_Type.closeSquareBracket, c.ToString()));
						break;
					}

				
			}
		}

			return l;
		}
	};