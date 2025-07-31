import re
from enum import Enum

#regexes we'll need to parse our language
line_comment = re.compile("//.*")
multiline_comment = re.compile("(?s)/\*.*\*/") #comments are ignored by the parser

#these are necessary for our langauge to compile
literal = re.compile("\d+\.*\d*")
identifier = re.compile("[a-zA-z_]\w*")

operators = "+-/*="

class Type(Enum):
	operator = 1
	literal = 2
	identifier = 3

class Node:
	left = None
	right = None
	value = None

program = "14 + 21 - 9 + 13"

#first we run our program through a lexer
def lex(s):
	symbols = []

	for i in range(0,len(s)):

		#literal (number)
		if s[i].isnumeric():
			l = literal.search(s, i)
			if l != None:
				l = l.expand()
				i += len(l)
				symbols.append((Type.literal, int(l)))

		#search for comments
		elif s[i] == '/':
			sl = line_comment.search(s, i)
			ml = multiline_comment.search(s, i)

			if sl != None:
				i += len(sl.expand())
			
			elif ml != None:
				i += len(ml.expand())
		
		#operator
		elif s[i] in operators:
			i += 1
			symbols.append((Type.operator, l))
		
		#identifier
		elif s[i].isalpha():
			l = literal.search(s, i)
			if l != None:
				l = l.expand()
				i += len(l)
				symbols.append((Type.identifier, l))

		#hit unidentified symbol
		else:
			return None
		
	return symbols

		

