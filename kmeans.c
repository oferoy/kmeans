/*
gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans -lm
./kmeans 15 100 < input_3.txt
valgrind --leak-check=full --track-origins=yes ./kmeans 15 < input_3.txt
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

double dist(struct vector *p1, double *centroid, int dim){
    struct cord *t1 = p1->cords;  
    double distance = 0.0;
    int i;
    for(i = 0; i < dim; i++){
        distance += pow(t1->value - centroid[i], 2);
        t1 = t1->next;
    }
    return sqrt(distance);
}
double distC(double *centroid1, double *centroid2, int dim){
    double distance = 0.0;
    int i;
    for(i = 0; i < dim; i++){
        distance += pow(centroid1[i] - centroid2[i], 2);
    }
    return sqrt(distance);
}

int isInteger(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    if (*str == '-') {
        str++;
    }
    while (*str != '\0') {
        if (*str < '0' || *str > '9') {
            return 0;
        }
        str++;
    }
    return 1;
}

void free_cords(struct cord *head) {
    struct cord *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void free_vectors(struct vector *head) {
    struct vector *temp;
    while (head != NULL && head->next != NULL) {
        temp = head;
        head = head->next;
        if(temp->cords != NULL){
            free_cords(temp->cords);
        }
        free(temp);
    }
    free(head);
}

int main(int argc, char **argv) {
    
    int dimension= 0;
    int clustersNum = 0;
    int itersNum = DEFAULT_ITERS_NUM;
    double **centroidsArr;
    double **newCentroidsArr;
    int *countArr;


    struct vector *head_vec, *curr_vec;
    struct cord *head_cord, *curr_cord;
    int i, j, k;
    double n;
    char c;
    int numOfPoints = 0, numOfCords = 0;
    int closestCentIndex = 0;
    double minDist = 0;
    double curDist = 0;
    int smallerThanEps = 0;
   
    /* gets the number of clusters and checks if it is valid*/
    if(argc == 3)
    {
        if(isInteger(argv[2])){
            itersNum = atoi(argv[2]); 
        }
        else{
        printf("Invalid maximum iteration!");
        return 1;
        }
    }
    else if (argc == 2)
    {
        itersNum = DEFAULT_ITERS_NUM;
    }
    else
    {
        printf("An Error Has Occurred");
        return 1;
    }
    if(isInteger(argv[1])){
        clustersNum = atoi(argv[1]);
    }
    else
    {
        printf("Invalid number of clusters!");
        return 1;
    }
    
    head_cord = malloc(sizeof(struct cord));
    if(head_cord == NULL){
        printf("An Error Has Occurred");
        return 1;
    }
    curr_cord = head_cord;
    curr_cord->next = NULL;
    
    head_vec = malloc(sizeof(struct vector));
    if(head_cord == NULL){
        free(head_cord);
        printf("An Error Has Occurred");
        return 1;
    }
    curr_vec = head_vec;
    curr_vec->next = NULL;

    

    if(itersNum <= 1 || itersNum >= MAX_ITERS_NUM)
    {
        printf("Invalid maximum iteration!");
        free(head_vec);
        free(head_cord);
        return 1;
    }
    
    while (scanf("%lf%c", &n, &c) == 2)
    {
        numOfCords++;
        if (c == '\n')
        {
            
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            if(curr_vec->next == NULL){
                free(head_cord);
                free_vectors(head_vec);
                printf("An Error Has Occurred");
                return 1;
            }
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            if(head_cord == NULL){
                free_vectors(head_vec);
                printf("An Error Has Occurred");
                return 1;
            }
            curr_cord = head_cord;
            curr_cord->next = NULL;
            numOfPoints++;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        if(curr_cord->next == NULL){
            free(head_cord);
            free_vectors(head_vec);
            printf("An Error Has Occurred");
            return 1;
        }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        
        
    }

    if(clustersNum<=1 || clustersNum >= numOfPoints)
    {
        
        printf("Invalid number of clusters!");
        free(head_cord);
        free_vectors(head_vec);
        return 1;
    }
    
    curr_vec = head_vec;
    

    dimension = numOfCords / numOfPoints;

    centroidsArr = (double **)malloc(clustersNum * sizeof(double *));
    if(centroidsArr == NULL){
        free(head_cord);
        free_vectors(head_vec);
        printf("An Error Has Occurred");
        return 1;
    }
    newCentroidsArr = (double **)malloc(clustersNum * sizeof(double *));
    if(newCentroidsArr == NULL){
        free(centroidsArr);
        free(head_cord);
        free_vectors(head_vec);
        printf("An Error Has Occurred");
        return 1;
    }
    for (i = 0; i < clustersNum; i++) {
        centroidsArr[i] = (double *)malloc(dimension * sizeof(double));
        if(centroidsArr[i] == NULL){
            for (j = 0; j < i; j++) {
                free(centroidsArr[j]);
                free(newCentroidsArr[j]);
            }
            free(newCentroidsArr);
            free(centroidsArr);
            free(head_cord);
            free_vectors(head_vec);
            printf("An Error Has Occurred");
        return 1;
        }
        newCentroidsArr[i] = (double *)malloc(dimension * sizeof(double));
        if(newCentroidsArr[i] == NULL){
            for (j = 0; j < i; j++) {
                free(centroidsArr[j]);
                free(newCentroidsArr[j]);
            }
            free(centroidsArr[j]);
            free(newCentroidsArr);
            free(centroidsArr);
            free(head_cord);
            free_vectors(head_vec);
            printf("An Error Has Occurred");
        return 1;
        }
    }

    /*initialize centroidsArr*/
    for(i = 0; i < clustersNum; i++){
        curr_cord = curr_vec->cords;
        for(j = 0; j < dimension; j++){
            centroidsArr[i][j] = curr_cord->value;
            curr_cord = curr_cord->next;
        }
        curr_vec = curr_vec->next;
    }
    
    /*initialize newCentroidsArr*/
    for(i = 0; i < clustersNum; i++){
        for(j = 0; j < dimension; j++){
            newCentroidsArr[i][j] = 0.0;
        }
    }
           
   
    /*initialize countArr*/
    countArr = malloc(sizeof(int) * clustersNum);
    if(countArr == NULL){
        for (i = 0; i < clustersNum; i++) {
        free(centroidsArr[i]);
        free(newCentroidsArr[i]);
        }
        free(centroidsArr);
        free(newCentroidsArr);
        free_vectors(head_vec);
        free(head_cord);
    }

    /*main algorithm*/
    for(i = 0; i < itersNum; i++){
        /*reset newCentroidsArr to all zeros*/
        for(j = 0; j < clustersNum; j++){
            for(k = 0; k < dimension; k++){
                newCentroidsArr[j][k] = 0.0;
            }
        }

        /*reset countArr to zeros*/
        for(j = 0; j < clustersNum; j++){
            countArr[j] = 0;
        }
        curr_vec = head_vec;
        /*loop through the points and assign each one to closest centroid*/

        while(curr_vec->next != NULL)
        {
            curr_cord = curr_vec->cords;
            closestCentIndex = 0;
            minDist = dist(curr_vec, centroidsArr[0], dimension); 
            for(j = 1; j < clustersNum; j++)
            {
                curDist = dist(curr_vec, centroidsArr[j], dimension);
                if(curDist < minDist)
                {
                    minDist = curDist;
                    closestCentIndex = j;
                }
            }
            
            countArr[closestCentIndex]++;
            curr_cord = curr_vec->cords;
            for(j = 0; j < dimension; j++)
            {
                newCentroidsArr[closestCentIndex][j] += curr_cord->value;
                curr_cord = curr_cord->next;
            }

            curr_vec = curr_vec->next;
        }
    

        /*divide the sum of the values of newCentroidsArr by the number of points assigned to the centroid*/
        for(j = 0; j < clustersNum; j++)
        {
            for(k = 0; k < dimension; k++)
            {
                newCentroidsArr[j][k] /= countArr[j];
            }
        }
        

        smallerThanEps = 1;
        for(j = 0; j < clustersNum; j++)
        {
            if(fabs(distC(newCentroidsArr[j], centroidsArr[j], dimension)) >= EPSILON)
            {
                smallerThanEps =0;
            }
        }
        if(smallerThanEps == 1)
        {
            break;
        }



        for (j = 0; j < clustersNum; j++){
            for (k = 0; k < dimension; k++){
                centroidsArr[j][k] = newCentroidsArr[j][k];
            }
        }
        
    }

    for(i = 0; i < clustersNum; i++){
        for(j = 0; j < dimension; j++){
            printf("%.4f", centroidsArr[i][j]);
            if(j != dimension - 1){
                printf(",");
            }
            else
            {
                printf("\n");   
            }
        }
    }
    
   
    for (i = 0; i < clustersNum; i++) {
        free(centroidsArr[i]);
        free(newCentroidsArr[i]);
    }
    free(centroidsArr);
    free(newCentroidsArr);
    free(countArr);
    free_vectors(head_vec);
    free(head_cord);
    
    return 0;
}