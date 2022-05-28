#include <bits/stdc++.h>
#include <omp.h>
#include<fstream>
#include<chrono>
using namespace std;
using std::ifstream;
using namespace std::chrono;

void display(int n,vector<vector<int>>& x){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout<<x[i][j]<<" ";
        }
        cout<<"\n";
    }
    cout<<"\n";
}

/*
void mulMat(vector<vector<int>>& mat1, vector<vector<int>>& mat2) {
    //int rslt[R1][C2];
    int n = mat1.size();
    vector<vector<int>> rslt(n,vector<int>(n));
 
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            rslt[i][j] = 0;
            for (int k = 0; k < n; k++){
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }
            cout << rslt[i][j] << " ";
        }
        cout << endl;
    }
}
*/

int main(int argc,char** argv){
    ifstream indata;
    int n=stoi(argv[1]);
    string in=argv[2];
    //int thread_num=stoi(argv[3]); //Enter thread number
    indata.open(in);
    vector<vector<int>> a(n,vector<int>(n));
    vector<vector<int>> u(n,vector<int>(n,0));
    vector<vector<int>> l(n,vector<int>(n,0));
    vector<vector<int>> permut(n,vector<int>(n,0));
    
    // inception point of timer
    auto start = high_resolution_clock::now(); 
    
    int num;
    for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		indata>>num;
		a[i][j]=num;
	}
    }
    
    //l & u initialisation
    #pragma omp parallel for collapse(2)
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i==j){
                l[i][j] = 1;
            }
        }
    }
    
    //p initialisation
    vector<int> p(n,0);
    #pragma omp parallel for
    for(int i=0;i<n;i++){  
    	p[i]=i;  
    }

    
    for(int k=0; k<n; k++) {
        int maxEle=0;
        int k1;
        
        for(int i=k; i<n; i++) {
            if(maxEle < abs(a[i][k])) {
                maxEle = abs(a[i][k]);
                k1 = i;
            }
        }
        
        if(maxEle==0) {
            cout<<"Singular Matrix\n";
            exit(0); //error
        }
        
        swap(p[k], p[k1]);
        a[k].swap(a[k1]);
        
        #pragma omp parallel for
        for(int i=0; i<k; i++) {
            swap(l[k][i], l[k1][i]);
        }
        u[k][k] = a[k][k];
        
        #pragma omp parallel for
        for(int i=k+1; i<n; i++) {
            l[i][k] = a[i][k]/u[k][k];
            u[k][i] = a[k][i];
        }
        
        #pragma omp parallel for collapse(2) 
        for(int i=k+1; i<n; i++) {
            for(int j=k+1; j<n; j++) {
                a[i][j] -= l[i][k]*u[k][j];
            }
        }
    }
    
    
    // Permutation matrix calculation
    #pragma omp parallel for collapse(2) 
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++){
            if(j==p[i])
                permut[i][j]=1;
            else
                permut[i][j]=0;
        }
    }
    
    // timer ends here
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout<<duration.count()<<"\n";
    
    //cout<<"\nOriginal matrix is :\n";
    //display(n,a);
    //cout<<"\nU matrix is :\n";
    //display(n,u);
    //cout<<"\nL matrix is :\n";
    //display(n,l);
    //cout<<"\nPermutation matrix is :\n";
    //display(n,permut);
    
    
    return 0;
}












