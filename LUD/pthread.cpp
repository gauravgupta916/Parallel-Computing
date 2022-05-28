#include <bits/stdc++.h>
#include<fstream>
#include<chrono>
#include<sys/time.h>
#include<pthread.h>
using namespace std;
using std::ifstream;
using namespace std::chrono;

int n;
int thread_count;
int tot_thread;
vector<int> p;
ifstream indata;
vector<vector<int>> a;
vector<vector<int>> u;
vector<vector<int>> l;
vector<vector<int>> permut;

pthread_mutex_t lock_;

void assign_values(){
    p.assign(n,0);
    a.assign(n,vector<int>(n));
    u.assign(n,vector<int>(n,0));
    l.assign(n,vector<int>(n,0));
    permut.assign(n,vector<int>(n,0));
}

void display(vector<vector<int>>& x){
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
    //int n = a.size();
    vector<vector<int>> rslt(n,vector<int>(n));

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            rslt[i][j] = 0;
            for (int k = 0; k < n; k++){
                rslt[i][j] += mat1[i][k] * mat2[k][j];
            }
            cout<<rslt[i][j]<<" ";
        }
        cout<<endl;
    }
}
*/

void* getPermutation(void *args){
    int avg_rows = ceil(1.0*n/tot_thread);
    
    pthread_mutex_lock(&lock_); //entry point of critical section
    // start of critical section
    int thread_number = thread_count;
    thread_count++;
    // end of critical section
    pthread_mutex_unlock(&lock_);

    int r = avg_rows*thread_number;
    int limit = min(n,r+avg_rows);
    for(int i=r;i<limit;i++) {
        for(int j=0;j<n;j++){
            if(j==p[i])
                permut[i][j]=1;
            else
                permut[i][j]=0;
        }
    }
    pthread_exit(NULL);
}


void* compute_k(void *args){

    int k = *((int *)args);
    int len = ceil((1.0*n-(k+1))/tot_thread);
    int start,end;
    
    pthread_mutex_lock(&lock_); //entry point of critical section
    // start of critical section
    thread_count++;
    int thread_number = thread_count;
    if(thread_count==1){
    	start = (k)*thread_count + 1;
    }else{
    	start = (thread_count-1)*len+1;
    }
    end = thread_count*len;
    // end of critical section
    pthread_mutex_unlock(&lock_);
    
    
    end = min(end,n);
    for(int i=start; i<end; i++){ 
          for(int j=k+1; j<n; j++){
               a[i][j] -= l[i][k]*u[k][j];
          }
    }
    pthread_exit(NULL);
}

void* compute_single_k(void *args){
    int k = *((int *)args);
    int len = ceil((1.0*n-(k+1))/tot_thread);
    int start,end;
    
    pthread_mutex_lock(&lock_); //entry of critical section
    // start of critical section
    thread_count++;
    int thread_number = thread_count;
    if(thread_count==1){
		start = (k)*thread_count + 1;
    }else{
		start = (thread_count-1)*len+1;
    }
    end = thread_count*len;
    // end of critical section
    pthread_mutex_unlock(&lock_);
    
    end = min(end,n);
    for(int i=start; i<end; i++){ 
          l[i][k] = a[i][k]/u[k][k];
          u[k][i] = a[k][i];
    }
    pthread_exit(NULL);
}

void *swap_k(void *args){
	int k = *((int *)args);
    int k1 = *((int *)args+1);
        
	int avg_rows = ceil(1.0*k/tot_thread);
	//int thread_number;
	pthread_mutex_lock(&lock_); //entry of critical section
	// start of critical section
	int thread_number = thread_count;
	thread_count++;
	// end of critical section
	pthread_mutex_unlock(&lock_);

	int r = avg_rows*thread_number;
	int limit = min(k,r+avg_rows);
    for(int i=r;i<limit;i++) {
        swap(l[k][i], l[k1][i]);
    }
    pthread_exit(NULL);
}

void *assigner_3(void *args){
	int thread_number;
	pthread_mutex_lock(&lock_);
	thread_number = thread_count;
	thread_count++;
	pthread_mutex_unlock(&lock_);
	
	if(thread_number == 0){
	   // a[][] initialisation
	    int num;
	    for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				indata>>num;
				a[i][j]=num;
			}
	    }
	}   
	    
	else if(thread_number == 1){
	    //l & u initialisation
	    for(int i=0; i<n; i++) {
			for(int j=0; j<n; j++) {
				if(i==j) {
				    l[i][j] = 1;
				}
			}
	    }
	}   
	 
	else if(thread_number == 1){
	  //p[] initialisation
	    for(int i=0;i<n;i++){
	    	p[i]=i;  
	    }
	}
	
	pthread_exit(NULL);
}

void *assigner_2(void *args){
	int thread_number;
	pthread_mutex_lock(&lock_);
	thread_number = thread_count;
	thread_count++;
	pthread_mutex_unlock(&lock_);
	
	if(thread_number == 0){
	   // a[][] initialisation & l[][] inititialisation
	    int num;
	    for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				indata>>num;
				a[i][j]=num;
				if(i==j) {
				    l[i][j] = 1;
				}
			}
	    }
	}      
	 
	else if(thread_number == 1){
	  //p[] initialisation
	    for(int i=0;i<n;i++){
	    	p[i]=i;  
	    }
	}
	
	pthread_exit(NULL);
}

int main(int argc,char** argv){
    lock_ = PTHREAD_MUTEX_INITIALIZER;
    n=stoi(argv[1]); // size of matrix
    string in=argv[2]; // name of text file from which input matrix is fetched
    indata.open(in);
    tot_thread = stoi(argv[3]); //number of threads
    assign_values();
    
    // inception point of timer
    auto start = high_resolution_clock::now();
    
    //total threads is equal to 1
    if(tot_thread == 1){
    	int num;
    	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			indata>>num;
			a[i][j]=num;
		}
    	}
    
    	//l & u initialisation
    	for(int i=0; i<n; i++) {
        	for(int j=0; j<n; j++) {
            		if(i==j) {
               	 	l[i][j] = 1;
            		}
        	}
    	}
    
    	//p initialisation
    	vector<int> p(n,0);
    	for(int i=0;i<n;i++){  
    		p[i]=i;  
    	}
    }else if(tot_thread == 2){ //total threads is equal to 2
    	thread_count = 0;
    	pthread_t thread[2];
    	for(int i=0;i<2;i++){
    		pthread_create(&thread[i],NULL,assigner_2,(void*)&n);
    	}
    	for(int i=0; i<2; i++){
        	pthread_join(thread[i], NULL);
    	}
    }else{ //total threads is greater than 2
    	thread_count = 0;
    	pthread_t thread[3];
    	for(int i=0;i<3;i++){
    		pthread_create(&thread[i],NULL,assigner_3,(void*)&n);
    	}
    	for(int i=0; i<3; i++){
        	pthread_join(thread[i], NULL);
    	}
    }
    
    
    for(int k=0; k<n; k++){
        int maxEle=0;
        int k1;
        for(int i=k; i<n; i++){
            if(maxEle < abs(a[i][k])) {
                maxEle = abs(a[i][k]);
                k1 = i;
            }
        }
        if(maxEle==0) {
            cout<<"Singular Matrix";
            exit(0); //error
        }
        
        swap(p[k], p[k1]);
        a[k].swap(a[k1]);
        
        
        for(int i=0; i<k; i++) {
            swap(l[k][i], l[k1][i]);
        }
        
        
        /*
        thread_count = 0;
        pthread_t thread_arr3[tot_thread];
        for(int i=0; i<tot_thread; i++){
           int save[2];
           save[0] = k;
           save[1] = k1;
           pthread_create(&thread_arr3[i],NULL,swap_k,(void*)save);
        }
        for(int i=0; i<tot_thread; i++){
        	pthread_join(thread_arr3[i], NULL);
        }
        */

        u[k][k] = a[k][k];

        // fillLU 
        /*
        for(int i=k+1; i<n; i++) {
            l[i][k] = a[i][k]/u[k][k]; //col wise
            u[k][i] = a[k][i]; //row wise
        }
        */
        
        thread_count = 0;
        pthread_t thread_arr1[tot_thread];
        for(int i=0; i<tot_thread; i++){
           pthread_create(&thread_arr1[i],NULL,compute_single_k,(void*)&k);
        }
        for(int i=0; i<tot_thread; i++){
        	pthread_join(thread_arr1[i], NULL);
        }
        
        /*
        for(int i=k+1; i<n; i++) {
           for(int j=k+1; j<n; j++) {
               a[i][j] -= l[i][k]*u[k][j];
            }
        }
        */
        
        thread_count = 0;
        pthread_t thread_arr[tot_thread];
        for(int i=0; i<tot_thread; i++){
           pthread_create(&thread_arr[i],NULL,compute_k,(void*)&k);
        }
        for(int i=0; i<tot_thread; i++){
        	pthread_join(thread_arr[i], NULL);
        }
        
    }

    // Permutation matrix calculation
    thread_count = 0;
    pthread_t thread_arr[tot_thread];
    for(int i=0;i<tot_thread;i++){
    	pthread_create(&thread_arr[i],NULL,getPermutation,NULL);
    }
    for(int i=0; i<tot_thread; i++){
        pthread_join(thread_arr[i], NULL);
    }
    
    /*
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++){
            if(j==p[i])
                permut[i][j]=1;
            else
                permut[i][j]=0;
        }
    }
    */
    
    // timer ends here
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout<<duration.count()<<" ";
    
    /*
    cout<<"\nOriginal matrix is :\n";
    display(a);
    cout<<"\nU matrix is :\n";
    display(u);
    cout<<"\nL matrix is :\n";
    display(l);
    cout<<"\nPermutation matrix is :\n";
    display(permut);
    */

    return 0;
}




