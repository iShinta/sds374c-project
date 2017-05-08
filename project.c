#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

int main(){
  double i0, i1, i2, i3, i4, i5, i6;
  printf("----- Program Start -----\n");

  //Chrono 1
  i0 = clock();

  printf("----- Initialize Variables -----\n");
  int lines_allocated = 128;
  int max_line_len = 100;
  int nb_movies = 5; //4444
  int nb_user = 481000;
  int max_user = 0;
  /* Allocate lines of text */
  char **words;
  // User translation table
  int *user_array;
  user_array = (int *)malloc(sizeof(int)*nb_user);      // allocate 481000 ints
  int ii;
  for(ii = 0; ii < nb_user; ii++){
    user_array[ii] = -1; //Assign default value
  }
  // Ratings array
  int **rating_array = (int**)malloc(sizeof(int*)*nb_movies); //5000 movies
  for(ii = 0; ii < nb_movies; ii++){
    rating_array[ii] = (int*)malloc(sizeof(int)*nb_user);
  }
  for(ii = 0; ii < nb_movies; ii++){ //Initialization of all at -1
    int j;
    for(j = 0; j < nb_user; j++){
      rating_array[ii][j] = -1;
    }
  }
  // Movie average array
  double *movie_average_array;
  movie_average_array = (double *)malloc(sizeof(double)*nb_movies); //5000 movies

  // User movie average array
  double *user_movie_average_array;
  user_movie_average_array = (double *)malloc(sizeof(double)*nb_user); //5000 movies

  //Chrono 1
  i1 = clock();

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
    int trouve;
    for(j = 1; j < i; j++){ //Don't include 0 which is the first line which is the ID of the movie
      //User
      char *pt = strtok(words[j], ",");

      //printf("First %s\n", pt);
      //Saving to Translation Table
      int k;
      trouve = 0;
      for(k = 0; k < nb_user; k++){
        if(user_array[k] == atoi(pt)){ //User found
          // printf("------ User %i Found ------\n", k);
          // break;
          trouve = k;
        }
      }

      // User not found
      if(trouve == 0){
        printf("Add user at %i\n", max_user);
        user_array[max_user] = atoi(pt);
        trouve = max_user;
        max_user++;
      }else{
        printf("User found at %i\n", trouve);
      }
      // else if(user_array[k] == -1){
      //   // printf("------ User %i Added ------\n", k);
      //   max_user++;
      //   user_array[k] = atoi(pt);
      //   break;
      // }

      //Rating
      pt = strtok(NULL, ",");
      int movie_nb = atoi(strtok(words[0], ":"));
      rating_array[movie_nb][trouve] = atoi(pt); //Position is trouve
    }

    printf("------ Freeing Memory ------\n", ii);
    /* Good practice to free memory */
    for (;i>=0;i--)
        free(words[i]);
    free(words);
  }

  //Chrono 2
  i2 = clock();

  printf("----- Calculate Movie Averages -----\n");
  //Calculate Movie averages
  int tt_counter = 0;
  int counter;
  int tt_sum = 0;
  int sum;
  double tt_average;
  for(ii = 0; ii < nb_movies; ii++){
    printf("------ Movie %i Average ------\n", ii);
    int k;
    counter = 0;
    sum = 0;
    for(k = 0; k < max_user ; k++){
      if(rating_array[ii][k] != -1){
        counter++;
        sum+=rating_array[ii][k];
      }
    }

    tt_counter += counter;
    tt_sum += sum;

    printf("Movie Sum: %i\n", sum);
    printf("Movie Counter: %i\n", counter);
    movie_average_array[ii] = (double)sum/counter;
    printf("Movie Average: %f\n", movie_average_array[ii]);
  }

  // Global Average
  tt_average = (double)tt_sum/tt_counter;

  //Chrono 3
  i3 = clock();

  printf("----- Calculate User Offsets -----\n");
  //Calculate User Offset
  for(ii = 0; ii < max_user; ii++){ //Go through users
    printf("------ User %i / %i Average ------\n", ii, max_user);
    int k;
    int sum = 0;
    int count = 0;
    for(k = 0; k < nb_movies; k++){
      if(rating_array[k][ii] != -1){
        count++;
        sum += rating_array[k][ii];
      }
    }
    printf("User Movie Sum: %i\n", sum);
    printf("User Movie Counter: %i\n", count);
    user_movie_average_array[ii] = (double)sum/count - tt_average; // Offset
    printf("User Movie Offset: %f / %f\n", user_movie_average_array[ii], tt_average);
  }

  //Chrono 4
  i4 = clock();

  //Prediction
  printf("----- Read Probe File -----\n");
  // Open the file
  FILE *fp2;

  words = (char **)malloc(sizeof(char*)*lines_allocated);
  if (words==NULL)
  {
     fprintf(stderr,"Out of memory (1).\n");
     exit(1);
  }

  fp2 = fopen("probe.txt", "r");
  if (fp2 == NULL)
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
  fclose(fp2);

  //Chrono 5
  i5 = clock();

  int j;
  int movie_id_pred = 1; //Change to any movie index (here 1)
  printf("----- Prediction Movie %i -----\n", movie_id_pred);
  for(j = 1; j < i; j++){
    // printf("------ Prediction User %i ------\n", atoi(words[j]));

    //Find user
    int k;
    for(k = 0; k < max_user; k++){
      if(user_array[k] == atoi(words[j])){
        // printf("Trouve!");
        break;
      }
    }

    //Find average movie score
    int movie_score = movie_average_array[movie_id_pred];

    //Add User's Offset
    printf("Prediction for user %i is %f\n", atoi(words[j]), movie_score + user_movie_average_array[k]);
  }

  //Chrono 6
  i6 = clock();

  printf("\n\n\nSummary\n");
  printf("-------\n");
  printf("Total Number of Movies                       ::              %i\n", 17770);
  printf("Total Number of Users                        ::              %i\n", nb_user);
  printf("Studied Number of Movies                     ::              %i\n", nb_movies);
  printf("Studied Number of Users                      ::              %i\n", max_user);
  // printf("Total number of inner elements            ::              %i\n", (n-2)*(n-2));
  // printf("Memory (GB) used per array                ::              %f\n", sizeof(double)*n*n);
  // printf("Threshold                                 ::              %3.2f\n", t);
  // printf("Smoothing constants (a, b, c)             ::              %3.2f %3.2f %3.2f\n", a, b, c);
  // printf("Number    of elements below threshold (X) ::              %i\n", resx);
  // printf("Fraction  of elements below threshold     ::              %f\n", ((double)resx/(n*n)));
  // printf("Number    of elements below threshold (Y) ::              %i\n", resy);
  // printf("Fraction  of elements below threshold     ::              %f\n", ((double)resy/((n-2)*(n-2))));
  // printf("\nAction        ::  time/s    Time Resolution = 1.0E-04\n");
  printf("-------\n");
  printf("CPU: Variables Initialization       ::  %f\n", (i1 - i0) / (float)CLOCKS_PER_SEC);
  printf("CPU: Files Reading & Matrix Filling ::  %f\n", (i2 - i1) / (float)CLOCKS_PER_SEC);
  printf("CPU: Calculate Movie Averages       ::  %f\n", (i3 - i2) / (float)CLOCKS_PER_SEC);
  printf("CPU: Calculate User Offset          ::  %f\n", (i4 - i3) / (float)CLOCKS_PER_SEC);
  printf("CPU: Users to Predict Reading       ::  %f\n", (i5 - i4) / (float)CLOCKS_PER_SEC);
  printf("CPU: Users Movie Prediction         ::  %f\n", (i6 - i5) / (float)CLOCKS_PER_SEC);

  printf("----- Program End -----\n");
  return 0;
}
