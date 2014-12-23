package node

import (
    "simulator/flow"
    "simulator/link"
    "simulator/packet"
)

type Receiver interface {
    receive(p *packet.Routable)
}

type expectedMap map[[2]int][]int

func newExpectedMap() *expectedMap {
    return &make(map[[2]int][]int).(expectedMap)
} 

type Host struct {
    id int
    flows []*flow.Flow
    port *link.Transport
    expected *expectedMap
}

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

// TODO: Host & Router constructors, both satisfy Receiver
