# define PY_SSIZE_T_CLEAN
# include <Python.h>
# include <stdio.h>
# include <stdlib.h>
#include <math.h>


//double** fit_c(double** K_list, double **points_list,int k,int iter,int d,int Pnum);
int epsilonCheck(double ** oldArr, double ** newArr, int d, int k, double eps);
double **kMeans(double  **pointsArr, double **KmeansArr , int d, int k, int Pnum);
void sumTwoArr(double * arr1, double * arr2, int d);
void findAvg(int *counter, double ** sumArr, int k, int d);
double euclidian_dist(double * arr1, double * arr2, int d);
void deallocate2DArray(double** arr, int rows);
double **indexToPoints(int *index_arr,double **points_list,int k,int d);


double** fit_c (double** K_list, double **points_list,int k,int iter,int d,int Pnum,double eps) {


        int i;
        int t;
        double **arr;
        double **oldArr;
        double **newArr;
        
        arr = points_list;
        oldArr = K_list;

        if (k < Pnum) {
            newArr = kMeans(arr, oldArr, d, k, Pnum);
            
            t = 0;
            while (t < iter && (epsilonCheck(oldArr, newArr, d, k, eps) == 1)) {
                for (i = 0; i < k; i++) {
                    free(oldArr[i]);
                }
                free(oldArr);
                oldArr = newArr;
                newArr = kMeans(arr, oldArr, d, k, Pnum);
                t++;
                
            }

            for (i = 0; i < Pnum; i++) {
                free(arr[i]);
            }
            free(arr);

            for (i = 0; i < k; i++) {
                free(oldArr[i]);
            }
            free(oldArr);

            return newArr;
        }
        else {
            printf("Invalid number of clusters!");
        }
        return NULL;
}
    


    void sumTwoArr(double *arr1, double *arr2, int d) {
        int i;
        for (i = 0; i < d; i++) {
            arr1[i] += arr2[i];
        }

    }

    double **kMeans(double **pointsArr, double **KmeansArr, int d, int k, int Pnum) {
        int *counter;
        double **sumArr;
        double temp;
        double minDist;
        int minIndex;
        int i;
        int j;
        int t;


        sumArr = (double **) calloc(k, sizeof(double *));
        if (sumArr == NULL) {
            printf("An Error Has Occurred");
            return 0;
        }
        for (t = 0; t < k; t++) {
            sumArr[t] = (double *) calloc(d, sizeof(double));
            if (sumArr[t] == NULL) {
                printf("An Error Has Occurred");
                return 0;
            }
        }

        counter = (int *) calloc(k, sizeof(int));
        if (counter == NULL) {
            printf("An Error Has Occurred");
            return 0;
        }

        for (i = 0; i < Pnum; i++) {

            minDist = euclidian_dist(pointsArr[i], KmeansArr[0], d);
            minIndex = 0;
            /*go over all the means and find the closest one*/
            for (j = 1; j < k; j++) {

                temp = euclidian_dist(pointsArr[i], KmeansArr[j], d);
                if (temp < minDist) {
                    minIndex = j;
                    minDist = temp;
                }
            }
            /*add the point to the sum of the closest mean*/
            sumTwoArr(sumArr[minIndex], pointsArr[i], d);
            /*add 1 to the counter of this mean*/
            counter[minIndex] += 1;


        }
        findAvg(counter, sumArr, k, d);
        free(counter);
        return sumArr;
    }

    double euclidian_dist(double *arr1, double *arr2, int d) {
        double sum = 0;
        int i;
        for (i = 0; i < d; i++) {
            sum += pow((arr1[i] - arr2[i]), 2);
        }

        return sqrt(sum);
    }

    void findAvg(int *counter, double **sumArr, int k, int d) {
        int i;
        int j;
        for (i = 0; i < k; i++) {
            if (counter[i] == 0) {
                continue;
            }
            for (j = 0; j < d; j++) {
                sumArr[i][j] = sumArr[i][j] / (double)counter[i];
            }
        }
    }

    int epsilonCheck(double **oldArr, double **newArr, int d, int k, double eps) {
        int i;
        int r;
        for (r = 0; r < k; r++) {
            for (i = 0; i < d; i++) {
                if ((oldArr[r][i] - newArr[r][i]) >= eps || (oldArr[r][i] - newArr[r][i]) <= -eps) {
                    return 1;
                }
            }
        }
        return 0;
    }


    void deallocate2DArray(double **arr, int rows) {
        int i;
        if (arr == NULL) {
            return;
            for (i = 0; i < rows; i++) {
                free(arr[i]);
            }
            free(arr);
        }
    }


static PyObject* fit(PyObject *self, PyObject *args)
{
    PyObject* k_list1;
    PyObject* points_list1;
    int k;
    int iter;
    int d;
    int Pnum;
    double eps;
    double **points_list;
    int t;
    int i;
    double **answer_c;
    double **k_list;
    PyObject* vector1;
    PyObject* cord1;
    

    /* This parses the Python arguments into a double (d)  variable named z and int (i) variable named n*/
    if(!PyArg_ParseTuple(args, "OOiiiif", &k_list1, &points_list1,&k,&iter,&d,&Pnum,&eps)) {
        return NULL; /* In the CPython API, a NULL value is never valid for a
                        PyObject* so it is used to signal that an error has occurred. */
    }

    points_list = (double **) calloc(Pnum, sizeof(double * ));
    k_list = (double **) calloc(k, sizeof(double * ));


    for(t=0; t<Pnum; t++) {
        points_list[t]= (double *) calloc(d, sizeof(double ));
        if (points_list[t]==NULL){
            printf("An Error Has Occurred");
            return 0;
        }
    }
    for(t=0; t<k; t++) {
        k_list[t] = (double *) calloc(d, sizeof(double ));
    }

    for( i = 0;i<Pnum;i++){
        for(t = 0;t<d;t++){
            points_list[i][t] = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(points_list1, i), t));
        }
    }

     for( i = 0;i<k;i++){
        for(t = 0;t<d;t++){
            k_list[i][t] = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(k_list1, i), t));
        }
    }

    answer_c = fit_c(k_list,points_list,k,iter,d,Pnum,eps);

    
    PyObject* answer_py = PyList_New(k);

    for( i = 0;i<k;i++){
        vector1 = PyList_New(d);
        for(t = 0;t<d;t++){
            cord1  = Py_BuildValue("f",answer_c[i][t]);
            PyList_SetItem(vector1,t,cord1);
        }
        PyList_SetItem(answer_py,i,vector1);
    }

    deallocate2DArray(answer_c,k);
    return answer_py;
}


static PyMethodDef capiMethods[] = {
        {
            "fit",
         (PyCFunction) fit,
        METH_VARARGS,
        PyDoc_STR("k_list,points_list,k,iter,d,len(points_list),eps")},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp",
        NULL,
        -1,
        capiMethods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}








