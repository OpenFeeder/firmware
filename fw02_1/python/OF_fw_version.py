# -*- coding: utf-8 -*-
"""
Éditeur de Spyder

Ceci est un script temporaire.
"""

from tkinter import Tk, Frame, Label, Button, N, S, W, E, DISABLED, NORMAL
import sys
import os
from functools import partial

class ver(Frame):

    def __init__(self, *args, firmware={}, file='', **kwargs):

        Frame.__init__(self, *args, **kwargs)

        self.file = file

        self.current_fw = {}
        self.current_fw['name'] = firmware['name']
        self.current_fw['version_major'] = firmware['version_major']
        self.current_fw['version_minor'] = firmware['version_minor']
        self.current_fw['version_patch'] = firmware['version_patch']

        self.new_fw = {}
        self.new_fw['name'] = firmware['name']
        self.new_fw['version_major'] = firmware['version_major']
        self.new_fw['version_minor'] = firmware['version_minor']
        self.new_fw['version_patch'] = firmware['version_patch']

        Label(self, text='Current').grid(row=0, column=0, columnspan=2)
        Label(self, text=str(self.current_fw['version_major'])).grid(row=0, column=2, columnspan=2, sticky=W+E)
        Label(self, text=str(self.current_fw['version_minor'])).grid(row=0, column=4, columnspan=2, sticky=W+E)
        Label(self, text=str(self.current_fw['version_patch'])).grid(row=0, column=6, columnspan=2, sticky=W+E)

        self.button = []

        self.button.append(Button(self, text='+', command=partial(self.modifyFW, 0)))
        self.button[0].grid(row=1, column=2, sticky=W+E)
        self.button.append(Button(self, text='-', command=partial(self.modifyFW, 1), state=DISABLED))
        self.button[1].grid(row=1, column=3, sticky=W+E)
        self.button.append(Button(self, text='+', command=partial(self.modifyFW, 2)))
        self.button[2].grid(row=1, column=4, sticky=W+E)
        self.button.append(Button(self, text='-', command=partial(self.modifyFW, 3), state=DISABLED))
        self.button[3].grid(row=1, column=5, sticky=W+E)
        self.button.append(Button(self, text='+', command=partial(self.modifyFW, 4)))
        self.button[4].grid(row=1, column=6, sticky=W+E)
        self.button.append(Button(self, text='-', command=partial(self.modifyFW, 5), state=DISABLED))
        self.button[5].grid(row=1, column=7, sticky=W+E)

        self.new_label = []

        Label(self, text='New').grid(row=2, column=0, columnspan=2)
        self.new_label.append(Label(self, text=str(self.new_fw['version_major'])))
        self.new_label[0].grid(row=2, column=2, columnspan=2)
        self.new_label.append(Label(self, text=str(self.new_fw['version_minor'])))
        self.new_label[1].grid(row=2, column=4, columnspan=2)
        self.new_label.append(Label(self, text=str(self.new_fw['version_patch'])))
        self.new_label[2].grid(row=2, column=6, columnspan=2)

        self.button_validate = Button(self, text='Validate', state=DISABLED, command=partial(self.validateFW))
        self.button_validate.grid(row=3, column=2, columnspan=3, sticky=W+E)

        Button(self, text='Skip', command=partial(self.closeWindow)).grid(row=3, column=5, columnspan=3, sticky=W+E)

    def modifyFW(self, idx):

        for i in range(0, 6):
            self.button[i].config(state=DISABLED)
            if i < 3:
                self.new_label[i].config(fg='black')

        if idx == 0:
            self.new_fw['version_major'] = self.current_fw['version_major'] + 1
        elif idx == 1:
            self.new_fw['version_major'] = self.current_fw['version_major'] - 1
        elif idx == 2:
            self.new_fw['version_minor'] = self.current_fw['version_minor'] + 1
        elif idx == 3:
            self.new_fw['version_minor'] = self.current_fw['version_minor'] - 1
        elif idx == 4:
            self.new_fw['version_patch'] = self.current_fw['version_patch'] + 1
        else:
            self.new_fw['version_patch'] = self.current_fw['version_patch'] - 1

        if idx & (1 << 0) == 1:
            for i in range(0, 6, 2):
                self.button[i].config(state=NORMAL)
        else:
            self.button[idx+1].config(state=NORMAL)
            self.new_label[idx//2].config(fg='red')

        self.updateNewFW()

        self.button_validate.config(state=NORMAL)

    def printNewFW(self):

        print('New firmware: {}.{}.{}'.format(self.fw['version_major'], self.fw['version_minor'], self.fw['version_patch']))

    def updateNewFW(self):

        self.new_label[0].config(text=str(self.new_fw['version_major']))
        self.new_label[1].config(text=str(self.new_fw['version_minor']))
        self.new_label[2].config(text=str(self.new_fw['version_patch']))

    def closeWindow(self):

        print('Firmware versioning skipped')
        self.master.destroy()

    def validateFW(self):

        writeVersion(self.new_fw, self.file)

        print('Firmware version updated from {}.{}.{} to {}.{}.{}'.format(self.current_fw['version_major'], self.current_fw['version_minor'], self.current_fw['version_patch'], self.new_fw['version_major'], self.new_fw['version_minor'], self.new_fw['version_patch']))

        self.master.destroy()

def readVersion(file):

    if os.path.isfile(file):

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


def writeVersion(fw, file):

    f = open(file, 'w')

    f.write('/* This file is automatically generated during build process\n')
    f.write(' * under MPLAB X and should not be manually edited */\n\n')

    f.write('#ifndef _APP_VERSION_H\n')
    f.write('#define _APP_VERSION_H\n\n')

    f.write('#define FW_NAME {}\n'.format(fw['name']))
    f.write('#define FW_VERSION_MAJOR {}\n'.format(fw['version_major']))
    f.write('#define FW_VERSION_MINOR {}\n'.format(fw['version_minor']))
    f.write('#define FW_VERSION_PATCH {}\n'.format(fw['version_patch']))

    f.write('\n#endif /* _APP_VERSION_H */\n')

    f.close()


def main():


    print('Updating app_version.h')

    file = sys.argv[1] + r'\..\src\app\app_version.h'
#    file = r'D:\github\openfeeder\firmwares\fw02_1\src\app\app_version.h'

    fw = readVersion(file)

    root = Tk()

    ver(root, firmware=fw, file=file).grid()

    root.mainloop()


if __name__ == '__main__':
    main()
