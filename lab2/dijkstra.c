#include <stdio.h>
#include <stdlib.h>

#define NUM_NODES                          100
#define NONE                               9999

// Uncomment the desired data structure
//#define SLL
//#define DLL
//#define DYN_ARR

#if defined(SLL)
#include "../synch_implementations/cdsl_queue.h"
#endif
#if defined(DLL)
#include "../synch_implementations/cdsl_deque.h"
#endif
#if defined(DYN_ARR)
#include "../synch_implementations/cdsl_dyn_array.h"
#endif

struct _NODE
{
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
};
typedef struct _QITEM QITEM;

#if defined(SLL)
cdsl_sll *qHead;
iterator_cdsl_sll iter;
#endif
#if defined(DLL)
cdsl_dll *qHead;
iterator_cdsl_dll iter;
#endif
#if defined(DYN_ARR)
cdsl_dyn_array *qHead;
iterator_cdsl_dyn_array iter;
#endif
             
int AdjMatrix[NUM_NODES][NUM_NODES];

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int i, iCost, iDist;

void init_ds() {
  #if defined(SLL)
  qHead = cdsl_sll_init();
  #endif
  #if defined(DLL)
  qHead = cdsl_dll_init();
  #endif
  #if defined(DYN_ARR)
  qHead = cdsl_dyn_array_init();
  #endif
}

void print_path (NODE *rgnNodes, int chNode)
{
  if (rgnNodes[chNode].iPrev != NONE)
    {
      print_path(rgnNodes, rgnNodes[chNode].iPrev);
    }
  printf (" %d", chNode);
  fflush(stdout);
}


void enqueue (int iNode, int iDist, int iPrev)
{
  QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
  
  if (!qNew) 
    {
      fprintf(stderr, "Out of memory.\n");
      exit(1);
    }
  qNew->iNode = iNode;
  qNew->iDist = iDist;
  qNew->iPrev = iPrev;
  
  qHead->enqueue(0, qHead, (void *)qNew);
  g_qCount++;

}


void dequeue (int *piNode, int *piDist, int *piPrev)
{
  iter = qHead->iter_begin(qHead);
  QITEM *qKill = (QITEM *)qHead->iter_deref(qHead, iter);

  if (qHead)
    {
	
      *piNode = qKill->iNode;
      *piDist = qKill->iDist;
      *piPrev = qKill->iPrev;
      qHead->remove(0, qHead, qKill);
      g_qCount--;
    }
}


int qcount (void)
{
  return(g_qCount);
}

int dijkstra(int chStart, int chEnd) 
{
  

  
  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[ch].iDist = NONE;
      rgnNodes[ch].iPrev = NONE;
    }

  if (chStart == chEnd) 
    {
      printf("Shortest path is 0 in cost. Just stay where you are.\n");
    }
  else
    {
      rgnNodes[chStart].iDist = 0;
      rgnNodes[chStart].iPrev = NONE;
      
      enqueue (chStart, 0, NONE);
      
     while (qcount() > 0)
	{
	  dequeue (&iNode, &iDist, &iPrev);
	  for (i = 0; i < NUM_NODES; i++)
	    {
	      if ((iCost = AdjMatrix[iNode][i]) != NONE)
		{
		  if ((NONE == rgnNodes[i].iDist) || 
		      (rgnNodes[i].iDist > (iCost + iDist)))
		    {
		      rgnNodes[i].iDist = iDist + iCost;
		      rgnNodes[i].iPrev = iNode;
		      enqueue (i, iDist + iCost, iNode);
		    }
		}
	    }
	}
      
      printf("Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
      printf("Path is: ");
      print_path(rgnNodes, chEnd);
      printf("\n");
    }
}

int main(int argc, char *argv[]) {
  int i,j,k;
  FILE *fp;
  
  if (argc<2) {
    fprintf(stderr, "Usage: dijkstra <filename>\n");
    fprintf(stderr, "Only supports matrix size is #define'd.\n");
    
    exit(1);
  }

  /* open the adjacency matrix file */
  fp = fopen (argv[1],"r");

  /* make a fully connected matrix */
  for (i=0;i<NUM_NODES;i++) {
    for (j=0;j<NUM_NODES;j++) {
      /* make it more sparce */
      fscanf(fp,"%d",&k);
	AdjMatrix[i][j]= k;
    }
  }
  
  init_ds();

  /* finds 10 shortest paths between nodes */
  for (i=0,j=NUM_NODES/2;i<20;i++,j++) {
			j=j%NUM_NODES;
      dijkstra(i,j);
  }
  exit(0);
  

}