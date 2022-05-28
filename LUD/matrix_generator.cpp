#include<bits/stdc++.h>
using namespace std;

int main(int argc,char** argv){
	int n=stoi(argv[1]);
	vector<vector<int>> mat(n,vector<int>(n));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			mat[i][j]=rand()%n;
		}
	}
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			cout<<mat[i][j]<<" ";
		}
		cout<<"\n";
	}
	return 0;
}

