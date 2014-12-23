package node

import (
    "simulator/packet"
)

type Receiver interface {
    receive(p *packet.Routable)
}
