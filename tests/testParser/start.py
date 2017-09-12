import subprocess
import sys
# subprocess.call(["cmake-build-debug/compiler", "tests/test001.in"])
count = int(sys.argv[1])
for i in range(0, count):
    testnum = 'test' + '0' * (len(str(100)) - len(str(i))) + str(i)
    fo = open(testnum + '.out')
    # print(testnum)
    ans = (subprocess.check_output(['/Users/a1/Documents/stuff/cstuff/compiler/bin/Debug/Compiler', '-p', '/Users/a1/Documents/stuff/cstuff/compiler/tests/testParser/'
     + testnum + '.in']))
    expectedAns = fo.read()
    # print(ans)
    if (ans == expectedAns):
        print("OK")
    else:
        print ("FALSE {0}".format(i))
    fo.close()