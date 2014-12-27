package node

import (
    "fmt"
    "simulator/flow"
    "simulator/link"
    "simulator/packet"
)

type expectedMap map[[2]int][]int

func newExpectedMap() *expectedMap {
    return &make(map[[2]int][]int).(expectedMap)
} 

type Host struct {
    id int
    flows []*flow.Flow
    expected *expectedMap
    port *link.Transport
    buffer []*packet.Routable
}

// TODO: Host constructor

func (h *Host) connect(t *link.Transport) {
    if h.port != nil {
        panic(fmt.Sprintf("host %d is already connected", h.id))
    }
    // Store the transport handler
    h.port := t
}

func (h *Host) disconnect(t *link.Transport) {
    if ! h.port.eq(t) {
        panic(fmt.Sprintf("host %d is not connected to link %d, %d",
                          h.id, t.link.id, t.target))
    }
    // Reset the transport handler
    h.port := nil
}

func (h *Host) receive(p *packet.Routable) {
    // TODO: process packets
}
