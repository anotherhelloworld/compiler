import sys
import os

class Enumerator:

    def __init__(self, start, first, last):
        self.start = start
        self.first = first
        self.last = last

    def Main(self):
        for i in range(self.first, self.last):
            os.system("mv test{0}.in test{1}.in".format(str(i).zfill(3), str(self.start).zfill(3)))
            os.system("mv test{0}.out test{1}.out".format(str(i).zfill(3), str(self.start).zfill(3)))
            self.start += 1

if __name__ == "__main__":
    enumerator = Enumerator(*[int(x) for x in sys.argv[1:]])
    enumerator.Main()
