//Made by zalfa - December 2023
#include<bits/stdc++.h>
using namespace std;

/* 
A = [i][j-1]; kiri 
S = [i+1][j]; mundur 
D = [i][j+1]; kanan 
W = [i-1][j]; maju 
*/

int n,m;
int main(){
	cin >> n >> m; //reading 2 input value for array/maze size
	char grid[n][m]; //maze
	int rx,ry,fx,fy;

	for(int i = 0; i<n; i++){
		for(int j = 0; j<m; j++){
			cin >> grid[i][j];
			if(grid[i][j]=='*'){
				rx = i; //robot x position
				ry = j; //robot y position
			}
			else if(grid[i][j] == '$'){
				fx = i; //finish x position
				fy = j; // finish y position
			}
		}
	}

	int strx = rx, stry = ry, frx = fx, fry = fy; //saving the original position of start and finish

	int q;
	cin >> q;
	char que[q]; //to store movements from input
	for(int i = 0; i<q; i++){
		cin >> que[i];
	}

/* v check every input v*/

	for(int i = 0; i<q; i++){
		if(que[i] == 'a'){ //kiri
			ry--; //move to column-1
			if(grid[rx][ry] == 'o') {
			cout << i << " "<< "Robot Menabrak Obstacle" << endl; //index, based on Testcase starts from 0, so 'i'
			ry++; //robot position stays in that position, doesn't count as a movement
			}
			else if(rx > n-1 || rx < 0 || ry < 0 || ry > m-1){
				cout << i << " " << "Robot Menabrak Tembok" << endl;
				ry++;
			}
			else{
				grid[rx][ry] = '#'; //if the position is safe, then it changed to '#' as valid position
				continue;
			}
		}
		else if(que[i] == 's'){//mundur
			rx++; //move to row+1
			if(grid[rx][ry] == 'o') {
			cout << i << " "<< "Robot Menabrak Obstacle" << endl;
			rx--; 
			}
			else if(rx > n-1 || rx < 0 || ry < 0 || ry > m-1){
				cout << i << " " << "Robot Menabrak Tembok" << endl;
				rx--;
			}
			else{
				grid[rx][ry] = '#'; 
				continue;
			} 

		}
		else if(que[i] == 'd'){ //kanan
			ry++; //move to column + 1
			if(grid[rx][ry] == 'o') {
				cout << i << " "<< "Robot Menabrak Obstacle" << endl;
				ry--;
				}
			else if(rx > n-1 || rx < 0 || ry < 0 || ry > m-1){
				cout << i << " " << "Robot Menabrak Tembok" << endl;
				ry--;
			}
			else{
				grid[rx][ry] = '#'; 
				continue;
			} 
		}
		else if(que[i] == 'w'){
			rx--; //move to row-1
			if(grid[rx][ry] == 'o') {
			cout << i << " "<< "Robot Menabrak Obstacle" << endl;
			rx++;
			}
			else if(rx > n-1 || rx < 0 || ry < 0 || ry > m-1){
				cout << i << " " << "Robot Menabrak Tembok" << endl;
				rx++;
			}
			
			else{
				grid[rx][ry] = '#'; 
				continue;
			} 
		}
		
	}

	if(rx == fx && ry == fy){ //if the final position of robot is the same as finish position
		grid[frx][fry] = '*'; //changing the char in finish index
		grid[strx][stry] = '#'; //changing the char in start index
		for(int i = 0; i<n; i++){
			for(int j = 0; j<m; j++){
				cout << grid[i][j];
				}
			cout << endl;
			}
	
		cout << "Robot Mencapai Finish" << '\n'; //robot's position is the same as finish's position
	}
	else if (rx != fx || ry != fy) cout << "Robot Tidak Mencapai Finish" << '\n';
}

/*notes about my code : could be more efficient if i use function for checking the 
position after each movement, but i don't know how to assign 2D array
in function, tried to seek some help from google about it but still didn't understand */
