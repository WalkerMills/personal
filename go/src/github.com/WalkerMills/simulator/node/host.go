package node

import (
    "simulator/flow"
    "simulator/link"
    // "simulator/packet"
)

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

// TODO: Host constructor, implement Receiver interface on Host
