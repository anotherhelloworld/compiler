import subprocess
# subprocess.call(["cmake-build-debug/compiler", "tests/test001.in"])
for i in range(0, 31):
    testnum = 'test' + '0' * (len(str(100)) - len(str(i))) + str(i)
    fo = open(testnum + '.out')
    # print(testnum)
    ans = (subprocess.check_output(['/Users/a1/Documents/stuff/c++ stuff/compiler/bin/Debug/Compiler', '-s', '/Users/a1/Documents/stuff/c++ stuff/compiler/tests/'
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