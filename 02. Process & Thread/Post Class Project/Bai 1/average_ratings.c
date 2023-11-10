#include "average_ratings.h"

void read_file(char* file, rateMatrix* mat_ptr)
{
    FILE *fp = fopen(file, "r");

    int bufferLength = 255;
    char buffer[bufferLength];
    int userID, movieID, rating, timeStamp;

    while (fgets(buffer, bufferLength, fp) != NULL) {
        sscanf(buffer, "%d %d %d %d", &userID, &movieID, &rating, &timeStamp);
        if ((userID -1) < 0 || (userID -1) > ROWS || (movieID -1) < 0 || (movieID -1) > COLUMNS) {
            printf("Invalid user ID or movie ID: %d %d\n", userID, movieID);
        } else {
            mat_ptr->rating[(userID -1)][(movieID -1)] = rating;
            //printf("Data: %d\n",mat_ptr->rating[(userID -1)][(movieID -1)]);
        }
    }

    fclose(fp);
}

int calculate_average_ratings(int movieID, rateMatrix* mat_ptr)
{
     int sum = 0;
     int counter = 0;
     for(int user=0; user < ROWS; user++) {
        // Filter zero value and count the valid one.
        if(mat_ptr->rating[user][movieID] != 0) {
           sum += mat_ptr->rating[user][movieID];
           counter++;
        } 
     }
     printf("Average rating of movie %d: %d\n", movieID, sum/counter); 
}




