import re
from enum import Enum

#regexes we'll need to parse our language
line_comment = re.compile("//.*")
multiline_comment = re.compile("(?s)/\*.*\*/") #comments are ignored by the parser

#these are necessary for our langauge to compile
literal = re.compile("\d+\.*\d*")
identifier = re.compile("[a-zA-z_]\w*")
operator = re.compile("[+-*/=]")

class Type(Enum):
	operator = 1
	literal = 2
	identifier = 3

class Node:
	left = None
	right = None
	value = None

program = "14 + 21 - 9 + 13"
