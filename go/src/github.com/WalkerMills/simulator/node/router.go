package node

import (
    "simulator/link"
    // "simulator/packet"
)


type Router struct {
    id int
    unused int
    ports map[int]*link.Transport
    hosts map[int]int
    routers map[int]int
    table map[int]int
    tmp_table map[int]int
    finished map[int]bool
}

// TODO: Router constructor, implement Receiver interface on Router
