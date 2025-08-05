using System.Text.RegularExpressions;

namespace main;

static class Lexer
{
	private static Regex number = new Regex("-?\\d+\\.?\\d*", RegexOptions.Compiled);
	private static Regex cond = new Regex("true|false", RegexOptions.Compiled);
	private static Regex none = new Regex("null", RegexOptions.Compiled);

	public static List<Token> Process(string input)
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
				if (match.Index == i)
				{
					l.Add(new Token(Token_Type.Literal, match.ToString()));
					i += match.Length;
				}
			}

			else if (Char.IsAsciiLetter(c))
			{
				var match = cond.Match(input, i);
				if (match.Index == i)
				{
					l.Add(new Token(Token_Type.Bool, match.ToString()));
					i += match.Length;
				}

				match = none.Match(input, i);
				if (match.Index == i)
				{
					l.Add(new Token(Token_Type.Null, match.ToString()));
					i += match.Length;
				}
			}

		}
		return l;
	}
};