#! /usr/bin/python

import os, sys, random, pickle, argparse

class Defaults(object):
    map_name = ''
    if sys.platform.startswith('linux') or sys.platform == 'darwin':
        map_name += '.'
        root = {'location': '/', 'truth': False, 'map': '/.wm101'}
    elif sys.platform.startswith('win'):
        root = {'location': 'C:\\\\', 'truth': False, 'map': 'C:\\\\.wm101'}
    map_name += 'wm101'
    map_location = None
    num = None
    size = 'large'
    hidden = True
    fill = False
    char = str(random.getrandbits(int(10000/3)))
    ext = ['', '.txt', '.zip', '.tar', '.exe', '.dll', '.png', '.html', '.pdf',
        '.doc', '.docx', '.mp3', '.mp4', '.rar', '.avi', '.iso', '.xml', 
        '.ppt', '.odt', '.abw', '.tar.gz', '.gif', '.jar', '.torrent']

    def __init__(self):
        Defaults.path = os.getcwd()

    def map_path(self, path, map_name=map_name):
        map_location = os.path.join(path, map_name)
        return map_location

def raise_ioerror(err_num, filename):
    raise IOError('[Errno {0}]: {1}: {2}'.format(
        err_num, os.strerror(err_num), repr(filename)))

def dir_list(path):
    dir_list = set()
    for root, dirs, files in os.walk(path):
        dir_list.add(root)
        for d in dirs:
            dir_list.add(os.path.join(root, d))
    return dir_list

def create(path=Defaults().path, root=Defaults.root['truth'],
    map_location=Defaults.map_location, num=Defaults.num, 
    size=Defaults.size, hidden=Defaults.hidden, fill=Defaults.fill):
    if os.path.isfile(path):
        raise_ioerror(20, path)
    elif not os.path.exists(path):
        raise_ioerror(2, path)
    if not map_location:
        map_location = Defaults().map_path(path)
    if root:
        path = Defaults.root['location']
    if map_location in ('default', 'Default'):
        if root:
            map_location = Defaults.root['map']
        else:
            map_location = Defaults().map_path(path)
    elif os.path.isdir(map_location):
        raise_ioerror(21, map_location)
    if size == 'small':
        upper = 100
        lower = 1
        if not num:
            num = random.randint(50, 100)
    else:
        upper = 100000
        lower = 1000
        if not num:
            num = random.randint(150, 200)
    if hidden not in (True, False):
        hidden = True
    map_file = open(map_location, 'w')
    map_file.write(str(random.getrandbits(int(10000/3))))
    map_file.close()
    dirs, files = dir_list(path), set()
    complete = False
    while complete != True:
        try:
            for directory in dirs:
                for iteration in range(num):
                    if hidden and (sys.platform.startswith('linux') or 
                        sys.platform.startswith('darwin')):
                        filename = '.'
                    else:
                        filename = ''
                    filename += random.choice('qwertyuiopasdfghjklzxcvbnm') 
                    filename += random.choice('qwertyuiopasdfghjklzxcvbnm') 
                    filename += str(random.randint(1, 1000))
                    filename += random.choice(Defaults.ext)
                    filename = os.path.join(directory, filename)
                    dest = open(filename, 'w')
                    repetitions = random.randint(lower, upper)
                    for iteration in range(repetitions):
                        dest.write(Defaults.char)
                    files.add(filename)
                    dest.close()
                    if hidden and sys.platform.startswith('win'):
                       os.system('attrib +h {0}'.format(filename))
        except IOError:
            complete = True
        finally:
            if not fill:
                complete = True
    files.add(map_location)
    map_file = open(map_location, 'w+b')
    pickle.dump(files, map_file, -1)
    map_file.close()
    if sys.platform.startswith('win'):
        os.system('attrib +h {0}'.format(map_location))

def remove(map_location):
    map_file = open(map_location, 'r+b')
    files = pickle.load(map_file)
    for f in files:
        os.remove(f)

if __name__ == '__main__' and (sys.platform.startswith('linux') or 
    sys.platform == 'darwin'):
    parser = argparse.ArgumentParser(description='Spams junk files')
    parser.add_argument('-p', '--path', dest='path',
        default=Defaults().path, help='The parent directory to be spammed')
    parser.add_argument('-r', '--root', action='store_const', dest='root',
        const=Defaults.root['truth'], 
        help='Sets path to filesystem root directory')
    parser.add_argument('-m', '--map', dest='map_location', metavar='MAP',
        default=Defaults.map_location)
    parser.add_argument('-n', '--number', dest='num', type=int, 
        default=Defaults.num, 
        help='The number of files created per directory')
    parser.add_argument('-s', '--size', dest='size', 
        choices=['large', 'small'], default=Defaults.size, 
        help='The size of the files created')
    parser.add_argument('-H', '--hidden', action='store_const', 
        const=Defaults.hidden,
        help='Specifies whether files will be hidden or not')
    parser.add_argument('-f', '--fill', action='store_const', 
        const=Defaults.fill, 
        help='Specifies whether or not the filesystem will be filled')
    args = parser.parse_args()
    if not args.map_location:
        args.map_location = Defaults().map_path(args.path)
    create(path=args.path, root=args.root, map_location=args.map_location, 
        num=args.num, size=args.size, hidden=args.hidden, fill=args.fill) 
