package link

import (
    "simulator/node"
)

type Link struct {
    id int
    capacity int
    size int
    delay int
    endpoints [2]*node.Receiver
}

type Transport struct {
    direction int
    handler *Link
}

// TODO: add link buffers flushing at rate Link.capacity, handling methods
