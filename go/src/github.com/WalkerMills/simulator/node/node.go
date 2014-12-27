package node

import (
    "simulator/links"
    "simulator/packet"
)

type Receiver interface {
    connect(t *link.Transport)
    disconnect(t *link.Transport)
    receive(p *packet.Routable)
}
