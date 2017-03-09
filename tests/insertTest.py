import sys
import os


class Insert:
    def __init__(self):
        return

    def Main(self, number, count):
        for i in range(count, number - 1, -1):
            testNum = '0' * (len(str(100)) - len(str(i))) + str(i)
            newTestnum = '0' * (len(str(100)) - len(str(i + 1))) + str(i + 1)
            fileName = "test{0}.in".format(testNum);
            newFileName = "test{0}.in".format(newTestnum)
            print(fileName, newFileName)
            os.rename(fileName, newFileName)
            fileName = "test{0}.out".format(testNum);
            newFileName = "test{0}.out".format(newTestnum)
            os.rename(fileName, newFileName)
            print(fileName, newFileName)

if __name__ == "__main__":
    ins = Insert()
    ins.Main(int(sys.argv[1]), int(sys.argv[2]))

