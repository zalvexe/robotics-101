#include<bits/stdc++.h>
using namespace std;
#define pb push_back
/* 
PENJELASAN :
1. grid maze disimpan di vector 2D, kenapa vektor? biar lebih mudah saat passing ke function 
karena size n m tidak pasti (tergantung input)
2. dicari point start nya pake fungsi find_point(), disimpan di pair start_index = {i,j}
3. mulai mencari possible movement lewat fungsi find_path() yang akan memanggil fungsi find_path_first() untuk mengecek gerakan yang possible
4. di dalam fungsi rekursi find_path_first : 
    - cek dulu, kalo char di index sekarang merupakan char finish ('$') berarti semua gerakan yang udah dicek tadi dimasukkan ke vector result [res.pb(move);]
    - kalo belum, berarti cek bisa gerak ke kanan/kiri/atas/bawah lewat if
    saat di if, contoh gerak ke bawah (s): 
    if(start_i.first + 1 < n && grid[start_i.first + 1][start_i.second] != obs && visited[start_i.first + 1][start_i.second] != visit)
        - cek, apakah baris ke i+1 masih kurang dari besar row, apakah baris ke i+1 bukan obstacle, apakah baris ke i+1 belum dikunjungi
        kalo semua memenuhi, berarti bisa gerak ke baris i+1 (kebawah) dan gerakan 's' dimasukkan ke string move
        - masuk lagi ke fungsi rekursi untuk mengecek gerakan selanjutnya dengan menggunakan nilai i+=1 dan vektor ke [i][j] sudah dikunjungi
        - setelah masuk ke fungsi rekursi tadi, maka nilai i akan kembali ke nilai awal, dan vektor ke [i][j] menjadi belum dikunjungi untuk digunakan saat pencarian possible move yg lain
5. setelah fungsi find_path_first dijalankan, maka hasil string move akan bisa diakses melalui vector<string> res untuk return nilai res
6. vector<string> res tadi akan diakses di main() untuk dicari string yg punya length terkecil
7. string yg punya length terkecil disimpan di shortestPath dan dimasukkan ke fungsi draw() untuk digambar jalan keluar robot 
dan maze yg sudah digambar disimpan di vektor output untuk diprint

ex input :
/*
4 4
.o.$
o..o
.o..
*...
out :
ddwwwd
*/

vector<vector<char> > output;
pair<int,int> index;
const char robot = '*';
const char obs = 'o';
const char space = '.';
const char finish = '$';
const char visit = '#';

class solve{
public:
    pair<int,int> find_point(vector<vector<char> >& grid){
        pair<int,int>start_index;
        for(int i = 0; i<grid.size(); i++){
            for(int j = 0; j<grid[i].size(); j++){
                if(grid[i][j] == robot){
                    start_index = {i,j};
                }
            }
        }
        return start_index;
    }

void find_path_first(pair<int,int> start_i, vector<vector<char> >& grid, int n, int m, vector<string>& res, string move, vector<vector<char> >& visited){
    if(grid[start_i.first][start_i.second] == finish){
        res.pb(move);
        
    }
    else{
        //go down
        if(start_i.first + 1 < n && grid[start_i.first + 1][start_i.second] != obs && visited[start_i.first + 1][start_i.second] != visit){
            visited[start_i.first][start_i.second] = visit;
            start_i.first += 1;
            find_path_first(start_i, grid, n, m, res, move+'s', visited);
            visited[start_i.first][start_i.second] = space;
            start_i.first -= 1;
        }
        //go up
        if(start_i.first - 1 >= 0 && grid[start_i.first - 1][start_i.second] != obs && visited[start_i.first - 1][start_i.second] != visit){
            visited[start_i.first][start_i.second] = visit;
            start_i.first -= 1;
            find_path_first(start_i, grid, n, m, res, move+'w', visited);
            visited[start_i.first][start_i.second] = space;
            start_i.first += 1;
        }
        //go right
        if(start_i.second + 1 < m && grid[start_i.first][start_i.second + 1] != obs && visited[start_i.first][start_i.second + 1] != visit){
            visited[start_i.first][start_i.second] = visit;
            start_i.second += 1;
            find_path_first(start_i, grid, n, m, res, move+'d', visited);
            visited[start_i.first][start_i.second] = space;
            start_i.second -= 1;
        }
        //go left
        if(start_i.second - 1 >= 0 && grid[start_i.first][start_i.second - 1] != obs && visited[start_i.first][start_i.second - 1] != visit){
            visited[start_i.first][start_i.second] = visit;
            start_i.second -= 1;
            find_path_first(start_i, grid, n, m, res, move+'a', visited);
            visited[start_i.first][start_i.second] = space;
            start_i.second += 1;
        }
    }
}
vector<string> find_path(vector<vector<char> >& grid, int n, int m){
        vector<string> res;
        output = grid;
        vector<vector<char> >visited(n, vector<char>(m, '.'));
        pair<int,int>start_i = find_point(grid);
        find_path_first(start_i, grid, n, m, res,"", visited);
        return res;
    }

void draw(vector<vector<char> >& output, char mv, pair<int,int>& index){
    if(output[index.first][index.second] == finish) output[index.first][index.second] = visit;
    else{
        if(mv == 'a'){
            output[index.first][index.second-1] = visit;
            index.second-=1;
        }
        else if(mv == 'd'){
            output[index.first][index.second+1] = visit;
            index.second+=1;
        }
        else if(mv == 's'){
            output[index.first+1][index.second] = visit;
            index.first+=1;
        }
        else if(mv == 'w'){
            output[index.first-1][index.second] = visit;
            index.first-=1;
        }
    }
}
};
int main(){
    int n,m;
    char c;
    cin >> n >> m;
    vector<vector<char> >grid;
    output = grid;
    for(int i = 0; i<n; i++){
        vector<char>p;
        for(int j = 0; j<m; j++){
            cin >> c;
            p.pb(c);
        }
        grid.pb(p);
    }

    solve solv;
    vector<string>key = solv.find_path(grid, n,m);
    pair<int,int>index = solv.find_point(grid);
    int len = sizeof(key)/sizeof(key[0]);
    string shortestPath;
    if(key.size() == 0) cout << "NO SOLUTION";
    else{
        cout << "All Possible Solutionssss : ";
        shortestPath = key[0];
        for(int i = 0; i<key.size(); i++){
            cout << key[i] << " ";
            if(shortestPath.length()>key[i].length()){
                shortestPath = key[i];
            }
        }
        cout << "\nShortest Path : " << shortestPath << '\n';
         for(int i = 0; i<shortestPath.length(); i++){
            solv.draw(output,shortestPath[i], index);
    }
        for(int i = 0; i<output.size(); i++){
            for(int j = 0; j<output[i].size(); j++){
                cout << output[i][j];
            }
            cout << endl;
        }
    }
   
}
