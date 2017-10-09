import subprocess
import sys
import os

count = int(sys.argv[1])
for i in range(0, count):
    testnum = 'test' + '0' * (len(str(100)) - len(str(i))) + str(i)
    fo = open(testnum + '.out')

    expectedAns = fo.read()
    os.system('/Users/a1/Documents/stuff/cstuff/compiler/bin/Debug/Compiler -g {0}.in > temp.asm'.format(testnum))
    os.system('nasm -f macho temp.asm -o test.o && ld -arch i386 -macosx_version_min 10.7.0 -o a.out test.o -lc /usr/lib/crt1.o -no_pie')
    # os.system('cat temp.asm > outs/{0}.asm'.format(testnum))
    ans = subprocess.check_output(['/Users/a1/Documents/stuff/cstuff/compiler/tests/testGenerationInt/a.out'])
    
    if (ans == expectedAns):
        print("OK")
    else:
        print ("FALSE {0}".format(i))
    fo.close()