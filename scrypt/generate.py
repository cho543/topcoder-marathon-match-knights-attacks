#!/usr/bin/env python3
import math
import tempfile
import os
import stat
import subprocess

helper = r'''\
#!/bin/bash
read s
echo $s >&2
head -n $s >&2
echo $s
perl -e "print( ('.' x $s . \"\\n\") x $s)"
'''

def generate_input(seed, tester='tester.jar', helper=None):
    cmd = [ 'java', '-jar', tester, '-seed', str(seed), '-exec', helper ]
    proc = subprocess.run(cmd, stdout=subprocess.PIPE)
    lines = proc.stdout.decode().splitlines()
    assert lines[-1].startswith('Score = ')
    assert len(lines) == int(lines[0]) + 2
    return '\n'.join(lines[: -2])

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('seed', type=int)
    parser.add_argument('--tester', default='tester.jar')
    args = parser.parse_args()

    fh = tempfile.NamedTemporaryFile(mode='w', suffix='.sh', delete=False)
    try:
        fh.write(helper)
        fh.close()
        os.chmod(fh.name, os.stat(fh.name).st_mode | stat.S_IXUSR)
        s = generate_input(args.seed, tester=args.tester, helper=fh.name)
    finally:
        os.unlink(fh.name)
    print(s)
