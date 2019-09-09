#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <random>

#define RUNS 625
#define MATRIXSIZE 100
#define PHIV 0.005
#define R 4
#define PINF 0.999
#define T1 4
#define PREM 0.9
#define PREPL 0.99
#define PACT 0.0025
#define T2 30

using namespace std;

struct Cell
{
    string state;
    int lastModified;
};

int neighborsCell(Cell *lnode, int position, string state);
void applyRules(Cell *node, int t);
void rule1(Cell *lnode, int position, int t);
void rule2(Cell *lnode, int position, int t);
void rule3(Cell *lnode, int position, int t);
void rule4(Cell *lnode, int position, int t);
void rule5(Cell *lnode, int position, int t);
void rule6(Cell *lnode, int position, int t);
void finalReport(Cell *node, int t);

double randomGenerator()
{
    // :(
    //min + (double)rand() * (max - min) / (double)RAND_MAX;
    return (double)rand() / (double)RAND_MAX;
}

int main()
{
    //create matrix
    Cell *lnode = new Cell[MATRIXSIZE * MATRIXSIZE];
    //start random for matrix state
    srand(190398);
    //random to initialize the matrix
    for (int i = 0; i < MATRIXSIZE * MATRIXSIZE; i++)
    {
        if (randomGenerator() <= PHIV)
        {
            lnode[i] = {"A1", 0};
        }
        else
        {
            lnode[i] = {"T", 0};
        }
    }
    //initial proportion of the matrix
    int a1 = 0;
    int T = 0;
    for (int i = 0; i < MATRIXSIZE * MATRIXSIZE; i++)
    {
        if (lnode[i].state.compare("A1") == 0)
        {
            a1++;
        }
        else
        {
            T++;
        }
    }
    cout << "initial state" << endl;
    cout << "A1: " << a1 << endl;
    cout << "T: " << T << endl;

    //apply rules
    for (int i = 0; i < RUNS; i++)
    {
        //cout << "step: " << i << endl;
        applyRules(lnode, i);
    }
    finalReport(lnode,RUNS);
    //final report

    delete[] lnode;

    return 0;
}

/*position = row * size + col*/
int neighborsCell(Cell * lnode, int position, string state)
{

    int row = floor(position / MATRIXSIZE);
    int col = position % MATRIXSIZE;
    int count = 0;
    vector<string> statesNeighbors;

    if (row > 0)
    {
        //arriba
        statesNeighbors.push_back(lnode[(row - 1) * MATRIXSIZE + col].state);
    }
    if (row > 0 && col + 1 < MATRIXSIZE)
    {
        //esquina superor derecha
        statesNeighbors.push_back(lnode[(row - 1) * MATRIXSIZE + (col + 1)].state);
    }
    if (col > 0)
    {
        //izquierda
        statesNeighbors.push_back(lnode[row * MATRIXSIZE + (col - 1)].state);
    }
    if (col + 1 < MATRIXSIZE)
    {
        //derecha
        statesNeighbors.push_back(lnode[row * MATRIXSIZE + (col + 1)].state);
    }
    if (row + 1 < MATRIXSIZE && col > 0)
    {
        //esquina inferior izquierda
        statesNeighbors.push_back(lnode[(row + 1) * MATRIXSIZE + (col - 1)].state);
    }
    if (row + 1 < MATRIXSIZE)
    {
        //abajo
        statesNeighbors.push_back(lnode[(row + 1) * MATRIXSIZE + col].state);
    }
    if (row > 0 && col > 0)
    {
        //esquina superior izquierda
        statesNeighbors.push_back(lnode[(row - 1) * MATRIXSIZE + (col - 1)].state);
    }
    if (row + 1 < MATRIXSIZE && col + 1 < MATRIXSIZE)
    {
        //esquina inferior derecha
        statesNeighbors.push_back(lnode[(row + 1) * MATRIXSIZE + (col + 1)].state);
    }
    //cout << statesNeighbors[0] << endl;
    for (const string nstate : statesNeighbors)
    {
        if (state.compare(nstate) == 0)
        {
            count++;
        }
    }
    statesNeighbors.clear();

    return count;
}

void applyRules(Cell *lnode, int t)
{
    //apply rules
    for (int i = 0; i < MATRIXSIZE*MATRIXSIZE; i++)
    {
        if (lnode[i].state == "T")
        {
            rule1(lnode, i, t);
        }
        else if (lnode[i].state == "A1")
        {
            rule2(lnode, i, t);
        }
        else if (lnode[i].state == "A2")
        {
            rule3(lnode, i, t);
        }
        else if (lnode[i].state == "D")
        {
            rule4(lnode, i, t);
        }
        else if (lnode[i].state == "A0")
        {
            rule5(lnode, i, t);
        }
        else if (lnode[i].state == "0")
        {
            rule6(lnode, i, t);
        }
    }
}

/*we suppose the given cell is in T state*/
void rule1(Cell *lnode, int position, int t)
{
    int neighborsA1 = neighborsCell(lnode, position, "A1");
    int neighborsA2 = neighborsCell(lnode, position, "A2");
    int row = floor(position / MATRIXSIZE);
    int col = position % MATRIXSIZE;

    if ((neighborsA1 >= 1) or (neighborsA2 >= R))
    {
        //cout << "hey girl" << endl;
        double probability = randomGenerator();
        //cout << probability << endl;
        if (probability <= PINF)
        {
            //cout << "menor a pinf" << endl;
            lnode[position].state = "A1";
            lnode[position].lastModified = t;
        }
        else
        {
            //cout << "mayor a pinf" << endl;
            lnode[position].state = "A0";
            lnode[position].lastModified = t;
        }
    }
}

/*we suppose the given cells is in A1 state*/
void rule2(Cell *lnode, int position, int t)
{
    //cout << "resta: " << t - lnode[position].lastModified << endl;
    if (t - lnode[position].lastModified >= T1)
    {
        //cout << "4" << endl;
        lnode[position].state = "A2";
        lnode[position].lastModified = t;
    }
    else
    {
        double probability = randomGenerator();
        if (probability <= PREM)
        {
            //cout << "0" << endl;
            lnode[position].state = "0";
            lnode[position].lastModified = t;
        }
    }
}

/*we suppose the given cells is in A2 state*/
void rule3(Cell *lnode, int position, int t)
{
    lnode[position].state = "D";
    lnode[position].lastModified = t;
}

/*we suppose the given cells is in D state*/
void rule4(Cell *lnode, int position, int t)
{
    double probability = randomGenerator();
    if (probability <= PREPL)
    {
        lnode[position].state = "T";
        lnode[position].lastModified = t;
    }
}

/*we suppose the given cells is in A0 state*/
void rule5(Cell *lnode, int position, int t)
{
    //cada t2 unidades de tiempo entra
    if (t - lnode[position].lastModified == T2){
        double probability = randomGenerator();
        if (probability <= PACT)
        {
            lnode[position].state = "A1";
            lnode[position].lastModified = t;
        }
    }
    
}

/*we suppose the given cells is in 0 state*/
void rule6(Cell *lnode, int position, int t)
{
    lnode[position].state = "T";
    lnode[position].lastModified = t;
}

void finalReport(Cell *lnode, int t)
{
    int a1, T, d, a2, a0, nothing;
    a1 = T = d = a2 = a0 = nothing = 0;
    for (int i = 0; i < MATRIXSIZE * MATRIXSIZE; i++)
    {
        if (lnode[i].state == "T")
        {
            T++;
        }
        else if (lnode[i].state == "A1")
        {
            a1++;
        }
        else if (lnode[i].state == "A2")
        {
            a2++;
        }
        else if (lnode[i].state == "A0")
        {
            a0++;
        }
        else if (lnode[i].state == "D")
        {
            d++;
        }
        else if (lnode[i].state == "0")
        {
            nothing++;
        }
    }

    cout << "report" << endl;
    cout << "A1: " << a1 << endl;
    cout << "A2: " << a2 << endl;
    cout << "A0: " << a0 << endl;
    cout << "T: " << T << endl;
    cout << "0: " << nothing << endl;
    cout << "D: " << d << endl;
    cout << "tiempo: " << t << endl;
}