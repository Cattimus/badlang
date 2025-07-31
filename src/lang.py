import re

#regexes we'll need to parse our language
line_comment = re.compile("//.*")
multiline_comment = re.compile("(?s)/\*.*\*/")
literal = re.compile("\d+\.*\d*")
identifier = re.compile("[a-zA-z_]\w*")
operator = re.compile("[+-*/=]")

class Node:
	left = None
	right = None
	value = None

program = "14 + 21 - 9 + 13"
