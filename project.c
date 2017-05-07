#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(){
  printf("----- Program Start -----\n");
  printf("----- Initialize Variables -----\n");
  int lines_allocated = 128;
  int max_line_len = 100;
  int nb_movies = 20; //4444
  /* Allocate lines of text */
  char **words;
  // User translation table
  int *user_array;
  user_array = (int *)malloc(sizeof(int)*481000);      // allocate 481000 ints
  int ii;
  for(ii = 0; ii < 481000; ii++){
    user_array[ii] = -1; //Assign default value
  }
  // Ratings array
  int **rating_array = (int**)malloc(sizeof(int*)*nb_movies); //5000 movies
  for(ii = 0; ii < nb_movies; ii++){
    rating_array[ii] = (int*)malloc(sizeof(int)*481000);
  }
  for(ii = 0; ii < nb_movies; ii++){ //Initialization of all at -1
    int j;
    for(j = 0; j < 481000; j++){
      rating_array[ii][j] = -1;
    }
  }
  // Movie average array
  double *movie_average_array;
  movie_average_array = (double *)malloc(sizeof(double)*nb_movies); //5000 movies

  printf("----- Read Files -----\n");
  // Open the file
  FILE *fp;

  for(ii = 1; ii < nb_movies ; ii++){ // Starts at 1 because no movie 0
    words = (char **)malloc(sizeof(char*)*lines_allocated);
    if (words==NULL)
    {
       fprintf(stderr,"Out of memory (1).\n");
       exit(1);
    }

    printf("------ Read Movie %i ------\n", ii);
    char strlink_beg[50] = "../project_data/training_data/mv_";
    if(ii<10){
      strcat(strlink_beg, "000000");
    }else if(ii<100){
      strcat(strlink_beg, "00000");
    }else if(ii<1000){
      strcat(strlink_beg, "0000");
    }else if(ii<10000){
      strcat(strlink_beg, "000");
    }else if(ii<100000){
      strcat(strlink_beg, "00");
    }else if(ii<1000000){
      strcat(strlink_beg, "0");
    }
    char ii_nb[20];
    sprintf(ii_nb, "%d", ii);
    strcat(strlink_beg, ii_nb);
    strcat(strlink_beg, ".txt");
    // printf("File read: %s\n", strlink_beg);

    fp = fopen(strlink_beg, "r");
    if (fp == NULL)
    {
        fprintf(stderr,"Error opening file.\n");
        exit(2);
    }

    printf("------ Read Lines ------\n");
    // iterations until break
    int i;
    for (i=0;1;i++)
    {
        int j;
        /* Have we gone over our line allocation? */
        if (i >= lines_allocated)
        {
            int new_size;

            /* Double our allocation and re-allocate */
            new_size = lines_allocated*2;
            words = (char **)realloc(words,sizeof(char*)*new_size);
            if (words==NULL)
            {
                fprintf(stderr,"Out of memory.\n");
                exit(3);
            }
            lines_allocated = new_size;
        }

        /* Allocate space for the next line */
        words[i] = malloc(max_line_len);
        if (words[i]==NULL)
        {
            fprintf(stderr,"Out of memory (3).\n");
            exit(4);
        }
        if (fgets(words[i],max_line_len-1,fp)==NULL)
            break;

        /* Get rid of CR or LF at end of line */
        for (j=strlen(words[i])-1;j>=0 && (words[i][j]=='\n' || words[i][j]=='\r');j--)
          ;
          words[i][j+1]='\0';
    }

    /* Close file */
    fclose(fp);

    printf("------ Filling Matrix ------\n");
    // Per line, got a string in format: 124105,4,2004-08-05
    // Split per ",". First is UserID, Second is Ratings
    // Saving UserID in Translation table
    // Assign Rating 2D matrix with new UserID
    // j is the current line, i is the total number of lines
    int j;
    for(j = 1; j < i; j++){ //Don't include 0 which is the first line which is the ID of the movie
      //User
      char *pt = strtok(words[j], ",");
      //printf("First %s\n", pt);
      //Saving to Translation Table
      int k;
      for(k = 0; k < 481000; k++){
        if(user_array[k] == atoi(pt)){
          // printf("------ User %i Found ------\n", k);
          break;
        }else if(user_array[k] == -1){
          // printf("------ User %i Added ------\n", k);
          user_array[k] = atoi(pt);
          break;
        }
      }

      //Rating
      pt = strtok(NULL, ",");
      int movie_nb = atoi(strtok(words[0], ":"));
      rating_array[movie_nb][k] = atoi(pt); //Position is k
    }

    printf("------ Freeing Memory ------\n", ii);
    /* Good practice to free memory */
    for (;i>=0;i--)
        free(words[i]);
    free(words);
  }

  printf("----- Calculate Movie Averages -----\n");
  //Calculate Movie averages
  int counter;
  int sum;
  for(ii = 0; ii < nb_movies; ii++){
    printf("------ Movie %i Average ------\n", ii);
    int k;
    counter = 0;
    sum = 0;
    for(k = 0; k < 481000 ; k++){
      if(rating_array[ii][k] != -1){
        counter++;
        sum+=rating_array[ii][k];
      }
    }
    printf("Movie Sum: %i\n", sum);
    printf("Movie Counter: %i\n", counter);
    printf("Movie Average (Calcul): %f\n", (double)sum/(double)counter);
    movie_average_array[ii] = (double)sum/counter;
    printf("Movie Average: %f\n", movie_average_array[ii]);
  }

  printf("----- Calculate User Offsets -----\n");
  //Calculate User Offset
  for(ii = 0; ii < 481000; ii++){ //Go through users
    //printf("------ User %i Average ------\n", ii);
    int k;
    int sum;
    int count;
    for(k = 0; k < nb_movies; k++){
      if(rating_array[k][ii] != -1){
        sum += rating_array[k][ii];
      }
    }
  }

  //Prediction

  printf("----- Program End -----\n");
  return 0;
}
