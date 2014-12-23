package packet


type Routable interface {
    destination() int
}


type Informational interface {
    sender() int
}


type Packet struct {
    src int
    dest int
    flow int
    id int
}

func (p *Packet) destination() int {
    return p.dest
}

func (p *Packet) acknowledge(next int) ack *Acknowledge {
    ack.dest = p.src
    ack.flow = p.flow
    ack.id = p.id
    ack.expected = next
}


type Acknowledge struct {
    dest int
    flow int
    id int
    expected int
}

func (p *Acknowledge) destination() int {
    return p.dest
}


type Routing struct {
    src int
    dest int
    host int
    cost int
    path []int
}

func (p *Routing) destination() int {
    return p.dest
}

func (p *Routing) sender() int {
    return p.src
}


type Finish struct {
    src int
    dest int
}

func (p *Finish) destination() int {
    return p.dest
}

func (p *Finish) sender() int {
    return p.src
}
