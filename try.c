/*
gcc -ansi -Wall -Wextra -Werror -pedantic-errors try.c -o try -lm
./try 3 100 < input_1.txt
*/

# include <stdio.h>
# include <stdlib.h>
# include <math.h>


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
int hasNext(struct vector *v){
    if (v->next == NULL){
        return 0;
    }
    return 1;
}
double dist(struct vector *p1, struct vector *p2, int dim){
    struct cord *t1 = p1->cords;  
    struct cord *t2 = p2->cords;
    double distance = 0.0;
    int i;
    for(i = 0; i < dim; i++){
        distance += pow(t1->value - t2->value, 2);
        t1 = t1->next;
        t2 = t2->next;
    }
    return sqrt(distance);
}



int main(int argc, char **argv) {
    
    int dimension= 0;
    int clustersNum = 0;
    int itersNum = DEFAULT_ITERS_NUM;
    struct vector *centroidsArr;
    struct vector *newCentroidsArr;
    struct vector *tempVec, *tempVec2;
    struct cord *tempCord, *tempCord2;
    int *countArr;
    

    struct vector *head_vec, *curr_vec /**next_vec*/;
    struct cord *head_cord, *curr_cord /**next_cord*/;
    int i, j, k;
    double n;
    char c;
    int numOfPoints, numOfCords;
    int closestCentIndex = 0;
    double minDist;
    double curDist;

    (void)argc;

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;
    
    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;

    


   
     /* gets the number of clusters and checks if it is valid*/
    clustersNum = atoi(argv[1]);
    itersNum = atoi(argv[2]); 

    if(itersNum <= 1 || itersNum >= MAX_ITERS_NUM)
    {
        printf("Invalid maximum iteration!");
        return -1;
    }
    
    while (scanf("%lf%c", &n, &c) == 2)
    {
        numOfCords++;
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
            numOfPoints++;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        
    }

    curr_vec = head_vec;
    curr_cord = curr_vec->cords;

    dimension = numOfCords / numOfPoints;

    centroidsArr = malloc(sizeof(struct vector) * clustersNum);
    newCentroidsArr = malloc(sizeof(struct vector) * clustersNum);

    /*initialize centroidsArr*/
    for(i = 0; i < clustersNum; i++){
        centroidsArr[i] = *curr_vec;
        curr_vec = curr_vec->next;
    }

    /*initialize newCentroidsArr*/
    tempCord = malloc(sizeof(struct cord));
    tempVec =  malloc(sizeof(struct vector));
    curr_cord = tempCord;
    curr_vec = tempVec;
    for(i = 0; i < clustersNum; i++){
        curr_vec->cords = curr_cord;
        for(j = 0; j < dimension; j++){
            curr_cord->value = 0;
            curr_cord->next = malloc(sizeof(struct cord));
            curr_cord = curr_cord->next;
            curr_cord->next = NULL;
        }
        newCentroidsArr[i] = *curr_vec;
        curr_vec->next = malloc(sizeof(struct vector));
        curr_vec = curr_vec->next;
    }

    /*initialize countArr*/
    countArr = malloc(sizeof(int) * clustersNum);

    /*main algorithm*/
    for(i = 0; i < 8; i++){
        /*reset newCentroidsArr to all zeros*/
        for(j = 0; j < clustersNum; j++){
            curr_vec = &newCentroidsArr[j];
            curr_cord = curr_vec->cords;
            for(k = 0; k < dimension; k++){
                curr_cord->value = 0.0;
                curr_cord = curr_cord->next;
            }
        }

        

        /*reset countArr to zeros*/
        for(j = 0; j < clustersNum; j++){
            countArr[j] = 0;
        }
        curr_vec = head_vec;
        k = 0; 
        /*loop through the points and assign each one to closest centroid*/
        while(hasNext(curr_vec) == 1)
        {
            curr_cord = curr_vec->cords;
            closestCentIndex = 0;
            minDist = dist(curr_vec, &centroidsArr[0], dimension); 
            for(j = 1; j < clustersNum; j++)
            {
                curDist = dist(curr_vec, &centroidsArr[j], dimension);
                if(curDist < minDist)
                {
                    minDist = curDist;
                    closestCentIndex = j;
                }
            }
            
            countArr[closestCentIndex]++;
            tempVec = &newCentroidsArr[closestCentIndex];
            tempCord = tempVec->cords;
            for(j = 0; j < dimension; j++)
            {
                tempCord->value += curr_cord->value;
                tempCord = tempCord->next;
                curr_cord = curr_cord->next;
            }

            curr_vec = curr_vec->next;
            k++;
            
        }
        /*divide the sum of the values of newCentroidsArr by the number of points assigned to the centroid*/
        for(j = 0; j < clustersNum; j++)
        {
            tempVec = &newCentroidsArr[j];
            tempCord = tempVec->cords;
            for(k = 0; k < dimension; k++)
            {
                tempCord->value /= countArr[j];
                tempCord = tempCord->next;
            }
        }
        
        for (j = 0; j < clustersNum; j++){
            tempVec = &newCentroidsArr[j];
            tempCord = tempVec->cords;
            tempVec2 = &centroidsArr[j];
            tempCord2 = tempVec2->cords; 
            for (k = 0; k < dimension; k++){
                tempCord2->value = tempCord->value;
                tempCord2 = tempCord2->next;
                tempCord = tempCord->next;
            }
        }
        

        
    }
    printf("%d\n", countArr[0]);
    printf("%d\n", countArr[1]);
    printf("%d\n", countArr[2]);
    printf("%f\n", centroidsArr[0].cords->value);
    printf("%f\n", centroidsArr[0].cords->next->value);
    printf("%f\n", centroidsArr[1].cords->value);
    printf("%f\n", centroidsArr[1].cords->next->value);
    printf("%f\n", centroidsArr[2].cords->value);
    printf("%f\n", centroidsArr[2].cords->next->value);
    
    
    j = 0;
    curr_vec = head_vec;
    curr_cord = curr_vec->cords;
    

    return 0;

    

    printf("%f", centroidsArr[0].cords->value);
    printf("%f", curr_cord->value);
    




    /*checks if the number of clusters is valid*/
    if(clustersNum <= 1 || clustersNum >= 100)
    {
        printf("Invalid number of clusters!");
        return -1;
    }

    
}