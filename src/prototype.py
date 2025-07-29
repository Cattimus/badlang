class filter:
	state = {}
	default = 0

	def eval(self, c):
		if c in self.state:
			return self.state[c]
		else:
			return self.default
		
class DFA:
	filters = []
	accepted_states = []
	initial = ''

	def eval(self, str):
		state = 0

		if self.initial != '' and str[0] != self.initial:
			return False
		
		for c in str[1:]:
			state = self.filters[state].eval(c)
			if state == -1:
				return False
			
		if state in self.accepted_states:
			return True
		else:
			return False

#DFA for comment structure
comment = DFA()
comment_0 = filter()
comment_0.state['/'] = 1
comment_0.default = -1
comment.filters.append(comment_0)
comment_1 = filter()
comment_1.state['\n'] = 2
comment_1.default = 1
comment.filters.append(comment_1)
comment.accepted_states.append(2)
comment.initial = '/'
print(comment.eval("//This is a comment\n"))
print(comment.eval("This is not a comment\n"))
print()

#DFA for identifier structure
identifier = DFA()
identifier_0 = filter()
identifier_0.default = -1
identifier_0.state[';'] = 2
identifier_0.state[' '] = 1
identifier_0.state['\n'] = 1
identifier_0.state['\t'] = 1
identifier_0.state['_'] = 0

c = 'a'
while(c < 'z'):
	identifier_0.state[c] = 0
	c = chr(ord(c)+1)
identifier_0.state[c] = 0

c = 'A'
while(c < 'Z'):
	identifier_0.state[c] = 0
	c = chr(ord(c)+1)
identifier_0.state[c] = 0

c = '0'
while(c < '9'):
	identifier_0.state[c] = 0
	c = chr(ord(c)+1)
identifier_0.state[c] = 0

identifier.accepted_states = [1,2]
identifier.filters.append(identifier_0)
print(identifier.eval("this_is_an_identifier9;"))
print(identifier.eval("this isn't an identifier"))
print()

literal = DFA()
literal_0 = filter()
literal_0.default = -1

c = '0'
while(c < '9'):
	literal_0.state[c] = 0
	c = chr(ord(c)+1)
literal_0.state[c] = 0

literal_0.state[' '] = 2
literal_0.state['\t'] = 2
literal_0.state['\n'] = 2

literal_0.state[';'] = 3

literal_0.state['.'] = 1

literal_1 = filter()
literal_1.default = -1

c = '0'
while(c < '9'):
	literal_1.state[c] = 0
	c = chr(ord(c)+1)
literal_1.state[c] = 0
literal.filters.append(literal_0)
literal.filters.append(literal_1)
literal.accepted_states = [2,3]
print(literal.eval("19.18231;"))
print(literal.eval("123.not a literal"))