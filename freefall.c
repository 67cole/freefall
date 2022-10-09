// Author: Colin Ngo
// Date: 25/6/2020
// Terminal based space invaders game
// Project in spare time!

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 15
#define EMPTY 0
#define STONE 1
#define MARCHING 2

#define MOVEMENT 1
#define LASER 2
#define SHIFT 3
#define FLIP 4
#define GRAVITY 5

#define LEFT 1
#define TOP_LEFT 2
#define TOP_RIGHT 3
#define RIGHT 4

void printMap(int map[SIZE][SIZE], int playerX);
void placeStones(int lines, int map[SIZE][SIZE], int negMap[150][SIZE]);
void movement(int *move);
void laser(int map[SIZE][SIZE], int playerX, int *condition); 
void shift(int map[SIZE][SIZE], int playerX, int *condition, int *shifts, int *left, int negMap[150][SIZE], int *negRow);
void flip(int map[SIZE][SIZE], int *count, int playerX);
void gravity(int map[SIZE][SIZE], int playerX, int *count);

int main (void) {
    // This line creates our 2D array called "map" and sets all
    // of the blocks in the map to EMPTY.
    int map[SIZE][SIZE] = {EMPTY};
    int negMap[150][SIZE] = {EMPTY};

    // This line creates out playerX variable. The player starts in the
    // middle of the map, at position 7.
    int playerX = SIZE / 2;

    printf("How many lines of stone? ");
    int lines;
    scanf("%d", &lines);

    int tempArray = 0;
    printf("Enter lines of stone:\n");
    placeStones(lines, map, negMap);
    printMap(map, playerX);

    int command;
    int gameCondition = 0;
    int flip_used = 0;
    int shift_used = 0;
    int marching = 0;
    int gravity_used = 0;
    while (gameCondition == 0 && scanf("%d", &command) != EOF) {
        if (command == MOVEMENT) {
            movement(&playerX);
            printMap(map, playerX);
        }
        else if (command == LASER) {
            laser(map, playerX, &gameCondition);
        }
        else if (command == SHIFT) {
            shift(map, playerX, &gameCondition, &shift_used, &marching, negMap, &tempArray);
        }
        else if (command == FLIP) {
            flip(map, &flip_used, playerX);
        }
        else if (command == GRAVITY) {
            gravity(map, playerX, &gravity_used);
        }
        
    }
    
    return 0;
}

// Print out the contents of the map array. Then print out the player line
// which will depends on the playerX variable.
void printMap(int map[SIZE][SIZE], int playerX) {
    
    // Print values from the map array.
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }    
    
    // Print the player line.
    i = 0;
    while (i < playerX) {
        printf("  ");
        i++;
    }
    printf("P\n");
}

void placeStones(int lines, int map[SIZE][SIZE], int negMap[150][SIZE]) {
    int row;
    int column;
    int length;
    int value;
    
    int i = 0;
    while (i < lines) {
        scanf("%d %d %d %d", &row, &column, &length, &value);
        
        int j = 0;
        if ((column + length) > SIZE || column >= SIZE || row >= SIZE || column < 0) {
            i++;
        }
        else if (row < 0) {
            row = (row * -1) - 1;
            while (j < length) {
                negMap[row][column++] = value;
                j++;
            }
            i++;
        }    
        else {
            while (j < length) {
                map[row][column++] = value;
                j++;
            }
            i++;
        }
        
    }
}
    
void movement(int *move) {
    int movement;
    scanf("%d", &movement);
    
    if (movement == 1) {
        if (*move != (SIZE - 1)) {
            *move = *move + 1;
        }
    }
    
    
    else if (movement == -1) {
        if (*move != 0) {
            *move = *move - 1;
        }
    }
}
        
void laser(int map[SIZE][SIZE], int playerX, int *condition) {
    int i = SIZE - 1;
    int stonesDestroyed = 0;
    
    while (i >= 0 && stonesDestroyed < 3) {
        if (map[i][playerX] == STONE || map[i][playerX] == MARCHING) { 
            map[i][playerX] = EMPTY;
            stonesDestroyed++;
        }
        else if (map[i][playerX] >= 3 && map[i][playerX] <= 9) {
            int counterX = 14;   
            
            while (counterX >= 0) {
                int counterY = 0;
                
                while (counterY < SIZE) {
                    if (counterX == i && counterY == playerX) {
                        counterY++;
                    }
                    else {
                        double distance = sqrt((counterX - i) * (counterX - i) + (counterY - playerX) * (counterY - playerX));
                        double bomb = map[i][playerX];
                        if (distance < bomb) {
                            map[counterX][counterY] = EMPTY;
                        }
                        counterY++;
                    }
                }
                counterX--;
            }
            map[i][playerX] = EMPTY;
            stonesDestroyed = 3;
        } 
        i--;
    }
    
    printMap(map, playerX);
    
    i = 0;
    int stones = 0;
    while (i < SIZE) {
        int j = 0;
        
        while (j < SIZE) {
            if (map[i][j] == STONE || map[i][j] == MARCHING) {
                stones++;
            }
            j++;
        }
        i++;
    }
    
    if (stones == 0) {
        printf("Game Won!\n");
        *condition = 1;
    }
            
    
}

void shift (int map[SIZE][SIZE], int playerX, int *condition, int *shifts, int *left, int negMap[150][SIZE], int *negRow) {
    int i = 0;
    int stones = 0;
    
    while (i < SIZE) {
        if (map[SIZE - 1][i] == STONE) {
            stones++;
        }
        i++;
    }
    
    if (stones > 0) {
        printMap(map, playerX);
        printf("Game Lost!\n");
        *condition = 1;
    }
    
    else {
        i = 13;
        while (i >= 0) {
            int j = 0;

            while (j < SIZE) {
                if (*shifts % 2 == 1) {
                    if (map[i][j] == MARCHING || map[i + 1][j] == MARCHING) {
                    }
                    else {
                        map[i + 1][j] = map[i][j];
                        map[i][j] = EMPTY;
                    }
                }
                else {
                    map[i + 1][j] = map[i][j];
                }
                j++;
            }
            i--;
        }
        
        if (*shifts % 2 == 1) {
            if (*left == 0) {
                int counterX = 0;
                
                while (counterX < SIZE) {
                    int counterY = 14;
                    
                    while (counterY >= 0) {
                        if (map[counterX][counterY] == MARCHING) {
                            if (counterY != (SIZE - 1)) {
                                map[counterX][counterY + 1] = map[counterX][counterY];
                                map[counterX][counterY] = EMPTY;
                            }
                        }
                        counterY--;
                    } 
                    counterX++;
                }
                *left = 1;
            }
            
            else if (*left == 1) {
                int counterX = 0;
                
                while (counterX < SIZE) {
                    int counterY = 0;
                    
                    while (counterY < SIZE) {
                        if (map[counterX][counterY] == MARCHING) {
                            if (counterY != 0) {
                                map[counterX][counterY - 1] = map[counterX][counterY];
                                map[counterX][counterY] = EMPTY;
                            }
                        }
                        counterY++;
                    }
                    counterX++;
                }
                *left = 0;
            }
        }
        
        i = 0;
        while (i < SIZE) {
            map[0][i] = negMap[*negRow][i];
            i++;
        }
        *negRow = *negRow + 1;
        *shifts = *shifts + 1;
        printMap(map, playerX);   
    } 
    
}   

void flip(int map[SIZE][SIZE], int *count, int playerX) {
    if (*count == 0) {
        int tempMap[SIZE][SIZE] = {};
        int row1 = 14;
        int row2 = 0;
        
        int i = 0;
        while (i < SIZE) {
            int j = 0;
            
            while (j < SIZE) {
                tempMap[i][j] = map[i][j];
                j++;
            }
            i++;
        }
        
        while (row1 >= 0 && row2 < SIZE) {
            int column = 0;
            
            while (column < SIZE) {
                map[row1][column] = tempMap[row2][column];
                column++;
            }
            row1--;
            row2++;
        }
        *count = *count + 1;            
        printMap(map, playerX);
    }
    else {
        printMap(map, playerX);
    }
    
}

void gravity(int map[SIZE][SIZE], int playerX, int *count) {
    int direction;
    scanf("%d", &direction);
    
    int i = 0;
    
    if (direction == LEFT && *count == 0) {      
        while (i < SIZE) {
            int rocks = 0;
            int j = 0;
            
            while (j < SIZE) {
                if (map[i][j] != EMPTY) {
                    map[i][rocks] = map[i][j];
                    if (rocks != j) {
                        map[i][j] = EMPTY;
                    }
                    rocks++;
                }
                j++;
            }
            i++;
        }       
        *count = 1;
    }
    
    else if (direction == TOP_LEFT && *count == 0) {
        *count = 1;
        while (i < SIZE) {
            int j = 0;
            while (j < SIZE) {
                if (map[i][j] != EMPTY && (i - 1) >= 0 && (j - 1) >= 0) {
                    int counterX = i;
                    int counterY = j;
                    int endLoop = 0;
                    while (endLoop == 0 && map[counterX - 1][counterY - 1] == EMPTY) {
                        map[counterX - 1][counterY - 1] = map[counterX][counterY];
                        map[counterX][counterY] = EMPTY;
                        counterX--;
                        counterY--;
                        
                        if ((counterX - 1) < 0 || (counterY - 1) < 0) {
                            endLoop = 1;
                        }
                    }
                }
                j++;
            }
            i++;
        }
    }
    
    else if (direction == TOP_RIGHT && *count == 0) {            
        *count = 1;
        while (i < SIZE) {
            int j = 14;
            while (j >= 0) {
                if (map[i][j] != EMPTY && (i - 1) >= 0 && (j + 1) < SIZE) {
                    int counterX = i;
                    int counterY = j;
                    int endLoop = 0;
                    while (endLoop == 0 && map[counterX - 1][counterY + 1] == EMPTY) {
                        map[counterX - 1][counterY + 1] = map[counterX][counterY];
                        map[counterX][counterY] = EMPTY;
                        counterX--;
                        counterY++;
                        
                        if ((counterX - 1) < 0 || (counterY + 1) >= SIZE) {
                            endLoop = 1;
                        }
                    }
                }
                j--;
            }
            i++;
        }
    }
    
    else if (direction == RIGHT && *count == 0) {
        while (i < SIZE) {
            int rocks = 14;
            int j = SIZE - 1;
            
            while (j >= 0) {
                if (map[i][j] != EMPTY) {
                    map[i][rocks] = map[i][j];
                    if (rocks != j) {
                        map[i][j] = EMPTY;
                    }
                    rocks--;
                }
                j--;
            }
            i++;
        }
        *count = 1;
    }
    
    else {
        *count = 1;
    }
    printMap(map, playerX);
}
        
