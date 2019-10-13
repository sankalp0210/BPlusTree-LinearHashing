import sys


class LinearHash:

	def __init__(self):
		'''Initialising the class variables.'''

		self.keys = 0
		self.total_blocks = 2
		self.split_idx = 0
		self.idx_hash = 1
		self.bucks = {}
		self.bucks[0] = self.bucks[1] = [[]]


	def add_bucket(self):
		'''Adding a new bucket'''

		self.bucks[len(self.bucks.keys())] = [[]]
		self.total_blocks += 1

		if len(self.bucks.keys()) > 1<<(self.idx_hash+1):
			self.idx_hash += 1
			self.split_idx = 0

		upd_idx = len(self.bucks.keys()) - 1<<self.idx_hash

		self.total_blocks -= len(self.bucks[upd_idx])
		to_update = [val for val in i for i in self.bucks[upd_idx]]

		self.bucks[upd_idx] = [[]]
		self.total_blocks += 1
		self.split_idx += 1

		for val in to_update:
			hash_val = val % (1<<self.idx_hash)

			if self.split_idx > hash_val:
				hash_val = val % (1<<(self.idx_hash+1))

				if len(self.bucks[hash_val][-1]) >= 250:
					self.total_blocks+=1
					self.bucks[hash_val].append([])
				self.bucks[hash_val][-1].append(val)


	def insert(self, val):
		'''Inserting into the hash table'''

		hash_val = val % (1<<self.idx_hash)

		if self.split_idx > hash_val:
			hash_val = val % (1<<(self.idx_hash+1))

		if any(val in i for i in self.bucks[hash_val]):
			return 1

		self.keys += 1

		if len(self.bucks[hash_val][-1]) >= 250:
			self.total_blocks += 1
			self.bucks[hash_val].append([])

		print(val)
		self.bucks[hash_val][-1].append(val)
		if self.keys / (self.total_blocks * 250.0) > 0.75:
			self.add_bucket()


if __name__ == "__main__":

	if len(sys.argv) != 2:
		sys.exit("Usage: python file.py input_file")

	hash_obj = LinearHash()

	with open(sys.argv[1], 'r') as f:
		for line in f:
			hash_obj.insert(int(line))