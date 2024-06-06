import numpy as np
import pandas as pd
import math
import mykmeanssp as f
import sys


def reader2(filename):
    x = pd.read_csv(filename,header=None)
    return x



# messures the euclidian distance between two points
def euclidian_dist(p1, p2):
    sum = 0
    for i in range(len(p1)):
        newp1 = float(p1[i])
        newp2 = float(p2[i])
        sum += (newp1 - newp2) ** 2
    return math.sqrt(sum)


def find_closest(p1, k_arr, j,DF):
    return euclidian_dist(p1, DF.iloc[k_arr[j]].values)



def kmeanspp2(DF,k):
    j = 0
    k_arr = [0 for i in range(int(k))]
    np.random.seed(0)
    index_arr = [i for i in range(len(DF))]
    k_arr[j] = int(np.random.choice(index_arr))
    shortest = np.zeros(len(DF))
    for i in range(len(DF)):
        shortest[i] = euclidian_dist(DF[i],DF[k_arr[0]])
    prob_list = [0 for i in range(len(DF))]
    while j< k-1:
        for i in range(len(DF)):
            #temp_dist = find_closest(x.iloc[i].values, k_arr, j,DF)
            temp_dist = np.linalg.norm(DF[i] - DF[k_arr[j]])
            if shortest[i] > temp_dist:
                shortest[i] = temp_dist
        sum_dists = shortest.sum()
        prob_list = np.divide(shortest,sum_dists)
        k_arr[j+1] = int(np.random.choice(index_arr, p=prob_list))
        j+=1
    return k_arr










def is_k_valid(k, points_list):
    return ((k % 1 == 0) and (k > 1) and (k < len(points_list)))

def is_iter_valid(iter):
    return (iter%1==0) and (iter>1) and (iter<1000)

def to_Points(index_arr,points_list):
    toRet = [[0 for i in range(len(points_list[0]))] for i in range(len(index_arr))]
    for j in range(len(index_arr)):
        d=0
        for t in range(len(points_list[index_arr[j]])):
            d+=1
            toRet[j][t] = points_list[index_arr[j]][t]
    return toRet,d



if len(sys.argv)>6 or len(sys.argv) < 5:
    print("An Error Has Occurred")
elif not sys.argv[1].isnumeric():
    print("Invalid number of clusters!")
elif len(sys.argv) == 6 and not sys.argv[2].isnumeric():
    print("Invalid maximum iteration!")
else:
    k = float(sys.argv[1])
    if len(sys.argv) == 6:
        iter = float(sys.argv[2])
        filename1 = sys.argv[4]
        filename2 = sys.argv[5]
        eps = float(sys.argv[3])
    else:
        iter = 300
        eps = float(sys.argv[2])
        filename1 = sys.argv[3]
        filename2 = sys.argv[4]
    points_list1 = reader2(filename1)
    points_list2 = reader2(filename2)
    DF = pd.merge(points_list1, points_list2, how="inner",on=0).sort_values(by=0, ascending=True).drop(0,axis='columns').values
    points_list = [DF[i].tolist() for i in range(len(DF))]

    if is_k_valid(k,points_list) and is_iter_valid(iter):
        pre_k_list = kmeanspp2(DF, k)
        y = 0
        for j in pre_k_list:
            if y == len(pre_k_list) - 1:
                print(j, end="")
            else:
                print(f"{j},", end="")
            y += 1
        print("")


        
        for i in range(len(points_list)):
            for j in range(len(points_list[i])):
                points_list[i][j] = float(points_list[i][j])
        iter = int(iter)
        k = int(k)
        k_list,d = to_Points(pre_k_list,points_list)
        array_means = f.fit(k_list,points_list,k,iter,d,len(points_list),eps)
        for t in range(len(array_means)):
            y = 0
            for j in array_means[t]:
                if y == len(array_means[t]) - 1:
                    print('%.4f' % j, end="")
                else:
                    print('%.4f' % j + ",", end="")
                y += 1
            print()
    else:
        if not  is_k_valid(k,points_list):
            print("Invalid number of clusters!")
        if not is_iter_valid(iter):
            print("Invalid maximum iteration!")
