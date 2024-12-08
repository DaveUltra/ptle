from math import *

from dolphin import memory


class Vec4:
	data = []

	def __init__(self):
		self.data = [0.0, 0.0, 0.0, 0.0]

	def set(self, v):
		self.data = v.data.copy()

	def add(self, o):
		self.data[0] += o.data[0]
		self.data[1] += o.data[1]
		self.data[2] += o.data[2]
		self.data[3] += o.data[3]

	def mul1(self, f):
		self.data[0] *= f
		self.data[1] *= f
		self.data[2] *= f
		self.data[3] *= f

	def length(self):
		return sqrt(self.data[0] * self.data[0] + self.data[1] * self.data[1] + self.data[2] * self.data[2])

	def normalize(self):
		l = self.length()
		if l == 0:
			return
		l = 1 / l
		self.data[0] *= l
		self.data[1] *= l
		self.data[2] *= l

class Mat4:
	data = []

	def __init__(self):
		self.data = [[0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0], [0.0, 0.0, 0.0, 0.0]]
	def fromArray(data):
		m = Mat4()
		m.data = data
		return m

	def load(self, addr):
		self.data = []
		for i in range(4):
			row = []
			for j in range(4):
				row.append(memory.read_f32(addr + i * 16 + j * 4))
			self.data.append(row)

	def store(self, addr):
		for i in range(4):
			for j in range(4):
				memory.write_f32(addr + i * 16 + j * 4, self.data[i][j])

	def mul(self, other):
		res = Mat4()
		for i in range(4):
			for j in range(4):
				for k in range(4):
					res.data[i][j] += self.data[i][k] * other.data[k][j]
		return res

	def mul1(self, v):
		for i in range(3):
			for j in range(3):
				self.data[i][j] *= v

	def mulVec(self, v):
		r = Vec4()
		for i in range(4):
			for k in range(4):
				r.data[i] += v.data[k] * self.data[k][i]
		return r

	def getRow(self, i):
		v = Vec4()
		v.data[0] = self.data[i][0]
		v.data[1] = self.data[i][1]
		v.data[2] = self.data[i][2]
		v.data[3] = self.data[i][3]
		return v
	def setRow(self, i, v):
		self.data[i][0] = v.data[0]
		self.data[i][1] = v.data[1]
		self.data[i][2] = v.data[2]
		self.data[i][3] = v.data[3]

	def rotateX(a):
		c = cos(a)
		s = sin(a)
		res = [
			[1, 0,  0, 0],
			[0, c, s, 0],
			[0, -s,  c, 0],
			[0, 0,  0, 1],
		]
		return Mat4.fromArray(res)

	def rotateZ(a):
		c = cos(a)
		s = sin(a)
		res = [
			[c, -s,  0, 0],
			[s, c, 0, 0],
			[0, 0,  1, 0],
			[0, 0,  0, 1],
		]
		return Mat4.fromArray(res)
