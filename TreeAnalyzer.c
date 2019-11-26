#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"

/*   Macro   */

/**  Signs for end of line  */
#define END_LINE "\r\n"
/**  Closes the file and returns a value  */
#define CLOSE_AND_RETURN(X) fclose(fp); return X;
/**  free the graph release and returns a value  */
#define FREE_AND_RETURN(X)  freeAllGraph(&myGraph); return X;
/**  String containing space  */
#define SPACE " "
/**  String with valid characters in the file  */
#define POSSIBLE " -1234567890"
/**  Maximum beam length  */
#define LINE_MAX_SIZE 1024

/*   Constants   */

/**  A string representing a line that leaf  */
const char *LEAF_LINE = "-";
/**  End of line mark of Windows  */
const char NEW_LINE_R = '\r';
/**  A sign for the end of a string  */
const char END_OF_STRING = '\0';
/** String that triggers file reading   */
char *const READ = "r";

/** Message - The number of parameters is incorrect   */
const char *NUM_OF_PARM_MSG =
        "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n";
/** Message - Invalid input   */
const char *INVALID_INPUT_MSG = "Invalid input\n";
/** Message - The graph is not a tree   */
const char *NO_TREE_MSG = "The given graph is not a tree\n";
/**  Print - Minimum length of track in tree  */
const char *MIN_BRANCH_MSG = "Length of Minimal Branch: %d\n";
/**  Print - The maximum length of the tree   */
const char *MAX_BRANCH_MSG = "Length of Maximal Branch: %d\n";
/**  Print - The diameter of the tree  */
const char *DIAMETER_MSG = "Diameter Length: %d\n";
/**  Print - Number of vertices  */
const char *BER_MSG = "Vertices Count: %d\n";
/**  Print - Number of ribs  */
const char *EDGES_MSG = "Edges Count: %d\n";
/**  Print - Root vertex  */
const char *ROOT_MSG = "Root Vertex: %d\n";
/**  Print - Track between two vertices  */
const char *SHORT_PATH_MSG = "Shortest Path Between %d and %d:";

/**  Represents a valid value  */
const int VALID = 0;
/**  Represents invalid value  */
const int INVALID = -1;
/**  Represents a line that is a vertex without children  */
const int NO_CHILDS = -2;

/** Empty value   */
const int EMPTY = 0;
/**  Location of the array - the graph file  */
const int PATH_TO_GRAPH_FILE = 1;
/**  Array location - first vertex  */
const int VER_1 = 2;
/**  Array location - second vertex  */
const int VER_2 = 3;


/*   Structures   */

/**
 * @brief Represents one vertex in the graph
 */
typedef struct Node
{
    /**  Pointer to an array of vertices, the vertex is attached to them  */
    struct Node **_neighbors;
    /**  Pointer to the vertex that led to it in the BFS run   */
    struct Node *_prev;
    /**  The value of the vertex   */
    int _key;
    /**  Number of neighbors of the vertex   */
    int _num_of_neighbors;
    /**  Is there a vertex pointing to a vertex   */
    int _hasDad;
    /**  The distance calculated in the BFS run   */
    int _dist;

} Node;

/**
 * @brief A structure representing a graph
 */
typedef struct Graph
{
    /**  Pointer to the root vertex of the graph   */
    Node *_head;
    /**  Pointer to an array containing all vertices in the graph   */
    Node *_allNodes;
    /**  Number of vertices in the graph   */
    int _numOfNodes;
    /**  Number of ribs in graph   */
    int _numOfRib;

} Graph;


/*   Input processing   */

/**
 * @brief A function that calls an integer from a string
 * @param string A string containing a number
 * @return Invalid number, written in string, -2 if the line leaf.-1 or >-2 if the string is invalid
 */
int readNum(const char *string)
{
    char *end = NULL;
    double number;
    /*  Is the string a "-" representing a leaf        */
    if (!strcmp(string, LEAF_LINE))
    {
        return NO_CHILDS;
    }
    number = strtod(string, &end); // get number
    /*  Check that the number is valid  */
    if ((*end != END_OF_STRING) || ((double) (int) number != number) || (number < VALID))
    {
        return INVALID;
    }
    return (int) number;
}

/**
 * @brief A function that reads a line from the graph file
 * @param fp File representing the graph structure
 * @param line An array of characters to fill in the line
 * @return 0 If the reading was successful. -1 otherwise
 */
int getLine(FILE *fp, char *line)
{
    int i = EMPTY;
    char c = (char) getc(fp);
    if (c == EOF)
    {
        return INVALID;
    }
    while (!strchr(END_LINE, c) && (c != EOF))
    {
        if (!strchr(POSSIBLE, c))
        {
            return INVALID;
        }
        line[i] = c;
        i++;
        c = (char) getc(fp); // Reading the next letter
        if (c ==
            NEW_LINE_R) //In Windows, there are two signs for the end of a line, so call the next one
        {
            c = (char) getc(fp);
        }
    }
    line[i] = END_OF_STRING;
    return EXIT_SUCCESS;
}


/*   Algorithms in the graph   */

/**
 * @brief An incoming function, recursively, for all vertices that descend from a particular vertex.
 * @param pNode Pointer to the vertex from which you are checking
 * @param pInt Pointer to number of vertices tested, The vertex adds itself
 */
void explor(Node *pNode, int *pInt)
{
    (*pInt)++;
    for (int i = 0; i < pNode->_num_of_neighbors - 1; ++i)
    {
        explor(pNode->_neighbors[i], pInt);
    }
}

/**
 * @brief Breadth-first search algorithm in the graph
 * @param myGraph Object that is the graph
 * @param starPointP Point to the vertex from which the search begins
 * @param destination Search target, NULL if none
 */
void BFS(Graph *myGraph, Node *starPointP, Node *destination)
{
    for (int i = 0; i < myGraph->_numOfNodes; ++i)
    {
        myGraph->_allNodes[i]._dist = myGraph->_numOfNodes;
    }
    starPointP->_dist = EMPTY;
    starPointP->_prev = NULL;

    Queue *myQueue = allocQueue();
    enqueue(myQueue, starPointP->_key);
    while (!queueIsEmpty(myQueue))
    {
        Node *u = &myGraph->_allNodes[dequeue(myQueue)];
        if (u == destination)
        {
            freeQueue(&myQueue);
            return;
        }
        for (int i = 0; i < u->_num_of_neighbors; ++i)
        {
            if ((u->_neighbors[i] != NULL) && (u->_neighbors[i]->_dist == myGraph->_numOfNodes))
            {
                enqueue(myQueue, u->_neighbors[i]->_key);
                u->_neighbors[i]->_dist = u->_dist + 1;
                u->_neighbors[i]->_prev = u;
            }
        }
    }
    freeQueue(&myQueue);
}

/**
 * @brief Calculate the minimum and maximum track length in the graph, by the dist field
 * @param myGraph Object that is the graph
 * @param maxP Pointer to the maximum number
 * @param minP Pointer to the minimum number
 * @param maxNodeP A pointer to a pointer that will maintain the vertex with the maximum distance
 */
void minAndMax(Graph *myGraph, int *maxP, int *minP, Node **maxNodeP)
{
    *minP = myGraph->_numOfNodes;
    *maxP = -1;

    for (int i = 0; i < myGraph->_numOfNodes; ++i)
    {
        if ((myGraph->_allNodes[i]._num_of_neighbors == 1) && (*minP > myGraph->_allNodes[i]._dist))
        {
            *minP =  myGraph->_allNodes[i]._dist;
        }
        if (*maxP < myGraph->_allNodes[i]._dist)
        {
            *maxP = myGraph->_allNodes[i]._dist;
            *maxNodeP = &(myGraph->_allNodes[i]);
        }
    }
}


/*   Calculate and print information   */

/**
 * @brief Prints graph details
 * @param myGraph Object that is the graph
 */
void printGraphDetails(Graph *myGraph)
{
    printf(ROOT_MSG, myGraph->_head->_key);
    printf(BER_MSG, myGraph->_numOfNodes);
    printf(EDGES_MSG, myGraph->_numOfRib);
}

/**
 * @brief A computer and prints a minimum and maximum trajectory on the Tree
 * @param myGraph Object that is the graph
 * @param maxP Pointer to the maximum number
 * @param minP Pointer to the minimum number
 * @param maxNodeP A pointer to a pointer that will maintain the vertex with the maximum distance
 */
void printMinMax(Graph *myGraph, int *minP, int *maxP, Node **maxNode)
{
    BFS(myGraph, myGraph->_head, NULL);
    minAndMax(myGraph, maxP, minP, maxNode);
    printf(MIN_BRANCH_MSG, *minP);
    printf(MAX_BRANCH_MSG, *maxP);
}

/**
 * @brief Calculates and prints the diameter of the tree
 * @param myGraph Object that is the graph
 * @param maxP Pointer to the maximum number
 * @param minP Pointer to the minimum number
 * @param maxNodeP A pointer to a pointer that will maintain the vertex with the maximum distance
 */
void printDiameter(Graph *myGraph, int *minP, int *maxP, Node **maxNode)
{
    BFS(myGraph, *maxNode, NULL);
    minAndMax(myGraph, maxP, minP, maxNode);
    printf(DIAMETER_MSG, *maxP);
}

/**
 * @brief Computer and print track in wood between two vertices
 * @param myGraph Object that is the graph
 * @param pNodeSor Pointer to the source vertex
 * @param pNodeDast Point to the destination vertex
 */
void getPath(Graph *myGraph, Node *pNodeSor, Node *pNodeDast)
{
    BFS(myGraph, pNodeDast, pNodeSor);
    Node *curNode = pNodeSor;
    printf(SHORT_PATH_MSG, pNodeSor->_key, pNodeDast->_key);
    while (curNode != NULL)
    {
        printf(" %d", curNode->_key);
        curNode = curNode->_prev;
    }
}


/*   Create the graph and test it   */

/**
 * @brief A function that creates one vertex in the graph, following the instructions in the file
 * @param myGraph Object that is the graph
 * @param fp The guidelines file
 * @param line Pointer to an array of characters, to read the following lines
 * @param verNum The vertex number built
 * @param neighborsArr An array pointer saves the numbers of all neighboring vertices to the vertex
 * @return 0 If the vertex was successfully created. -1 otherwise.
 */
int oneNode(Graph *myGraph, FILE *fp, char *line, int *neighborsArr, int verNum)
{
    if (getLine(fp, line) < VALID)
    {
        return INVALID;
    }

    char *oneWord = strtok(line, SPACE);
    if (oneWord == NULL)
    {
        return INVALID;
    }

    int numOfNeighbors = EMPTY;
    while (oneWord != NULL)
    {
        neighborsArr[numOfNeighbors] = readNum(oneWord); // Saving the number
        /* In the case of a vertex no children */
        if ((neighborsArr[numOfNeighbors] == NO_CHILDS) && (numOfNeighbors == EMPTY))
        {
            break;
        }
        /* Tests that the number is valid */
        if ((neighborsArr[numOfNeighbors] >= myGraph->_numOfNodes) ||
            neighborsArr[numOfNeighbors] < VALID)
        {
            return INVALID;
        }
        numOfNeighbors++;
        oneWord = strtok(NULL, SPACE);
    }
    /*  Adds the number of neighbors to the number of ribs in the entire graph  */
    myGraph->_numOfRib += numOfNeighbors;
    /*  Creates an array of pointers, guarding all its neighbors at the apex  */
    myGraph->_allNodes[verNum]._neighbors = (Node **) malloc((numOfNeighbors + 1) * sizeof(Node *));
    if (myGraph->_allNodes[verNum]._neighbors == NULL)
    {
        return INVALID;
    }
    Node ** myNeigb = myGraph->_allNodes[verNum]._neighbors;
    myGraph->_allNodes[verNum]._num_of_neighbors = numOfNeighbors + 1;

    /*  Moves on all neighbors and adds their pointer to the neighbors array  */
    for (int neigbor = 0; neigbor < numOfNeighbors; neigbor++)
    {
        myNeigb[neigbor] = &(myGraph->_allNodes[neighborsArr[neigbor]]);
        myNeigb[neigbor]->_hasDad += 1;
    }
    myNeigb[numOfNeighbors] = NULL; // The last vertex will mark the father
    return EXIT_SUCCESS;
}

/**
 * @brief Creates and fills all vertices in the graph, according to the guidelines file
 * @param filePath String, path to the guidelines file
 * @param myGraph Object that is the graph
 * @return 0 with successful construction, -1 otherwise
 */
int creatAllNods(char *filePath, Graph *myGraph)
{
    FILE *fp = fopen(filePath, READ);
    if (fp == NULL)
    {
        return INVALID;
    }

    char line[LINE_MAX_SIZE];
    int numOfNodes;
    char *oneWord = NULL;

    if (getLine(fp, line) < VALID)
    {
        CLOSE_AND_RETURN(INVALID)
    }

    oneWord = strtok(line, SPACE); // Line dividing by spaces
    if ((oneWord == NULL) || ((numOfNodes = readNum(oneWord)) < VALID))
    {
        CLOSE_AND_RETURN(INVALID)
    }

    oneWord = strtok(NULL, SPACE); // If there are other words in the front row - invalid
    if (oneWord != NULL)
    {
        CLOSE_AND_RETURN(INVALID)
    }

    myGraph->_numOfNodes = numOfNodes;
    /*  Create an array for all vertices  */
    myGraph->_allNodes = (Node *) calloc(numOfNodes, sizeof(Node));
    if (myGraph->_allNodes == NULL)
    {
        CLOSE_AND_RETURN(INVALID)
    }

    for (int i = 0; i < numOfNodes; ++i)
    {
        myGraph->_allNodes[i]._key = i;
        myGraph->_allNodes[i]._hasDad = EMPTY;
    }
    /*  An array for saving all neighboring vertices to the current vertex.  */
    int *neighborsArr = (int *) malloc((myGraph->_numOfNodes) * sizeof(int));
    if (neighborsArr == NULL)
    {
        CLOSE_AND_RETURN(INVALID)
    }

    /*  Fill all the vertices */
    for (int i = 0; i < numOfNodes; ++i)
    {
        if (oneNode(myGraph, fp, line, neighborsArr, i) < VALID)
        {
            free(neighborsArr);
            CLOSE_AND_RETURN(INVALID)
        }
    }
    free(neighborsArr);

    /*  Check that there are no more rows in the file  */
    if (getLine(fp, line) >= VALID)
    {
        CLOSE_AND_RETURN(INVALID)
    }
    CLOSE_AND_RETURN(EXIT_SUCCESS)
}

/**
 * @brief Checking whether the graph is a tree
 * @param myGraph Object that is the graph
 * @return 0 If yes, -1 if not.
 */
int checkGraph(Graph *myGraph)
{
    if (myGraph->_numOfNodes - 1 != myGraph->_numOfRib) // Number of ribs is valid
    {
        return INVALID;
    }

    /*   Root marking and testing with no more than one root and no vertex with two ancestors   */
    for (int i = 0; i < myGraph->_numOfNodes; ++i)
    {
        if ((myGraph->_head == NULL) && (myGraph->_allNodes[i]._hasDad == EMPTY))
        {
            myGraph->_head = &myGraph->_allNodes[i];
            continue;
        }
        if (myGraph->_allNodes[i]._hasDad != 1)
        {
            return INVALID;
        }
    }
    if (myGraph->_head == NULL)
    {
        return INVALID;
    }

    /*  Check that the graph is Ties  */
    int tiable = EMPTY;
    explor(myGraph->_head, &tiable);
    if (tiable != myGraph->_numOfNodes)
    {
        return INVALID;
    }
    return EXIT_SUCCESS;
}

/**
 * @brief In case the graph is a tree, the graph is turned unintentionally
 * (adding the father as a neighbor)
 * @param pNode Pointer to the vertex that turns
 */
void reversRib(Node *pNode)
{
    for (int i = 0; i < pNode->_num_of_neighbors - 1; ++i)
    {
        pNode->_neighbors[i]->_neighbors[pNode->_neighbors[i]->_num_of_neighbors - 1] = pNode;
        reversRib(pNode->_neighbors[i]);
    }
}

/**
 * @brief Release the entire space assigned to the graph
 * @param myGraph Object that is the graph
 */
void freeAllGraph(Graph **myGraph)
{
    for (int i = 0; i < (*myGraph)->_numOfNodes; ++i)
    {
        free((*myGraph)->_allNodes[i]._neighbors);
    }
    free((*myGraph)->_allNodes);
    free(*myGraph);
    *myGraph = NULL;
}

/**
 * @brief Initializing a new graph
 * @return Pointing to the new graph, the user must release all assigned space
 */
Graph *initGraph()
{
    Graph *myGraph = (Graph *) malloc(sizeof(Graph));
    myGraph->_numOfRib = EMPTY;
    myGraph->_numOfNodes = EMPTY;
    myGraph->_head = NULL;
    myGraph->_allNodes = NULL;
    return myGraph;
}


/*   Main function   */

/**
 * @brief Main function, creates the graph, by the file. Calculates and prints all requirements
 * @param argc The number of arguments received from the user
 * @param argv The array of arguments received from the user
 * @return 0 If everything went through successfully. 1 otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != 4) // Checking the number of arguments
    {
        fprintf(stderr, "%s", NUM_OF_PARM_MSG);
        return EXIT_FAILURE;
    }

    /*  Create and initialize the graph */
    Graph *myGraph = initGraph();
    if (creatAllNods(argv[PATH_TO_GRAPH_FILE], myGraph) < VALID)
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        FREE_AND_RETURN(EXIT_FAILURE)
    }

    /*  Get the numbers of the vertices to calculate the route */
    int num1 = readNum(argv[VER_1]);
    int num2 = readNum(argv[VER_2]);
    if ((num1 < VALID) || (num1 >= myGraph->_numOfNodes) ||
        (num2 < VALID) || (num2 >= myGraph->_numOfNodes))
    {
        fprintf(stderr, "%s", INVALID_INPUT_MSG);
        FREE_AND_RETURN(EXIT_FAILURE)
    }

    if (checkGraph(myGraph) < VALID) // Check that the graph is a tree
    {
        fprintf(stderr, "%s", NO_TREE_MSG);
        FREE_AND_RETURN(EXIT_FAILURE)
    }

    /*  Calculate and print all requested information */
    printGraphDetails(myGraph);
    reversRib(myGraph->_head);

    Node *maxNode = NULL;
    int max, min;
    printMinMax(myGraph, &min, &max, &maxNode);
    printDiameter(myGraph, &min, &max, &maxNode);
    getPath(myGraph, &myGraph->_allNodes[num1], &myGraph->_allNodes[num2]);
    FREE_AND_RETURN(EXIT_SUCCESS)
}