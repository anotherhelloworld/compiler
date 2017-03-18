import subprocess
import sys
import pickle
# subprocess.call(["cmake-build-debug/compiler", "tests/test001.in"])
count = int(sys.argv[1])
for i in range(0, count):
    testnum = 'test' + '0' * (len(str(100)) - len(str(i))) + str(i)
    fo = open(testnum + '.out', 'w')
    ans = (subprocess.check_output(['/Users/a1/Documents/stuff/c++ stuff/compiler/bin/Debug/Compiler', '-p', '/Users/a1/Documents/stuff/c++ stuff/compiler/tests/testParser/'
     + testnum + '.in']))
    fo.write(ans)
    fo.close()