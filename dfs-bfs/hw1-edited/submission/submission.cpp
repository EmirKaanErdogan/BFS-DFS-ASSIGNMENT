

/*

    BLG336E Analysis of Algorithms
    Assigment -1- 
    <Emir Kaan Erdoğan>
    <150200706>  

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
#include <unordered_set>
#include <time.h>
using namespace std; 

class Kid{
    private:
        int x_position; 
        int y_position; 
        int power; 
    public:
        Kid(); // default constructor 
        Kid(int x_position,int y_position,int power); // constructor with parameters; 
        Kid(Kid*);// copy constructor 
        void print_object()const; 
        // Getters ; 
        int get_xPosition()const{
            return this->x_position; 
        }
        int get_yPosition()const{
            return this->y_position;
        }
        int get_power()const {
            return this->power; 
        }
        // Setters 
        void set_xPosition(int x_position){
            this->x_position=x_position; 
        }
        void set_yPosition(int y_position){
            this->y_position=y_position;
        }
        void set_power(int power){
            this->power=power; 
        }
};
Kid::Kid(){
    cout<<"Welcome to Default Constructor ! "<<endl ; 
}
Kid::Kid(int x_positon,int y_position,int power){
    this->x_position=x_positon;
    this->y_position=y_position;
    this->power=power; 
}
Kid::Kid(Kid* copy_kid){
    this->x_position=copy_kid->x_position;
    this->y_position=copy_kid->y_position;
    this->power=copy_kid->power; 
}
void Kid::print_object()const{
    cout<<"X pos: "<<this->x_position<<", Y pos: "<<this->y_position<<" , Power: "<<this->power<<endl; 
}

int write_graph(const vector <vector<int> >&adj_matrix){
    ofstream outfile("graph.txt");
    if(!outfile.is_open()){
        cout<<"File cannot be opened !"<<endl ;
        return EXIT_FAILURE; 
    }
    outfile<< "Graph:"<<endl; 
    cout<<"Adjacency Matrix"<<endl ; 
    for(size_t i=0;i<adj_matrix.size();i++){
        for(size_t j=0;j<adj_matrix[0].size();j++){
            outfile<<adj_matrix[i][j]<<" ";
            cout<<adj_matrix[i][j]<<" "; 
        }
        outfile<<endl ;
        cout<<endl; 
    }
    outfile.close();
    return EXIT_SUCCESS;
}
int write_bfs(vector<int>&path,int source){
    ofstream outfile("bfs.txt");
    if(!outfile.is_open()){
        cout<<"File cannot be opened !"<<endl ;
        return EXIT_FAILURE; 
    }
    outfile<<"BFS: "<<endl ;
    outfile<<path.size()<<" ";
    outfile<<source;
    for(vector<int>::reverse_iterator i=path.rbegin(); i!=path.rend(); ++i){
        outfile<<"->"<<*i;
    }
    outfile.close();

    return EXIT_SUCCESS;
}
int write_cycle(vector<int>&path,int source,bool is_cyclic){
    ofstream outfile("dfs.txt");  

    if(!outfile.is_open()){
        cout<<"File cannot be opened !"<<endl ;
        return EXIT_FAILURE;
    }
    outfile<<"DFS:"<<endl ;
    if(!is_cyclic){
        outfile<<"-1"; 
        outfile.close();
        return EXIT_SUCCESS;
    }
    outfile<<path.size()<<" ";
    for(size_t i=0;i<path.size();i++){
        outfile<<path[i]<<"->";
    }
    outfile<<source;
    outfile.close();
    return EXIT_SUCCESS;
}
double get_distance(int x1,int y1,int x2,int y2){
    return (pow(double(x1-x2),2)+pow(double(y1-y2),2)); 
}
bool is_reachable(const Kid& kid1, const Kid& kid2){
    double distance = get_distance(kid1.get_xPosition(), kid1.get_yPosition(),
                                    kid2.get_xPosition(), kid2.get_yPosition());
    if(kid1.get_power() >= distance && kid2.get_power() >= distance){
        return true; 
    }
    return false; 
}

vector<vector<int> > create_grid(vector<Kid*> kids){
    vector <vector<int> > my_grid(kids.size(),vector<int>(kids.size()));
    //cout<<my_grid.size()<<","<<my_grid[0].size()<<endl ; 
    //print_out_grid(my_grid);
    for(size_t i=0;i<my_grid.size();i++){
        for(size_t j=0;j<my_grid[0].size();j++){
            if(i!=j){
                if(is_reachable(kids[i],kids[j])){
                    my_grid[i][j]=1;
                    my_grid[j][i]=1; 
                }
            }
        }
    }
    write_graph(my_grid);

    return my_grid; 
}

vector<Kid*> read_file(string file_name,vector<vector<int> >&my_grid,int* source_kid,int* target_kid){
    vector <Kid*> kids ; 
    //int num_kids; 
    int num; 
    int row_counter=0 ;
    ifstream input_file(file_name);
    string line; 
    vector <vector<int> > row; 
    bool first_line=true; 
    bool first_col=true; 
    if(input_file.is_open()){
        while(getline(input_file,line)){
            stringstream ss(line); // create a stringstream from the line ; 
            string column; 
            int col_counter=0 ;
            while(ss>>num){
                //cout<<num<<" ";
                if(first_line){
                    if(first_col){
                        row.resize(num+1,vector<int>(3));
                        first_col=false; 
                    }  
                    row[0][col_counter]=num;
                }
                else{
                    row[row_counter][col_counter]=num; 
                }
                col_counter++; 
            }
            first_line=false;
            if(row_counter>0){
                kids.push_back(new Kid(row[row_counter][0],row[row_counter][1],row[row_counter][2]));
            }
            row_counter++;
            //cout<<endl; 
        }
    }
    else{
        cout<<"The file cannot be found"<<endl ;
        
    }
    input_file.close(); 
    /*
    for(int i=0;i<kids.size();i++){
        kids[i]->print_object();
    }
    */
    my_grid =create_grid(kids); 
    *source_kid=row[0][1];
    cout<<"Source Kid:"<<row[0][1]<<",Target Kid:"<<row[0][2]<<endl ;
    *target_kid= row[0][2];
     
    return kids; 
    
}
void print_parent_path(vector <int> parent_path,int src,int target){
    vector <int> path; 
    int current= target; 
    while(current!=src){
        path.push_back(current);
        current=parent_path[current] ;
    }
    cout<<endl ;
    cout<<path.size()<<" ";
    write_bfs(path,src); 
    cout<<src;
    for(vector<int>::reverse_iterator i=path.rbegin(); i!=path.rend(); ++i){
    cout<<"->"<<*i;
    }
    cout<<endl ;
}
int min_passes(vector<vector<int> > grid, int src, int target) {
    /*
    To find the shortest path using BFS the following steps are implemented.
    1) a queue for kids and an unordered set to prevent ambigous search which is initially set to false 
    and a vector of distances that has the size of the grid and  
    2) while the queue is not empty iterate thorugh the unvisited nodes and mark them as visited
    and update the corresponding distance. The process should continue untill the distance of the target is achieved

    */
    queue<int> search_queue;
    unordered_set<int> visited;
    vector<int> dist(grid.size(), -1);
    vector<int> parent(grid.size(),-1); 
    // enqueue the source and set it as visited
    search_queue.push(src);
    visited.insert(src);
    // distance from source to itself is 0
    dist[src] = 0; 
    while (!search_queue.empty()) {
        // dequeue the node 
        int current_kid = search_queue.front();
        search_queue.pop();
        if (current_kid == target) { 
            print_parent_path(parent,src,target); 
            return dist[target];
        }
        // control the neighbours&& the kids that the current kid can pass to.
        for (size_t i = 0; i < grid[current_kid].size(); i++) {
            // the neighbouring kid should be reachable and has not been visited yet; 
            if (grid[current_kid][i] == 1 && visited.find(i) == visited.end()) {
                // mark the kid as visited and enqueue it
                visited.insert(i);
                search_queue.push(i);
                // update the distance for the neighbour  
                dist[i] = dist[current_kid] + 1; 
                parent[i]=current_kid ;
            }
        }
    }
    return -1; // target is not reachable from source
}

bool dfs(int current, const int source,const vector<vector<int> >& grid, unordered_set<int>& visited, vector<int>& cycle_path){
    cycle_path.push_back(current); 
    visited.insert(current);

    if(grid[current][source]==1 && cycle_path.size()>2){
        return true ; 
    }
    for(size_t i=0;i<grid.size();i++){
        if(grid[current][i]==1){
            if(grid[current][i]==source && cycle_path.size()>2){
                return true; 
            }
            if(visited.find(i)==visited.end()){
                if(dfs(i,source,grid,visited,cycle_path)){
                    return true; 
                }
            }
        }
    }
    cycle_path.pop_back();
    return false; 
}

void print_cycle(const vector<vector<int> >& grid, const int source) {
    vector<int> cycle_path; 
    unordered_set<int> new_visited; 
    if(dfs(source,source,grid,new_visited,cycle_path)){
        write_cycle(cycle_path,source,true);
        cout<<"Cycle Found ! "<<endl ;
        cout<<cycle_path.size()<<" ";
        for(size_t i=0;i<cycle_path.size();i++){
            cout<<cycle_path[i]<<"->";
        }
        cout<<source;
        cout<<endl; 
    }
    else{
        write_cycle(cycle_path,source,false);
        cout<<"-1"<<endl ; 
        cout<<"No cycle Found ! "<<endl ; 
    }
}

int main(int argc,char** argv){

    /*
        INSERT ALL YOUR CODE IN HERE!
        Program Compile Command: g++ -std=c++11 -Wall -Werror main.cpp -o main
        Program Run Command: ./main <input.txt>
        Expected input: /graphs/case{$n}/input_{$n}.txt
        Expected output: graph.txt bfs.txt dfs.txt
        Please, try to write clean and readable code. Remember to comment!!
    */
    clock_t start,end ;
    string file_name=argv[1];
    vector<vector<int> > my_grid; // adjacency matrix; 
    int* skidp=new int[1]; 
    int* tkidp=new int[1];
    vector <Kid*> my_vector= read_file(file_name,my_grid,skidp,tkidp);
    int source_kid= *skidp; 
    int target_kid = *tkidp ;
    //cout<<"source kid: "<<source_kid<<", target kid: "<<target_kid<<endl;
    // deallocate memory and avoid dangling pointer ;
    delete [] skidp;
    skidp=nullptr; 
    delete [] tkidp;
    tkidp=nullptr; 
    my_vector[source_kid]->print_object();
    my_vector[target_kid]->print_object(); 
    start=clock();
    int minimum_passes=min_passes(my_grid,source_kid,target_kid);
    cout<<"The minimum Passes: "<<minimum_passes<<endl;
    end=clock();
    double elapsed1= (double)(end-start)/CLOCKS_PER_SEC ;
    cout<<"BFS Run Time: "<<elapsed1<<" ms"<<endl; 
    start=clock();
    print_cycle(my_grid,source_kid);
    end=clock();
    double elapsed2= (double)(end-start)/CLOCKS_PER_SEC ;
    cout<<"DFS Run Time: "<<elapsed2<<" ms"<<endl;
    return 0 ; 
}