class filter:
	accepted = []
	rejected = []
	fail = []

	def eval(self, c):
		if self.accepted:
			if c in self.accepted:
				return True
		
		if self.rejected:
			if c in self.rejected:
				return False
			
		if self.fail:
			if c in self.fail:
				return -1
		
		#if we get to this point and we don't have a filter, we should accept the output
		if not self.accepted:
			return True

		return False

		
class DFA:
	filters = []
	accepted_states = []

	def eval(self, str):
		state = 0
		for c in str[1:]:
			for f in self.filters[state]:
				r = f[0].eval(c)
				if r == True:
					state = f[1]
				elif r == -1:
					return False
				
		if state in self.accepted_states:
			return True
		else:
			return False
		
#we'll make a DFA for comment, since it is reasonably complex
#for this we're using the array representation of graphs, since it uses less memory than adjacency matrices\

comment_0 = filter()
comment_0.accepted.append('/')

#we're not actually failing here, we're just passing over the character
comment_1 = filter()
comment_1.rejected.append('\n')

comment_2 = filter()
comment_2.accepted.append('\n')

#we're going to imagine this is a linked list
c = [
	[(comment_0, 1)],
	[(comment_1, 1), (comment_2, 2)]
]

comment = DFA()
comment.accepted_states = [2]
comment.filters = c
print(comment.eval("//This is not a comment\n"))