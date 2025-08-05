
using main;

var l = Lexer.Process("12321.12312");

foreach (var match in l)
{
	Console.WriteLine(match.raw_data);
}