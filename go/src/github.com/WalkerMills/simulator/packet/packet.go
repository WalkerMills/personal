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

type Acknowledge struct {
    dest int
    flow int
    id int
    expected int
}

type Routing struct {
    src int
    dest int
    host int
    cost int
    path []int
}

type Finish struct {
    src int
    dest int
}

// TODO: all packet pointers satisfy Routable
// TODO: Routing & Finish pointers satisfy Informational
