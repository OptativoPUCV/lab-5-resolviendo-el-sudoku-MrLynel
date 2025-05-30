#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}


// Verifica que el nodo actual cumpla con las reglas del Sudoku.
int is_valid(Node* n)
{
  int i,j,k,p;
  //para verificar filas
  for(i = 0; i< 9; i++)
  {
    int fila[10] = {0};
    for(j= 0; j < 9;j++)
    {
      if(n->sudo[i][j]!= 0){
        if(fila[n->sudo[i][j]]== 1)
        {
          return 0; //no es valido
        }
        fila[n->sudo[i][j]] = 1;
      }
    }
  }

  //Para verificar columnas
  for(i = 0; i < 9;i++)
  {
    int col[10]= {0};
    for(j = 0; j < 9;j++)
    {
      if(n->sudo[j][i]!= 0)
      {
        if(col[n->sudo[j][i]]== 1)
        {
          return 0; //no es valido
        }
        col[n->sudo[j][i]] = 1;
      }
    }
  }

  //Para verificar submatrices
  for(k = 0; k < 9;k++)
  {
    int sub[10]= {0};
    for(p = 0; p< 9;p++)
    {
      int i = 3*(k/3)+(p/3);
      int j = 3*(k%3)+(p%3);
      if(n->sudo[i][j]!= 0)
      {
        if(sub[n->sudo[i][j]]== 1)
        {
          return 0; //no es valido
        }
        sub[n->sudo[i][j]] = 1;
      }
    }
  }
  return 1;
}


// Genera una lista con los nodos adyacentes al estado actual.
List* get_adj_nodes(Node* n)
{
  List* list=createList();
  int fila = -1;
  int col = -1;
  int i,j, num;
  for(i = 0; i < 9 && fila == -1; i++)
  {
    for(j = 0; j < 9 && fila == -1; j++)
    {
      if(n->sudo[i][j]== 0)
      {
        fila = i;
        col = j;
      }
    }
  }
  if(fila == 1 && col == -1)
  {
    return list; //no hay nodos adyacentes
  }
  for(num = 1; num <= 9; num++)
  {
    Node * new = copy(n);
    new->sudo[fila][col] = num;
    if(is_valid(new))
    {
      pushBack(list,new);
    }
    else
    {
      free(new);
    }
  }

  return list;
}

// Retorna 1 si el nodo representa un tablero completo, 0 si aún hay espacios 
int is_final(Node* n)
{
    int i, j;
    for(i = 0; i < 9; i++)
    {
      for(j = 0; j < 9; j++)
      {
        //todos los valores de la matriz son distintos a 0
        if(n->sudo[i][j] == 0)return 0;
      }
    }
  return 1; // retorna 1 si el nodo corresponde a un nodo final
}

// Realiza una búsqueda en profundidad hasta encontrar una solución al Sudoku.
Node* DFS(Node* initial, int* contador)
{
  Stack * pila = createStack();
  pushBack(pila, initial);
  Node* current;

  while(!is_empty(pila)) //no esta vacia
  {
    current = front(pila); 
    popFront(pila);
    (*contador)++;

    if(is_final(current)== 1)
    {
      clean(pila);
      return current;
    }
    List* adjNodes = get_adj_nodes(current);
    Node* node;
    while(!is_empty(adjNodes)) //no esta vacia
    {
      node = front(adjNodes); 
      popFront(adjNodes); 
      pushBack(pila, node); 
    }
    clean(adjNodes); 
    free(current);

  }
  clean(pila);
  return NULL;
}


/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/