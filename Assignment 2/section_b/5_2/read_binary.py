#!/usr/bin/python

bytes = open("encrypted.xor", "rb").read()

for b in bytes:
	print(ord(b))