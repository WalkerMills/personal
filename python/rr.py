#! /usr/bin/python

# Recusively removes either files, directories, or both

import os
import argparse
import shutil

def recurse(dir_name):
    dir_list = set()
    file_list = set()
    for root, dirs, files in os.walk(dir_name):
        for d in dirs:
            if d != '':
                dir_list.add(os.path.join(root, d))
        for f in files:
            file_list.add(os.path.join(root, f))
    return dir_list, file_list
parser = argparse.ArgumentParser(
    description='Recursively remove files and/or directories')
type_group = parser.add_mutually_exclusive_group()
search_group = parser.add_mutually_exclusive_group()
action_group = parser.add_mutually_exclusive_group()
parser.add_argument('parent', type=str, metavar='PARENT',
    help='The parent directory to be recused into')
search_group.add_argument('-s', '--search', dest='search', type=str,
    default=None, help='A search term for the files/directories')
search_group.add_argument('-R', '--reverse', dest='rev', type=str,
    metavar='SEARCH', default=None,
    help='A reverse search term for the files/directories')
action_group.add_argument('-l', '--list', action='store_true', dest='lists',
    default=False, help='Lists files/directories instead of deleting them')
action_group.add_argument('-m', '--move', type=str, dest='move', default=False,
    metavar='DEST', help='Moves files/directories instead of deleting them')
action_group.add_argument('-c', '--copy', type=str, dest='copy', default=False,
    metavar='DEST', help='Copies files/directories instead of deleting them')
type_group.add_argument('-p', '--parent', dest='root', action='store_true',
    default=False, help='Specifies the parent directory only')
type_group.add_argument('-f', '--files', dest='files', action='store_true',
    default=False, help='Specifies files only')
type_group.add_argument('-d', '--dirs', '--directories', dest='dirs', 
    default=False, action='store_true', help='Specifies directories only')
type_group.add_argument('-a', '--all', dest='both', action='store_true', 
    default=True, help='Specifies both directories and files (default)')
args = parser.parse_args()
if args.root and not args.lists:
    if args.move:
        name = os.path.split(args.parent)[1]
        shutil.move(args.parent, os.path.join(args.move, name))
    else:
        shutil.rmtree(args.parent)
else:
    dirs, files = recurse(args.parent)
    if not args.files and not args.dirs:
        args.files, args.dirs = True, True
    if args.search:
        for d in dirs.copy():
            if args.search not in d:
                dirs.discard(d)
        for f in files.copy():
            if args.search not in f:
                files.discard(f)
    elif args.rev:
        for d in dirs.copy():
            if args.rev in d:
                dirs.discard(d)
        for f in files.copy():
            if args.rev in f:
                files.discard(f)
    if args.dirs:
        if args.lists and len(dirs) > 0:
            print('\n'.join(dirs))
        elif not args.lists:
            for d in dirs:
                if args.move:
                    name = d[len(args.parent) + 1:]
                    shutil.move(d, os.path.join(args.move, name))
                elif args.copy:
                    name = d[len(args.parent) + 1:]
                    shutil.copytree(d, os.path.join(args.copy, name))
                else:
                    shutil.rmtree(d)
    if args.files:
        if args.lists and len(files) > 0:
            print('\n'.join(files))
        elif not args.lists:
            for f in files:
                if os.path.isfile(f):
                    if args.move:
                        shutil.move(f, args.move)
                    elif args.copy:
                        shutil.copy2(f, args.copy)
                    else:
                        os.remove(f)
