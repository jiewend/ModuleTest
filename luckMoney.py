import random
def luckMoney(total, num):
    print("total : " +  str(total) + " num : " + str(num))
    if num * 0.01 == total:
        for i in range(num):
            print(0.01)
    else:
        maxnum = total / 0.01
        each = set()
        while len(each) <= num - 1:
            each.add(random.randint(1, maxnum-1))
        each = list(each)
        each.append(maxnum)
        each.sort()
        luckM = [each[0] / float(100)]
        for i in range(1, len(each)):
            luckM.append((each[i] - each[i - 1]) / float(100))
        print(luckM)
        print(sum(luckM))




if __name__ == '__main__':
    luckMoney(200, 10)
