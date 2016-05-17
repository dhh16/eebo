import os


def makedir(d):
    if not os.path.isdir(d):
        os.makedirs(d)
