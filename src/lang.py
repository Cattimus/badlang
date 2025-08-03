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

class Unit:
	data_type = None

#binary expressions
class Binary_Expression(Unit):
	value = None
	left = None
	right = None
	operator = None

	def __init__(self):
		self.data_type = Type.expression

	def eval(self):
		if self.left.data_type == Type.expression:
			self.left.eval()

		if self.right.data_type == Type.expression:
			self.right.eval()

		if self.operator == '+':
			self.value = self.left.value + self.right.value
		elif self.operator == '-':
			self.value = self.left.value - self.right.value
		elif self.operator == '=':
			self.left.value = self.right.value

#identifier (has a name and a value)
class Identifier(Unit):
	name = None
	value = None

	def __init__(self, name, value):
		self.name = name
		self.value = value
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

		elif s[i].isspace():
			i += 1
			continue

		#hit unidentified symbol
		else:
			return None
		
	return symbols

#parse data into executable format
def parse_symbols(symbols):
	expressions = []
	stack = []
	identifiers = {}

	for symbol in symbols:
		if symbol[0] == Type.literal:
			if stack:
				e = stack.pop()
				e.right = Literal(symbol[1])
				expressions.append(e)
			else:
				stack.append(Literal(symbol[1]))
		
		elif symbol[0] == Type.operator:
			if stack:
				e = Binary_Expression()
				e.operator = symbol[1]
				e.left = stack.pop()
				stack.push(e)
			else:
				print("Error - operator with no expression")
				exit(-1)

		elif symbol[0] == Type.identifier:
			if not identifiers[symbol[1]]:
				identifiers[symbol[1]] = Identifier(symbol[1], 0)

			if stack:
				e = stack.pop()
				e.right = identifiers[symbol[0]]
				expressions.append(e)
			else:
				stack.append(identifiers[symbol[1]])

	return (expressions, identifiers)
