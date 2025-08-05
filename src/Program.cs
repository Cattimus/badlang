
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using main;

string example_text = @"{
	""date"": 14,
	""thing"": null,
	""response"": true,
	""do?"": false,
	""my \""string\"" thing"": 123213,
	""arr"" : [
		""wow"", ""it's"", ""working"", true, false, 1281.12312, -1231.23
	]
}";

Console.WriteLine(example_text);
var l = Lexer.Process(example_text);

foreach (var match in l)
{
	Console.WriteLine(match);
}