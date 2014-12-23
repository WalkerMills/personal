#!/usr/bin/env python2

import sys
from mpd import MPDClient
from socket import error as SocketError

HOST = 'localhost'
PORT = '6600'
CONNECTION = {'host': HOST, 'port': PORT}

def main():
    client = MPDClient()

    # Connect client
    try:
        client.connect(**CONNECTION)
    except SocketError:
        sys.exit(1)

    current = client.currentsong()
    status = client.status()
    client.disconnect()

    artist = current["artist"]
    song = current["title"]

    if artist != "":
        sep = " - "
    else:
        sep = ""

    random = int(status["random"])
    state = status["state"]

    if random or state == "pause":
        info = " ["

        if state == "pause":
            info += "p"        
        if random:
            info += "z"

        info += "]"
    else:
        info = ""

    sys.stdout.write("{}{}{}{}".format(artist, sep, song, info))
    sys.stdout.flush()
    sys.exit(0)

if __name__ == "__main__":
    main()

