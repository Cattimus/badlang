
#DFA that describes a comment string
g = [
	lambda c: 1 if c == '/' else -1,
	lambda c: 1 if c != '\n' else 2,
	lambda c: -1
]

def check_identifier(c):
	if c.isnumeric() or c.isalpha() or c == '_':
		return 0
	elif c.isspace():
		return 1
	elif c == ';':
		return 2
	else:
		return -1

g2 = [
	check_identifier
]

comment = "this_is_an_identifier;"
state = 0
for c in comment:
	state = g2[state](c)
	if state == -1:
		print("String is rejected")
		exit(state)

print("String is accepted")