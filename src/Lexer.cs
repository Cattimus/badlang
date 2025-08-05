using System.Text.RegularExpressions;

namespace main;

static class Lexer
{
	private static Regex number = new Regex("-?\\d+\\.?\\d*");
	private static Regex condnull = new Regex("true|false|null");

	public static List<Token> process(ref string input)
	{
		List<Token> l = new List<Token>();

		for (int i = 0; i < input.Length; i++)
		{
			char c = input[i];
			if (Char.IsWhiteSpace(c))
			{
				continue;
			}

			switch (c)
			{
				case ':':
				case ',':
					{
						l.Add(new Token(Token_Type.Separator, null));
						break;
					}
				case '{':
					{
						l.Add(new Token(Token_Type.OpenBracket, null));
						break;
					}
				case '}':
					{
						l.Add(new Token(Token_Type.CloseBracket, null));
						break;
					}
				case '[':
					{
						l.Add(new Token(Token_Type.OpenSquareBracket, null));
						break;
					}
				case ']':
					{
						l.Add(new Token(Token_Type.CloseSquareBracket, null));
						break;
					}

				case '"':
					{
						//handle string literal here
						break;
					}
			}

			if (Char.IsAsciiDigit(c) || c == '-')
			{
				var match = number.Match(input, i);
				if (match.Index != 0)
				{
					//error out here
					Console.WriteLine("Match didn't work");
				}
			}

			else if (Char.IsAsciiLetter(c))
			{
				//check for bool/null
			}

		}
		return l;
	}
};