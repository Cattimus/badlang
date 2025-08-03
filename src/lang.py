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

class Node:
	left = None
	right = None
	lex_type = None
	value = None
	name = None

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

#create visual representation of tree
def ident_from_node(root):
	n = f'{root.name}'
	return n

def tree_to_graph(root):
	global labels
	output = ""

	label = root.name + f'[label="{root.value}"]'
	if not label in labels:
		labels.append(label)

	if root.left != None:
		output += root.name + " -- " + root.left.name + "\n"
		output += tree_to_graph(root.left)
	
	if root.right != None:
		output += root.name + " -- " + root.right.name + "\n"
		output += tree_to_graph(root.right)

	return output

def tree_str(root):
	output = "graph {\n"
	output += tree_to_graph(root)
	for label in labels:
		output += label + "\n"
	output += "}"
	return output

def node_from_tuple(tup):
	n = Node()
	n.lex_type = tup[0]
	n.value = tup[1]
	return n

def next_name(name):
	return chr(ord(name)+1)

def assemble_tree(symbols):
	root = node_from_tuple(symbols[1])
	root.name = 'a'
	root.left = node_from_tuple(symbols[0])
	root.left.name = 'b'
	root.right = node_from_tuple(symbols[2])
	root.right.name = 'c'

	current_name = 'd'

	for i in range(3, len(symbols)):
		c = node_from_tuple(symbols[i])
		c.name = current_name
		current_name = next_name(current_name)

		if c.lex_type == Type.operator:
			c.right = root
			root = c

		elif c.lex_type == Type.identifier or c.lex_type == Type.literal:
			root.left = c

	return root

def eval_tree(root):
	stack = []

	#create a stack with our operators
	while root.left and root.right:
		stack.append(root)
	
		if root.right.lex_type == Type.identifier or root.right.lex_type == Type.literal:
			break

		root = root.right

	result = 0
	while stack:
		c = stack.pop()
		left = c.right.value
		right = c.left.value

		if c.right.lex_type == Type.operator:
			left = result

		if c.value == '+':
			result = left + right
		elif c.value == '-':
			result = left - right

	return result

#get identifiers from symbols
def get_identifiers(symbols):
	identifiers = {}
	for symbol in symbols:
		if symbol[0] == Type.identifier:
			identifiers[symbol[1]] = 0
	
	return identifiers
		
# get symbols from program/string input
program = "14 + 8 + 21 - 9 + 13"
symbols = lex(program)
identifiers = get_identifiers(symbols)
root = assemble_tree(symbols)
print("interpreted: ", eval_tree(root))
print("reference: ", eval(program))