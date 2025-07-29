
#DFA of a string that starts with a, contains only a and b, ends with b
g =[
	{'a': 0, 'b': 1},
	{'b': 1, 'a': 0}
]

str = 'aaabaaabbbababbb'
state = 0
for c in str:
	try:
		state = g[state][c]
	except:
		print("string was rejected")
		exit(0)


if state == 1:
	print("String was accepted")
else:
	print("String was rejected")