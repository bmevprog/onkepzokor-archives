#!/usr/bin/env python3
import sys

value = {'I': 1, 'V': 5, 'X': 10, 'L': 50, 'C': 100, 'D': 500, 'M': 1000}

def get_year(s):
	return sum(value.get(c, 0) for c in s)

t = int(sys.stdin.readline())
for _ in range(t):
	print(get_year(sys.stdin.readline()))
