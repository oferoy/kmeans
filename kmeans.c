# include <stdio.h>
# include <stdlib.h>
# include <math.h>
#include <stdin.h>
#include <stdout.h>

#define EPSILON (0.001)
#define DEFAULT_ITERS_NUM (200)
#define MAX_ITERS_NUM (1000)

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};


int readFile(int argc, char **argv) {   
    struct vector *head_vec, *curr_vec, *next_vec;
    struct cord *head_cord, *curr_cord, *next_cord;
    int i, j, rows = 0, cols;
    double n;
    char c;

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;


    while (scanf("%lf%c", &n, &c) == 2)
    {

        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }

    return 0;
}


int main() {
    int clustersNum = getchar();
    int itersNum = DEFAULT_ITERS_NUM;
    int input = 0;
    /* gets the number of clusters and checks if it is valid*/
    while ((input = getchar()) != ' ');
    {
        clustersNum *= 10;
        clustersNum += input;
    }
    if(clustersNum <= 1 || clustersNum >= N)
    {
        printf("Invalid number of clusters!");
        return -1;
    }

    /* gets the number of iterations and checks if it is valid*/
    while ((input = getchar()) != ' ');
    {
        itersNum *= 10;
        itersNum += input;
    }
    if(itersNum <= 1 || itersNum >= MAX_ITERS_NUM)
    {
        printf("Invalid maximum iteration!")
        return -1;
    }













}