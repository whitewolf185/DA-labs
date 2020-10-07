from random import *
seed()

count = int(input())
out = open("i.txt", "wt")
ans = open("a.txt", "wt")

array = [[str(randrange(10)) + str(randrange(10)) + str(randrange(10)) + str(randrange(10)) + str(randrange(10)) + str(randrange(10)), randrange(18446744073709551616)] for i in range(count)]

print("\n".join(["\t".join([str(i[0]),str(i[1])]) for i in array]), end = "", file = out)

array.sort(key = lambda i: i[0])

print("\n".join(["\t".join([str(i[0]),str(i[1])]) for i in array]), file = ans)

ans.close()
out.close()