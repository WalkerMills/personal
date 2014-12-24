package link

import (
    "simulator/node"
    "simulator/packet"
)

const (
    UP int = 0
    DOWN int = 1
)

type Link struct {
    id int
    capacity int
    size int
    delay int
    endpoints [2]*node.Receiver
    buffers [2][]*packet.Routable
}

type Transport struct {
    direction int
    handler *Link
}

// TODO: add link buffers flushing at rate Link.capacity, handling methods
