#!/usr/bin/env python3

s = int(input())
board = [ input() for _ in range(s) ]

print('param s :=', s, ';')
print('param board :=')
for y in range(s):
    for x in range(s):
        print(y + 1, x + 1, int(board[y][x]))
print(';')
print('end;')
