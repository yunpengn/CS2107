#!/usr/bin/python

p = 205023009136450173084188261524390005311
q = 300941700773930581581777121246681821097

N = p * q

r = (p - 1) * (q - 1)

e = 65537

# Below we first need to find d.
def multiplicative_inverse(e, phi):
    d = 0
    x1 = 0
    x2 = 1
    y1 = 1
    temp_phi = phi
    
    while e > 0:
        temp1 = temp_phi / e
        temp2 = temp_phi - temp1 * e
        temp_phi = e
        e = temp2
        
        x = x2- temp1 * x1
        y = d - temp1 * y1
        
        x2 = x1
        x1 = x
        d = y1
        y1 = y
    
    if temp_phi == 1:
        return d + phi

d, d1 = multiplicative_inverse(e, r)

c = int("16a8344aeb9a2d1cd449e22acd976a1a712a51982ba0151355394a841f5e13b", 16)



print(d)