package link

import (
    "simulator/node"
    "simulator/packet"
)


type Link struct {
    id int
    capacity int
    size int
    delay int
    connected [2]*node.Receiver
    buffers [2][]*packet.Routable
}

// TODO: Link constructor

func (l *Link) handler(direction int) *Transport {
    return &Transport{target: 1 - direction, link: l}
}

func (l *Link) connect(A *node.Receiver, B *node.Receiver) {
    // Connect the endpoints
    l.connected[direction] := [2]*node.Receiver{A, B}
    // Give the endpoints transport handlers
    for direction, endpoint := range l.connected {
        go l.connected[direction].connect(l.handler(direction))
    }
}

func (l *Link) disconnect() {
    // Tell the receivers to disconnect
    for i, endpoint := range l.connected {
        go endpoint.disconnect(l.handler(i))
    }
    // Disconnect the endpoints
    l.connected := [2]*node.Receiver{nil, nil}
}

func (l *Link) enqueue(dest int, p *packet.Routable) {
    // TODO: rate limit packets (CoDel)
}


type Transport {
    target int
    link *Link
}

func (t *Transport) eq(other *Transport) bool {
    return t.link == other.link && t.target == other.target
}

func (t *Transport) send(p *packet.Routable) {
    t.link.enqueue(t.target, p)
}
