package main

import (
	"fmt"
	"math/rand"
	"os"
	"time"
	"univalle/sim/CD4TCellMonteCarloSimulation/model"

	chart "github.com/wcharczuk/go-chart"
)

const (
	lNodeSize = 100

	yearsInWeeks = 625

	pHIV = 0.005

	runs = 100
)

func main() {
	globalYearsResume := []model.NodeOverview{}

	for i := 0; i < 13; i++ {
		globalYearsResume = append(globalYearsResume, model.NodeOverview{NodeSize: lNodeSize})
	}

	globalFirstWeeksResume := []model.NodeOverview{}

	for i := 0; i < 16; i++ {
		globalFirstWeeksResume = append(globalFirstWeeksResume, model.NodeOverview{NodeSize: lNodeSize})
	}

	for i := 0; i < runs; i++ {
		fmt.Printf("RUN %d\n", i)
		rand.Seed(time.Now().UnixNano())

		lNode := model.NewInfectedLNode(rand.Float32, lNodeSize, pHIV)
		yearsResume := lNode.Run(yearsInWeeks, rand.Float32)

		for j := 0; j < len(yearsResume); j++ {
			globalYearsResume[j].A0 = (globalYearsResume[j].A0*float32(i) + yearsResume[j].A0) / float32(i+1)
			globalYearsResume[j].A1 = (globalYearsResume[j].A1*float32(i) + yearsResume[j].A1) / float32(i+1)
			globalYearsResume[j].A2 = (globalYearsResume[j].A2*float32(i) + yearsResume[j].A2) / float32(i+1)
			globalYearsResume[j].T = (globalYearsResume[j].T*float32(i) + yearsResume[j].T) / float32(i+1)
			globalYearsResume[j].D = (globalYearsResume[j].D*float32(i) + yearsResume[j].D) / float32(i+1)
			globalYearsResume[j].Empty = (globalYearsResume[j].Empty*float32(i) + yearsResume[j].Empty) / float32(i+1)
		}
	}

	xValues := []float64{}
	ValuesT := []float64{}
	ValuesA1PlusA2 := []float64{}
	ValuesA0 := []float64{}
	ValuesD := []float64{}
	ValuesEmpty := []float64{}

	for i := 0; i < len(globalYearsResume); i++ {
		xValues = append(xValues, float64(i))
		ValuesT = append(ValuesT, float64(globalYearsResume[i].T))
		ValuesA1PlusA2 = append(ValuesA1PlusA2, float64(globalYearsResume[i].A1)+float64(globalYearsResume[i].A2))
		ValuesA0 = append(ValuesA0, float64(globalYearsResume[i].A0))
		ValuesD = append(ValuesD, float64(globalYearsResume[i].D))
		ValuesEmpty = append(ValuesEmpty, float64(globalYearsResume[i].Empty))
	}

	graph := chart.Chart{
		XAxis: chart.XAxis{
			Name: "Años",
		},
		YAxis: chart.YAxis{
			Name: "Conteo de células",
		},
		Series: []chart.Series{
			chart.AnnotationSeries{
				Annotations: []chart.Value2{
					{XValue: 12.0, YValue: ValuesT[12], Label: fmt.Sprintf("T %f", globalYearsResume[12].T)},
					{XValue: 12.0, YValue: ValuesA1PlusA2[12], Label: fmt.Sprintf("A1 + A2 %f", globalYearsResume[12].A2+globalYearsResume[12].A1)},
					{XValue: 12.0, YValue: ValuesA0[12], Label: fmt.Sprintf("A0 %f", globalYearsResume[12].A0)},
					{XValue: 12.0, YValue: ValuesD[12], Label: fmt.Sprintf("D %f", globalYearsResume[12].D)},
					{XValue: 12.0, YValue: ValuesEmpty[12], Label: fmt.Sprintf("Espacios vacíos %f", globalYearsResume[12].Empty)},
				},
			},
			chart.ContinuousSeries{
				XValues: xValues,
				YValues: ValuesT,
			},
			chart.ContinuousSeries{
				XValues: xValues,
				YValues: ValuesA1PlusA2,
			},
			chart.ContinuousSeries{
				XValues: xValues,
				YValues: ValuesA0,
			},
			chart.ContinuousSeries{
				XValues: xValues,
				YValues: ValuesD,
			},
			chart.ContinuousSeries{
				XValues: xValues,
				YValues: ValuesEmpty,
			},
		},
	}

	f, _ := os.Create("Years.png")
	defer f.Close()
	graph.Render(chart.PNG, f)
}
