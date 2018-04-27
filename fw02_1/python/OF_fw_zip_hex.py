# -*- coding: utf-8 -*-
"""
Created on Fri Apr 27 10:19:19 2018

@author: jerome
"""

import sys
import os
import zipfile


def readVersion(file):

    fw = {}
    fw['name'] = ''
    fw['version_major'] = -1
    fw['version_minor'] = -1
    fw['version_patch'] = -1

    f = open(file, 'r')

    for line in f:
        substr = line.split()
        if 'FW_NAME' in substr:
            idx = substr.index('FW_NAME')
            fw['name'] = substr[idx+1]
        if 'FW_VERSION_MAJOR' in substr:
            idx = substr.index('FW_VERSION_MAJOR')
            fw['version_major'] = int(substr[idx+1])
        if 'FW_VERSION_MINOR' in substr:
            idx = substr.index('FW_VERSION_MINOR')
            fw['version_minor'] = int(substr[idx+1])
        if 'FW_VERSION_PATCH' in substr:
            idx = substr.index('FW_VERSION_PATCH')
            fw['version_patch'] = int(substr[idx+1])

    f.close()

    return fw


def main():

    print('Zipping hex file')

    hex_file = os.path.join(sys.argv[1], '..', 'hex', 'fw02_1.hex')
    ver_file = os.path.join(sys.argv[1], '..', 'src', 'app', 'app_version.h')

    if os.path.isfile(hex_file) is False:
        print('Hex file not found: {}\n'.format(hex_file))
        return

    if os.path.isfile(ver_file) is False:
        print('Ver file not found: {}\n'.format(ver_file))
        return

    fw = readVersion(ver_file)

    zipfile_name = 'fw02_1_v_{}_{}_{}.zip'.format(fw['version_major'], fw['version_minor'], fw['version_patch'])

    zip_file = os.path.join(sys.argv[1], '..', 'hex', zipfile_name)

    if os.path.isfile(zip_file):
        os.remove(zip_file)

    zipf = zipfile.ZipFile(zip_file, 'w', zipfile.ZIP_DEFLATED)
    zipf.write(hex_file, os.path.basename(hex_file))
    zipf.close()

    print('Hex file added to {}'.format(os.path.realpath(zip_file)))


if __name__ == '__main__':
    main()
