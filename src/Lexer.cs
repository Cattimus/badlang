using System.Text.RegularExpressions;

namespace main;

static class Lexer
{
	private static Regex number = new Regex("-?\\d+\\.?\\d*", RegexOptions.Compiled);
	private static Regex cond = new Regex("true|false", RegexOptions.Compiled);
	private static Regex none = new Regex("null", RegexOptions.Compiled);
	private static Regex str = new Regex(""".*?(?<!\\)""", RegexOptions.Compiled);

	public static List<Token> Process(string input)
	{
		List<Token> l = new List<Token>();
		int line = 0;

		for (int i = 0; i < input.Length; i++)
		{
			char c = input[i];
			if (Char.IsWhiteSpace(c))
			{
				if (c == '\n')
				{
					line++;
				}
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
						var match = str.Match(input, i);
						if (match.Index == i)
						{
							l.Add(new Token(Token_Type.String, match.ToString()));
							i += match.Length;
						}
						else
						{
							Console.WriteLine("Invalid token on line: {0}", line);
							Environment.Exit(-1);
						}
						break;
					}
			}

			if (Char.IsAsciiDigit(c) || c == '-')
			{
				var match = number.Match(input, i);
				if (match.Index == i)
				{
					l.Add(new Token(Token_Type.Number, match.ToString()));
					i += match.Length;
				}
				else
				{
					Console.WriteLine("Invalid token on line: {0}", line);
					Environment.Exit(-1);
				}
			}

			else if (Char.IsAsciiLetter(c))
			{
				var match = cond.Match(input, i);
				if (match.Index == i)
				{
					l.Add(new Token(Token_Type.Bool, match.ToString()));
					i += match.Length;
					continue;
				}

				match = none.Match(input, i);
				if (match.Index == i)
				{
					l.Add(new Token(Token_Type.Null, match.ToString()));
					i += match.Length;
					continue;
				}

				//This code should never be reached if we match a valid case
				Console.WriteLine("Invalid token on line: {0}", line);
				Environment.Exit(-1);
			}
		}
		return l;
	}
};