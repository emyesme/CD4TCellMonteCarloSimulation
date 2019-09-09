package model

const (
	pInf  = 0.999
	pRem  = 0.0 // or 0.9
	pAct  = 0.0025
	pRepl = 0.99
	r     = 4
	t1    = 4
	t2    = 30
)

// for T cells
func rule1(randomGen func() float32, lNode *LNode, i, j int) State {
	if lNode.NeighborsInState(i, j, StateA1) > 0 || lNode.NeighborsInState(i, j, StateA2) >= r {
		if randomGen() > pInf {
			return StateA0
		}

		return StateA1
	}

	return StateT
}

// for A1 cells
func rule2(randomGen func() float32, time, lasUpdate int) State {
	if (time - lasUpdate) > t1 {
		if randomGen() <= pRem {
			return StateEmpty
		}
		return StateA2
	}

	return StateA1
}

// for A2 cells
func rule3() State {
	return StateD
}

// for D cells
func rule4(randomGen func() float32) State {
	if randomGen() <= pRepl {
		return StateT
	}
	return StateD
}

// for A0 cells
func rule5(randomGen func() float32, time, lasUpdate int) State {
	if time-lasUpdate > t2 {
		if randomGen() <= pAct {
			return StateA1
		}
	}

	return StateA0
}

// for empty state cells
func rule6() State {
	return StateEmpty
}
