#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// void importdata(){
//
// }

// void runalgo(){
//
// }
//
// void runparallelalgo(){
//
// }

void

int main(){

  int BUFSIZE = 32767; //max number of lines
  char** words = (**char)malloc(sizeof(*char));
  FILE *fp = fopen("../project_data/training_data/mv_0000001.txt", "r");
  if(fp == 0){
    fprintf(stderr, "Error opening file");
    exit(1);
  }

  int i = 0;
  words[i] = malloc(BUFSIZE);
  while(fscanf(fp, "%100s", words[i]) == 1) //line no longer than 100
  {
        i++;
        words[i] = realloc(words, sizeof(char*)*i);
  }

  int j;
  for(j = 0; j < i; j++){
    printf("%s\n", words);
  }

  //Import data from Netflix
  // importdata();

  //Run algorithm
  // runalgo();

  //Run parallel algorithm
  // runparallelalgo();

}
