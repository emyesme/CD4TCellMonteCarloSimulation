package main

import (
	"fmt"
	"math/rand"
	"time"
	"univalle/sim/project/model"
)

const (
	lNodeSize = 100

	yearsInWeeks = 625

	pHIV = 0.005

	runs = 10000
)

func main() {
	globalOverview := model.NodeOverview{
		Paramenters: map[model.State]float32{},
		NodeSize:    lNodeSize,
	}

	for i := 0; i < runs; i++ {
		rand.Seed(time.Now().UnixNano())

		lNode := model.NewInfectedLNode(rand.Float32, lNodeSize, pHIV)
		// lNode.GetNodeOverview().Print()
		fmt.Println()

		lNode.Run(yearsInWeeks, rand.Float32)
		localOverview := lNode.GetNodeOverview()

		globalOverview.Paramenters[model.StateA0] = (globalOverview.Paramenters[model.StateA0]*(float32)(i) + localOverview.Paramenters[model.StateA0]) / (float32)(i+1)
		globalOverview.Paramenters[model.StateA1] = (globalOverview.Paramenters[model.StateA1]*(float32)(i) + localOverview.Paramenters[model.StateA1]) / (float32)(i+1)
		globalOverview.Paramenters[model.StateA2] = (globalOverview.Paramenters[model.StateA2]*(float32)(i) + localOverview.Paramenters[model.StateA2]) / (float32)(i+1)
		globalOverview.Paramenters[model.StateT] = (globalOverview.Paramenters[model.StateT]*(float32)(i) + localOverview.Paramenters[model.StateT]) / (float32)(i+1)
		globalOverview.Paramenters[model.StateD] = (globalOverview.Paramenters[model.StateD]*(float32)(i) + localOverview.Paramenters[model.StateD]) / (float32)(i+1)
		globalOverview.Paramenters[model.StateEmpty] = (globalOverview.Paramenters[model.StateEmpty]*(float32)(i) + localOverview.Paramenters[model.StateEmpty]) / (float32)(i+1)

		globalOverview.Print()
		fmt.Println()
	}
}
