"""
Write in Python (or another higher programming language) a function that prints the row of Fibonacci
"Translate"this program to assembly. You do not need to generate numbers greater than 255.
"""

a = 0
b = 1

print(0)

while b < 255:
    temp = a
    a = b
    b = temp+b

    print(a)