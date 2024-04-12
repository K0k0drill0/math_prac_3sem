i = 0
res = 0

while (res < 10**18):
    res *= 37
    res += 257
    i += 1

print(i)
