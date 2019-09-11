package model

import (
	"fmt"
	"math"
)

// State ...
type State string

const (
	//StateT ...
	StateT = "T"
	//StateA0 ...
	StateA0 = "A0"
	//StateA1 ...
	StateA1 = "A1"
	//StateA2 ...
	StateA2 = "A2"
	//StateD ...
	StateD = "D"
	// StateEmpty ..
	StateEmpty = ""
)

// Cell ...
type Cell struct {
	State      State
	LastUpdate int
}

//LNode ...
type LNode struct {
	Size  int
	Cells *[]Cell
}

// NewInfectedLNode ...
func NewInfectedLNode(randomGen func() float32, lNodeSize int, pHIV float32) LNode {
	cells := []Cell{}

	for i := 0; i < lNodeSize*lNodeSize; i++ {
		// prob := randomGen()
		if i == 0 /* prob <= pHIV */ {
			cells = append(cells, Cell{State: StateA1, LastUpdate: 0})
			continue
		}
		cells = append(cells, Cell{State: StateT, LastUpdate: 0})
	}

	return LNode{lNodeSize, &cells}
}

// NeighborsInState ...
func (n *LNode) NeighborsInState(i, j int, state State) int {
	statesNeighbors := []State{}

	if i > 0 {
		//arriba
		statesNeighbors = append(statesNeighbors, (*n.Cells)[(i-1)*n.Size+j].State)
	}
	if i > 0 && j+1 < n.Size {
		//esquina superor derecha
		statesNeighbors = append(statesNeighbors, (*n.Cells)[(i-1)*n.Size+(j+1)].State)
	}
	if j > 0 {
		//izquierda
		statesNeighbors = append(statesNeighbors, (*n.Cells)[i*n.Size+(j-1)].State)
	}
	if j+1 < n.Size {
		//derecha
		statesNeighbors = append(statesNeighbors, (*n.Cells)[i*n.Size+(j+1)].State)
	}
	if i+1 < n.Size && j > 0 {
		//esquina inferior izquierda
		statesNeighbors = append(statesNeighbors, (*n.Cells)[(i+1)*n.Size+(j-1)].State)
	}
	if i+1 < n.Size {
		//abajo
		statesNeighbors = append(statesNeighbors, (*n.Cells)[(i+1)*n.Size+j].State)
	}
	if i > 0 && j > 0 {
		//esquina superior izquierda
		statesNeighbors = append(statesNeighbors, (*n.Cells)[(i-1)*n.Size+(j-1)].State)
	}
	if i+1 < n.Size && j+1 < n.Size {
		//esquina inferior derecha
		statesNeighbors = append(statesNeighbors, (*n.Cells)[(i+1)*n.Size+(j+1)].State)
	}

	count := 0

	for _, neighbourState := range statesNeighbors {
		if neighbourState == state {
			count++
		}
	}

	return count
}

func (n *LNode) getCell(i, j int) Cell {
	return (*n.Cells)[i*n.Size+j]
}

func (n *LNode) setCell(i, j, time int, state State) {
	(*n.Cells)[i*n.Size+j] = Cell{state, time}
}

func (n *LNode) getCellsCopy() []Cell {
	newCells := []Cell{}

	for i := 0; i < len(*n.Cells); i++ {
		newCells = append(newCells, (*n.Cells)[i])
	}

	return newCells
}

// Run ...
func (n *LNode) Run(steps int, randomGen func() float32) []NodeOverview {
	yearsResume := []NodeOverview{}

	yearsResume = append(yearsResume, n.GetNodeOverview())

	for time := 1; time <= steps; time++ {
		cellsCopy := n.getCellsCopy()
		for i := 0; i < n.Size; i++ {
			for j := 0; j < n.Size; j++ {
				// TODO: fix the lastUpdate
				if n.getCell(i, j).State == StateT {
					cellsCopy[i*n.Size+j].State = rule1(randomGen, n, i, j)
					if cellsCopy[i*n.Size+j].State != StateT {
						cellsCopy[i*n.Size+j].LastUpdate = time
					}
					continue
				}

				if n.getCell(i, j).State == StateA1 {
					cellsCopy[i*n.Size+j].State = rule2(randomGen, time, n.getCell(i, j).LastUpdate)
					if cellsCopy[i*n.Size+j].State != StateA1 {
						cellsCopy[i*n.Size+j].LastUpdate = time
					}
					continue
				}

				if n.getCell(i, j).State == StateA2 {
					cellsCopy[i*n.Size+j].State = rule3()
					cellsCopy[i*n.Size+j].LastUpdate = time
					continue
				}

				if n.getCell(i, j).State == StateD {
					cellsCopy[i*n.Size+j].State = rule4(randomGen)
					if cellsCopy[i*n.Size+j].State != StateD {
						cellsCopy[i*n.Size+j].LastUpdate = time
					}
					continue
				}

				if n.getCell(i, j).State == StateA0 {
					cellsCopy[i*n.Size+j].State = rule5(randomGen, time, n.getCell(i, j).LastUpdate)
					if cellsCopy[i*n.Size+j].State != StateA0 {
						cellsCopy[i*n.Size+j].LastUpdate = time
					}
					continue
				}

				if n.getCell(i, j).State == StateEmpty {
					cellsCopy[i*n.Size+j].State = rule6()
					cellsCopy[i*n.Size+j].LastUpdate = time
					continue
				}
			}

		}

		n.Cells = &cellsCopy

		if math.Mod(float64(time), 52.0) == 0 {
			yearsResume = append(yearsResume, n.GetNodeOverview())
		}
	}

	return yearsResume
}

// NodeOverview ...
type NodeOverview struct {
	A0       float32
	A1       float32
	A2       float32
	T        float32
	D        float32
	Empty    float32
	NodeSize float32
}

// Print ...
func (o NodeOverview) Print() {
	fmt.Println("NODE OVERVIEW")
	fmt.Printf("State A0: %f - %f\n", o.A0, (o.A0/(o.NodeSize*o.NodeSize))*100)
	fmt.Printf("State A1: %f - %f\n", o.A1, (o.A1/(o.NodeSize*o.NodeSize))*100)
	fmt.Printf("State A2: %f - %f\n", o.A2, (o.A2/(o.NodeSize*o.NodeSize))*100)
	fmt.Printf("State T: %f - %f\n", o.T, (o.T/(o.NodeSize*o.NodeSize))*100)
	fmt.Printf("State D: %f - %f\n", o.D, (o.D/(o.NodeSize*o.NodeSize))*100)
	fmt.Printf("State empty: %f - %f\n", o.Empty, (o.Empty/(o.NodeSize*o.NodeSize))*100)
}

// GetNodeOverview ...
func (n *LNode) GetNodeOverview() NodeOverview {
	overview := NodeOverview{NodeSize: float32(n.Size)}
	for i := 0; i < n.Size; i++ {
		for j := 0; j < n.Size; j++ {
			switch n.getCell(i, j).State {
			case StateT:
				overview.T += 1.0

			case StateA1:
				overview.A1 += 1.0

			case StateA2:
				overview.A2 += 1.0

			case StateA0:
				overview.A0 += 1.0

			case StateD:
				overview.D += 1.0

			case StateEmpty:
				overview.Empty += 1.0
			}
		}
	}

	return overview
}
