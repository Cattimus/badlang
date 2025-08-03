import re
from enum import Enum

#regexes we'll need to parse our language
line_comment = re.compile("//.*")
multiline_comment = re.compile("(?s)/\\*.*\\*/") #comments are ignored by the parser

#these are necessary for our langauge to compile
literal = re.compile("\\d+\\.*\\d*")
identifier = re.compile("[a-zA-z_]\\w*")

operators = "+-/*="

class Type(Enum):
	operator = 1
	literal = 2
	identifier = 3

class Binary_Expression:
	left = None
	right = None
	operator = None

class Identifier:
	name = None
	value = None

#first we run our program through a lexer
def lex(s):
	symbols = []
	i = 0
	while i < len(s):

		#literal (number)
		if s[i].isnumeric():
			l = literal.search(s, i)
			if l != None:
				l = l.group(0)
				i += len(l)
				symbols.append((Type.literal, int(l)))

		#search for comments
		elif s[i] == '/':
			sl = line_comment.search(s, i)
			ml = multiline_comment.search(s, i)

			if sl != None:
				i += len(sl.group(0))
			
			elif ml != None:
				i += len(ml.group(0))
		
		#operator
		elif s[i] in operators:
			symbols.append((Type.operator, s[i]))
			i += 1
		
		#identifier
		elif s[i].isalpha():
			l = identifier.search(s, i)
			if l != None:
				l = l.group(0)
				i += len(l)
				symbols.append((Type.identifier, l))

		elif s[i].isspace():
			i += 1
			continue

		#hit unidentified symbol
		else:
			return None
		
	return symbols

labels = []
		
# get symbols from program/string input
program = "14 + 8 + 21 - 9 + 13"