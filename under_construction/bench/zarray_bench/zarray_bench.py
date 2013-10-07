
if __name__ == "__main__":
    vals = [];
    good = True;
    for i in range(10000000):
        vals.append(i)
    for i in range(10000000):
        if vals[i] != i:
            good = False
    for i in range(10000000):
        vals.pop()
    if not good:
        print "failed"
