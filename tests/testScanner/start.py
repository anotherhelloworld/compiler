import subprocess
import sys
# subprocess.call(["cmake-build-debug/compiler", "tests/test001.in"])
count = int(sys.argv[1])
for i in range(0, count):
    testnum = 'test' + '0' * (len(str(100)) - len(str(i))) + str(i)
    fo = open(testnum + '.out')
    # print(testnum)
    ans = (subprocess.check_output(['/Users/a1/Documents/stuff/cstuff/compiler/bin/Debug/Compiler', '-s', '/Users/a1/Documents/stuff/cstuff/compiler/tests/testScanner/'
     + testnum + '.in'])).split()
    expectedAns = fo.read().split()
    # print(ans)
    if (ans == expectedAns):
        print("OK")
    else:
        print (i)
        for j in range(len(ans)):
            if (ans[j] != expectedAns[j]):
                print("    " + str(j) + " " + ans[j] + " " + expectedAns[j])
    fo.close()