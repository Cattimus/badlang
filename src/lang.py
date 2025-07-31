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
				symbols.append((Type.literal, l))

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

# get symbols from program/string input
program = "14 + l + 21 - 9 + 13"
symbols = lex(program)

root = Node()
root.name = 'a'
cursor = root

current_name = 'b'

for symbol in symbols:
	c = Node()
	c.lex_type = symbol[0]
	c.value = symbol[1]
	c.name = current_name
	current_name = chr(ord(current_name)+1)

	if symbol[0] == Type.literal or symbol[0] == Type.identifier:
		if cursor.left != None and cursor.right == None:
			cursor.right = c
			cursor = cursor.right
		elif cursor.left == None:
			cursor.left = c

	elif symbol[0] == Type.operator:
		if cursor.lex_type == None:
			cursor.lex_type = c.lex_type
			cursor.value = c.value
		elif cursor.right == None:
			cursor.right = c
			cursor = cursor.right

		#rotate values
		else:
			temp = cursor
			cursor = c
			cursor.left = temp

print(tree_str(root))