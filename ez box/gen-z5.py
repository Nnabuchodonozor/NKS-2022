#!/usr/bin/python3

import random
import hashlib
import base64
import math
import numpy

def spn(x, k, sb):
    y = x
    for i in range(3):
        '''key'''
        u = y ^ k[i]
        '''sboxes'''
        u = (sb[(u >> 0) & 0xf] << 0) ^ \
            (sb[(u >> 4) & 0xf] << 4) ^ \
            (sb[(u >> 8) & 0xf] << 8) ^ \
            (sb[(u >> 12) & 0xf] << 12)
        '''permutation'''
        y = 0
        y ^= (((u) & 0x8421))
        y ^= (((u) & 0x2100) >> 6)
        y ^= (((u) & 0x4210) >> 3)
        y ^= (((u) & 0x0842) << 3)
        y ^= (((u) & 0x0084) << 6)
        y ^= (((u) & 0x0008) << 9)
        y ^= (((u) & 0x1000) >> 9)
    '''last round'''
    u = y ^ k[3]
    u = (sb[(u >> 0) & 0xf] << 0) ^ \
        (sb[(u >> 4) & 0xf] << 4) ^ \
        (sb[(u >> 8) & 0xf] << 8) ^ \
        (sb[(u >> 12) & 0xf] << 12)
    u = u ^ k[4]
    return u


def generate(userid, msk):
    """Generate sbox
    """
    sb = numpy.random.RandomState(seed=int(userid)).permutation(16)
    secret = hashlib.sha256((str(userid) + msk).encode('utf-8')).hexdigest()
    key = [int(secret[4 * i:4 * i + 4], 16) for i in range(5)]

    return userid, key, sb


def printspn(key, sb, fname):
    with open(fname, 'w') as f:
        for x in range(0x10000):
            y = spn(x, key, sb)
            f.write('{:04x}\n'.format(y))


'''MSK'''
msk = 'TODO'

# with open('ids.txt', 'r') as f:
studentid = 92202
userid, key, sb = generate(studentid, msk)

sbstr = ''.join(['{:01x}'.format(y) for y in sb])
keystr = ['{:04x}'.format(k) for k in key]
print(userid, keystr, sbstr)

# fname = '.'.join([userid, sbstr, 'dat'])
# printspn(key, sb, fname)


