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
#define PREM 0.0
#define PREPL 0.99
#define PACT 0.0025
#define T2 30

using namespace std;

double a1Global  = 0;
double a2Global = 0;
double a0Global = 0;
double TGlobal = 0;
double nothingGlobal = 0;
double dGlobal = 0;
double tGlobal = 0;

struct Cell
{
    string state;
    int lastModified;
};

int neighborsCell(Cell *lnode, int position, string state);
void applyRules(Cell *nodeNew, Cell *nodeOld, int t);
void rule1(Cell *lnodeOld, Cell *lnodeNew, int position, int t);
void rule2(Cell *lnodeOld, Cell *lnodeNew, int position, int t);
void rule3(Cell *lnodeNew, int position, int t);
void rule4(Cell *lnodeNew, int position, int t);
void rule5(Cell *lnodeOld, Cell *lnodeNew, int position, int t);
void rule6(Cell *lnodeNew, int position, int t);
void finalReport(Cell *node, int t, int i);

double randomGenerator()
{
    // :(
    //min + (double)rand() * (max - min) / (double)RAND_MAX;
    double var = (double)rand() / (double)RAND_MAX;
    return var;
}

void copyArray(Cell * a, Cell * b)
{
    for (int i = 0; i < MATRIXSIZE*MATRIXSIZE; i++)
    {
        b[i] = a [i];
    }
}

int main()
{
    //create matrix
    Cell *lnodeA = new Cell[MATRIXSIZE * MATRIXSIZE];
    //start random for matrix state
    //srand(time(NULL));
    //random to initialize the matrix
    /* for (int i = 0; i < MATRIXSIZE * MATRIXSIZE; i++)
    {
        if (i == 2)
        {
            lnodeA[i] = {"A1", 0};
        }
        else
        {
            lnodeA[i] = {"T", 0};
        }
    } */

    //initial proportion of the matrix
    int a1 = 0;
    int T = 0;
    /* for (int i = 0; i < MATRIXSIZE * MATRIXSIZE; i++)
    {
        if (lnodeA[i].state.compare("A1") == 0)
        {
            a1++;
        }
        else
        {
            T++;
        }
    } */
/*     cout << "initial state" << endl;
    cout << "A1: " << a1 << endl;
    cout << "T: " << T << endl; */

    Cell * lnodeB = new Cell[MATRIXSIZE * MATRIXSIZE];

    copyArray(lnodeA, lnodeB);

   for(int j = 0; j < 10000; j++)
   {
        srand(time(NULL));
        for (int i = 1; i <= RUNS; i++)
        {
            for (int i = 0; i < MATRIXSIZE * MATRIXSIZE; i++)
            {
                if (randomGenerator() <= PHIV)
                {
                    lnodeA[i] = {"A1", 0};
                }
                else
                {
                    lnodeA[i] = {"T", 0};
                }
            }

            applyRules(lnodeA, lnodeB, i);
            copyArray(lnodeB, lnodeA);
            // cout << lnodeB[0].state << " " << lnodeB[1].state << " " << lnodeB[2].state << " " << endl;
            // cout << lnodeB[3].state << " " << lnodeB[4].state << " " << lnodeB[5].state << " " << endl;
            // cout << lnodeB[6].state << " " << lnodeB[7].state << " " << lnodeB[8].state << " " << endl;
            // cout << endl;
        }
        finalReport(lnodeB, RUNS, j);
    }

    delete[] lnodeA;
    delete[] lnodeB;

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

void applyRules(Cell *lnodeOld, Cell *lnodeNew, int t)
{
    //apply rules
    for (int i = 0; i < MATRIXSIZE*MATRIXSIZE; i++)
    {
        if (lnodeOld[i].state == "T")
        {
            rule1(lnodeOld, lnodeNew, i, t);
        }
        else if (lnodeOld[i].state == "A1")
        {
            rule2(lnodeOld, lnodeNew, i, t);
        }
        else if (lnodeOld[i].state == "A2")
        {
            rule3(lnodeNew, i, t);
        }
        else if (lnodeOld[i].state == "D")
        {
            rule4(lnodeNew, i, t);
        }
        else if (lnodeOld[i].state == "A0")
        {
            rule5(lnodeOld, lnodeNew, i, t);
        }
        else if (lnodeOld[i].state == "0")
        {
            rule6(lnodeNew, i, t);
        }
    }
}

/*we suppose the given cell is in T state*/
void rule1(Cell *lnodeOld, Cell *lnodeNew, int position, int t)
{
    int neighborsA1 = neighborsCell(lnodeOld, position, "A1");
    int neighborsA2 = neighborsCell(lnodeOld, position, "A2");
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
            lnodeNew[position].state = "A1";
            lnodeNew[position].lastModified = t;
        }
        else
        {
            //cout << "mayor a pinf" << endl;
            lnodeNew[position].state = "A0";
            lnodeNew[position].lastModified = t;
        }
    }
}

/*we suppose the given cells is in A1 state*/
void rule2(Cell *lnodeOld, Cell *lnodeNew, int position, int t)
{
    //cout << "resta: " << t - lnode[position].lastModified << endl;
    if (t - lnodeOld[position].lastModified >= T1)
    {
        //cout << "4" << endl;
        lnodeNew[position].state = "A2";
        lnodeNew[position].lastModified = t;
    }
    else
    {
        double probability = randomGenerator();
        if (probability <= PREM)
        {
            //cout << "0" << endl;
            lnodeNew[position].state = "0";
            lnodeNew[position].lastModified = t;
        }
    }
}

/*we suppose the given cells is in A2 state*/
void rule3(Cell *lnodeNew, int position, int t)
{
    lnodeNew[position].state = "D";
    lnodeNew[position].lastModified = t;
}

/*we suppose the given cells is in D state*/
void rule4(Cell *lnodeNew, int position, int t)
{
    double probability = randomGenerator();
    if (probability <= PREPL)
    {
        lnodeNew[position].state = "T";
        lnodeNew[position].lastModified = t;
    }
}

/*we suppose the given cells is in A0 state*/
void rule5(Cell *lnodeOld, Cell *lnodeNew, int position, int t)
{
    //cada t2 unidades de tiempo entra
    if (t - lnodeOld[position].lastModified == T2){
        double probability = randomGenerator();
        if (probability <= PACT)
        {
            lnodeNew[position].state = "A1";
            lnodeNew[position].lastModified = t;
        }
    }
    
}

/*we suppose the given cells is in 0 state*/
void rule6(Cell *lnodeNew, int position, int t)
{
    lnodeNew[position].state = "T";
    lnodeNew[position].lastModified = t;
}

void finalReport(Cell *lnode, int t, int i)
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

    a1Global = ( a1Global* i + a1 )/(i+1);
    a2Global =( a2Global* i + a2 )/(i+1);
    a0Global =( a0Global* i + a0 )/(i+1);
    TGlobal =( TGlobal* i + T )/(i+1);
    nothingGlobal =( nothingGlobal * i + nothing )/(i+1);
    dGlobal =( dGlobal* i + d )/(i+1);
    tGlobal =( tGlobal* i + t )/(i+1);

    cout << "report" << endl;
    cout << "A1: " << a1Global << endl;
    cout << "A2: " << a2Global << endl;
    cout << "A0: " << a0Global << endl;
    cout << "T: " << TGlobal << endl;
    cout << "0: " << nothingGlobal << endl;
    cout << "D: " << dGlobal << endl;
    cout << "tiempo: " << tGlobal << endl;
    cout << endl;
}