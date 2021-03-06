#!/usr/bin/python

import binascii

p = 205023009136450173084188261524390005311
q = 300941700773930581581777121246681821097

N = p * q

r = (p - 1) * (q - 1)

e = 65537

# Below we first need to find d.
def ext_Euclid(n, m):
	if (m == 0):
		return 1, 0
	else:
		x, y = ext_Euclid(m, n % m)
		x, y = y, (x - (n // m) * y)
		return x, y

d, d1 = ext_Euclid(e, r)

while d < 0:
	d = d + r

c = int("16a8344aeb9a2d1cd449e22acd976a1a712a51982ba0151355394a841f5e13b", 16)

ciphertext = "16a8344aeb9a2d1cd449e22acd976a1a712a51982ba0151355394a841f5e13b"

plaintext = pow(c, d, N)

print("The plaintext is: ")
print(plaintext)

hex_str = hex(plaintext)
print("We convert it into hexadecimal representation: ")
print(hex_str)

print("The final result is: ")
print(binascii.unhexlify(hex_str.replace("0x", "")))