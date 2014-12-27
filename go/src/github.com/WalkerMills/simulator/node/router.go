package node

import (
    "fmt"
    "simulator/link"
    "simulator/packet"
)


type Router struct {
    id int
    unused int
    ports []*link.Transport
    buffer []*packet.Routable
    hosts map[int]int
    routers map[int]int
    table map[int]int
    tmp_table map[int]int
    finished map[int]bool
}

// TODO: Router constructor

func (r *Router) connect(t *link.Transport) {
    i := 0
    // Find the next unused port
    for ; i < r.ports.len && r.ports[i] != nil; i++ {}
    // If no open port was found, panic
    if i == r.ports.len {
        panic(fmt.Sprintf("router %d has no more open ports", r.id))
    }
    // Update lowest unused port index
    r.ports.unused = i
    // Connect the transport handler
    r.ports[r.unused] = t    
}

func (r *Router) disconnect(t *link.Transport) {
    // Find the port's index
    i := 0
    for ; i < r.ports.len && ! t.eq(r.ports[i]); i++ {}
    // If no index was found, panic
    if i == r.ports.len {
        panic(fmt.Sprintf("router %d is not connected to link %d, %d",
                          r.id, t.link.id, t.target))
    }
    // Nil the port
    r.ports[i] = nil
    // Update lowest unused port, if necessary
    if i < r.unused {
        r.unused = i
    }
}

func (r *Router) receive(p *packet.Routable) {
    // TODO: process packets
}
