package flow

import (
    "simulator/node"
)

type Flow struct {
    id int
    dest int
    data int
    localhost *node.Host
}

// TODO: flow constructor, TCP structs/constructors
