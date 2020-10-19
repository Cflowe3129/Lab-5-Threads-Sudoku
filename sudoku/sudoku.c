#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = { //rows 6 & 8, columns 5, 7, & 8, and boxes 7 & 8 should not pass
        {3,1,6, 5,7,8, 4,9,2},
        {5,2,9, 1,3,4, 7,6,8},
        {4,8,7, 6,2,9, 5,3,1},

        {2,6,3, 4,1,5, 9,8,7},
        {9,7,4, 8,6,3, 1,2,5},
        {8,5,1, 7,9,2, 6,4,3},

        {1,3,8, 9,4,7, 2,5,0},
        {6,9,2, 3,5,1, 8,7,4},
        {7,4,5, 2,8,0, 3,0,9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];
int counter = 0;


struct Coordinates {
  int row;
  int column;
};

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
    int column;
    int row = *((int *)args);
    bool seen[SIZE];
  
//     printf("%i THERE Row\n",row);
    for(column = 0; column < SIZE; column++) { //sets all values in the row to false
      seen[column] = false;
    }
  
    for(column = 0; column < SIZE; column++) { //iterates through each column in a given row
      if(board[row][column] == 0 || seen[board[row][column] - 1] == true) { //if an elements value is 0 or has been visited, the row fails
        row_check[row] = false;
        return NULL;
      } 
      seen[board[row][column] - 1] = true;
    }
  
    row_check[row] = true; //the row did not fail
      
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
    
    int row;
    int column = *((int *)args);
    bool seen[SIZE];
  
  
    for(row = 0; row < SIZE; row++) { //sets all values in the row to false
      seen[row] = false;
    }
  
    for(row = 0; row < SIZE; row++) { //iterates through each row in a given column
      if(board[row][column] == 0 || seen[board[row][column] - 1] == true) { //if an elements value is 0 or has been visited, the column fails
        col_check[column] = false;
        return NULL;
      } 
      seen[board[row][column] - 1] = true;
    }
  
    col_check[column] = true; //the column did not fail
  
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
  int row = ((struct Coordinates*)args)->row;
  int column = ((struct Coordinates*)args)->column;
  int rowReset = row;
  bool seen[SIZE];


  int i;
  for(i = 0; i < SIZE; i++) { //sets all values in the row to false
      seen[column] = false;
    }
  
  
  
  for(rowReset; rowReset < row+3; rowReset++) { //iterates through the row of a given grid
    
    int columnReset = column;
    for(columnReset;columnReset < column+3; columnReset++) { //iterates through the column of a given grid
//       printf("%i", board[rowReset][columnReset]); //print each individual subgrid
      
      if(board[rowReset][columnReset] == 0 || seen[board[rowReset][columnReset] - 1] == true) { //if an elements value is 0 or has been visited, the box fails

        box_check[counter++] = false;
        return NULL;
      } 
      seen[board[rowReset][columnReset] - 1] = true;
    }   
  }
    
  box_check[counter++] = true; //the box did not fail
}


// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.


    pthread_t thread_id1; 
    pthread_t thread_id2; 
    pthread_t thread_id3; 

    // 3. Create a thread for each cell of each matrix operation.
  
    int y;
    for(y = 0; y < SIZE; y++) {
      int x;
      for(x = 0; x < SIZE; x++) {
        int *argument = (int *) malloc(sizeof(*argument));
        int *argument2 = (int *) malloc(sizeof(*argument2));


        *argument = x;
        *argument2 = y;
        pthread_create(&thread_id1, NULL, checkRow, (void *)argument);
        pthread_create(&thread_id2, NULL, checkCol, (void *)argument2);
        
        if(x == 0 && y == 0 || x == 0 && y == 3 || x == 0 && y == 6 || x == 3 && y == 0 || x == 3 && y == 3 || x == 3 && y == 6 || x == 6 && y == 0 || x == 6 && y == 3 || x == 6 && y == 6) {
          struct Coordinates *coordinates = (struct Coordinates*) malloc(sizeof(coordinates));
          coordinates->row = y;
          coordinates->column = x;
          pthread_create(&thread_id3, NULL, checkBox, coordinates);
          pthread_join(thread_id3, NULL);

        }
      
      }
      

    }

  // 4. Wait for all threads to finish.
    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);


    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        } else {
            printf("Row %i passed\n", i);
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        } else {
            printf("Col %i passed\n", i);

        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        } else {
            printf("Box %i passed\n", i);
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}

