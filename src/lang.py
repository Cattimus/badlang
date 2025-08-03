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
	expression = 4
	assignment = 5
	separator = 6

class Unit:
	data_type = None

#binary expressions
class Binary_Expression(Unit):
	left = None
	right = None
	operator = None

	def __init__(self):
		self.data_type = Type.expression

#identifier (has a name and a value)
class Identifier(Unit):
	name = None

	def __init__(self, name, value):
		self.name = name
		self.data_type = Type.identifier

class Literal(Unit):
	value = None

	def __init__(self, value):
		self.value = value
		self.data_type = Type.literal

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
		
		elif s[i] == ';':
			symbols.append((Type.separator, ';'))
			i += 1

		elif s[i].isspace():
			i += 1
			continue

		#hit unidentified symbol
		else:
			return None
		
	return symbols


def parse_expression(symbols):
	global variables
	stack = []
	expressions = []
	total = 0

	for symbol in symbols:
		if symbol[0] == Type.literal or symbol[0] == Type.operator:
			stack.append(symbol[1])
		
		if symbol[0] == Type.identifier:
			if not symbol[1] in variables:
				print("undeclared expression: ", symbol[1])
				exit(-1)
			else:
				stack.append(variables[symbol[1]])
		
		if symbol[0] == Type.separator:
			break
	
	total = len(stack)
	while(stack):

		if expressions:
			e = Binary_Expression()
			e.right = expressions.pop()
			e.operator = stack.pop()
			e.left = stack.pop()
			expressions.append(e)
		
		else:
			e = Binary_Expression()
			e.right = stack.pop()
			e.operator = stack.pop()
			e.left = stack.pop()
			expressions.append(e)
	return (expressions, total)







program = '''
var = 14 + 21 + 19 + 41 - 12;
1 + 2 + 3 + 4 + 5 + 6;
val = var + 4;
'''
symbols = lex(program)
expressions, identifiers = parse_symbols(symbols)
expressions[0].eval()
print(identifiers['var'])