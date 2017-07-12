/*
I have implemented Dijkstra's Algorithm using adjacency matrix representation.
I chose matrix representation because I could figure out a way to backtrack all
the nodes for the shortest path only using matrix representation.
Comments have been used for making sure code is reusable and readable
I learnt the use of base classes, derived classes, constructors, destructors,
virtual functions, and containers.

This algorithm is not ideal because in case of very large number of vertices,
it would take O(n^2) time. I am kneen to know a better way to implement this algorithm.
*/

#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>

using namespace std;

//base class
class Graph
{
	public:
		//graph of 50 nodes where first quantity defines wheher a path exists or not
		//and second one defines its weight if its exists
		pair <bool, int> graph[100][100]; 
		int edge_density,min_dist,max_dist,vertices;
		
		Graph(int edge_density, int min_dist,int max_dist, int vertices): edge_density(edge_density), min_dist(min_dist), max_dist(max_dist), vertices(vertices)
		{
			//initialization
			for(int i=0;i<vertices;i++)
			{
				for(int j=0;j<vertices;j++)
				{
					graph[i][j].first = false;
					graph[i][j].second = 0;
				}
			}
			
			for(int i=0;i<vertices;i++)
			{
				for(int j=0;j<vertices;j++)
				{
					if(i!=j)
					{
						if(graph[j][i].second == RAND_MAX && graph[j][i].first == false)
						{
							graph[i][j].first = false;
							graph[i][j].second = 0;
						}
						else if(graph[j][i].first == true && graph[j][i].second != RAND_MAX)
						{
							graph[i][j].first = true;
							graph[i][j].second = graph[j][i].second;
						}
						else if((rand()%100) < edge_density)
						{
							graph[i][j].first = true;
							graph[i][j].second = (rand()%(max_dist - min_dist)) + min_dist;
						}
						else
						{
							graph[i][j].first = false;
							graph[i][j].second = RAND_MAX;
						}
						
					}
					//loop is not allowed
					else
					{
						graph[i][j].first = false;
						graph[i][j].second = 0;
					}
				}
			}		
		}
		
		virtual void path(int source, int destination) = 0; //pure virtual function for calculating shortest path 
		
		//function for counting number of edges
		inline void getedges()
		{
			int counter =0;
			for(int i=0;i<vertices;i++)
			{
				for(int j=0;j<vertices;j++)
				{
					if(graph[i][j].first == true)
						counter++;
				}
			}
			cout << "Number of edges: " << (counter/2) << endl;
		}
		//function for checking whether a edge exists or not between x and y
		inline void adjacent(int x, int y)
		{
			if(graph[x-1][y-1].first == true)
				cout << "Edge exists" << endl;
			else
				cout << "Edge does not exist" << endl;
		}
		//function for finding all the neighbors of node x
		inline void neighbors(int x)
		{
			cout << "Neighbors are: ";
			for(int i=0;i<vertices;i++)
			{
				if(i==(x-1))
					continue;
				else if(graph[x-1][i].first == true)
					cout << i+1 << " ";
			}
			cout << "\n";
		}
		//function for adding a edge between node x and node y
		inline void add(int x, int y)
		{
			if(graph[x-1][y-1].first == false)
			{
				graph[x-1][y-1].first = true;
				graph[y-1][x-1].first = true;
			}
		}
		//function for deleting a edge between node x and y if the edge exists
		inline void delete_edge(int x, int y)
		{
			if(graph[x-1][y-1].first == true)
			{
				graph[x-1][y-1].first = false;
				graph[y-1][x-1].first = false;
			}
		}
		//function returns weight of the edge between node x and node y
		inline void get_edge_value(int x, int y)
		{
			if(graph[x-1][y-1].first == true)
				cout << "Edge value: " << graph[x-1][y-1].second << endl;
		}
		//function sets the weight of the edge between node x and node y if the edge exists
		inline void set_edge_value(int x, int y, int value)
		{
			if(graph[x-1][y-1].first == true)
				graph[x-1][y-1].second = value;
		}
};
//Derived class
class ShortestPath:public Graph
{
	public:
		ShortestPath(int edge_density, int min_dist, int max_dist, int vertices): Graph(edge_density, min_dist, max_dist, vertices){}
		
		//function for finding the min value in the open set according to the algorithm
		inline pair <int,int> find_min(vector <pair <int, int> > &open_set)
		{
			int index = 0;
			int small = open_set[0].second;
			for(int i=1;i<(int)open_set.size();i++)
			{
				if(open_set[i].second < small)
				{
					index = i;
					small = open_set[i].second;
				}
			}			
			return (make_pair(index, small));
		}
		//finding min value in a vector
		inline int findmin(vector <int> &temp_weight)
		{
			int small = temp_weight[0];
			for(int i=1;i<(int)temp_weight.size();i++)
			{
				if(temp_weight[i] < small)
				{
					small = temp_weight[i];
				}
			}
			return small;
		}
		
		void path (int source, int destination)
		{
			bool exhaust = false;
			bool not_in_closed_set;
			bool not_in_open_set;
			
			vector <pair <int, int> > closed_set; //store node index and weight
			vector <pair <int, int> > open_set;	  //store node index and weight
			
			closed_set.push_back(make_pair(source-1,0)); //place the source in closed set
			//place all the edges of source in the open set
			for(int i=0; i<vertices; i++)
			{
				if(i==source-1)
					continue;
				else if(graph[source-1][i].first == true)
					open_set.push_back(make_pair(i,graph[source-1][i].second));
			}
			//run the loop till open set is empty or closed set contains the destination node
			while(closed_set[closed_set.size() -1].first != (destination-1))
			{		
				if(open_set.size() == 0)
				{
					exhaust = true;
					break;
				}
				
				pair <int,int> min = find_min(open_set); //index of the vector where min node was located and weight of node with min weight
				
				int node_to_move = open_set[min.first].first; //actual index of the node
								
				open_set.erase(open_set.begin() + min.first); // on finding the min weight node, delete it in the open set and place in closed set
				closed_set.push_back(make_pair(node_to_move, min.second));
				
				//find all the edges of the node just placed in the closed set and place it in the open set				
				for(int i=0;i<vertices;i++)
				{
					not_in_closed_set = true;
					not_in_open_set = true;
					
					if(node_to_move == i)
						continue;
					
					else if(graph[node_to_move][i].first == true) //if edge already exists in closed set, break
					{
						for(int j=0;j<(int)closed_set.size();j++)
							if(closed_set[j].first == i)
							{
								not_in_closed_set = false;
								break;
							}
							
						if(not_in_closed_set) //checks whether if it is already present in open set and updates it if we found a lesser path node
						{
							for(int j=0;j<(int)open_set.size();j++)
								if(open_set[j].first == i)
								{
									if((open_set[j].second) > ((graph[node_to_move][i].second)+(min.second)))
										open_set[j].second = ((graph[node_to_move][i].second)+(min.second));
									
									not_in_open_set = false;
									break;
								}
						}
						//if not in closed set and not in open set, place the edge in open set
						if(not_in_open_set && not_in_closed_set)
						{
							open_set.push_back(make_pair(i,((graph[node_to_move][i].second)+(min.second))));
						}				
					}				
				}	
			}
				
			if(exhaust)
				cout << "No path exists" << endl;
			else
			{
				cout << "Path distance: " << closed_set[closed_set.size() -1].second << " ";
				//backtrack all the nodes to find the path
				vector <int> result;
				vector <int> temp_index;
				vector <int> temp_weight;
				
				result.push_back(closed_set[closed_set.size() -1].first); // place the destination in result vector
				//iterate untill result vector contains source node
				while(result[result.size()-1]!=(source-1))
				{
					//puts all the indexes of the nodes leading to 'the most recently placed node in result vector' in temp_index vector
					for(int i=0;i<vertices;i++)
					{
						if(graph[i][result[result.size()-1]].first == true)
							temp_index.push_back(i);
					}
					//puts all the weight of the nodes according to its weights in the closed set in the temp_weight vector
					for(int i=0;i<(int)temp_index.size();i++)
					{
						for(int j=0;j<(int)closed_set.size();j++)
						{
							if(temp_index[i] == closed_set[j].first)
							{
								temp_weight.push_back(closed_set[j].second);
								break;
							}
						}
					}
					//finds the node with min weight
					int minimum = findmin(temp_weight);
					//matches the min weight node with the index of that node which is our backtracked node
					for(int j=0;j<(int)closed_set.size();j++)
					{
						if(minimum == closed_set[j].second)
						{
							result.push_back(closed_set[j].first);
							break;
						}
					}
					//clear the vectors for new iteration
					temp_index.clear();
					temp_weight.clear();
				}
				//print it back to front
				cout << ":Path: ";
				for(int i=((int)result.size() -1); i>=0; i--)
					cout << "Node " << result[i]+1 << "- ";
				
				cout << "\n";
			}
		}
};

int main()
{	
	int edge_density,min_dist,max_dist,source,vertices;
	cout << "Enter the number of vertices: " << endl;
	cin >> vertices;
	cout << "Enter edge density in %: " << endl;
	cin >> edge_density;
	cout << "Enter min and max distance range separated by space: " << endl;
	cin >> min_dist >> max_dist;
	cout << "Enter the source node (from node 1 to node " << vertices << "): " <<endl;
	cin >> source;
	
	ShortestPath mygraph(edge_density,min_dist,max_dist,vertices); //initializing graph
	cout << "Shortest dist from node " << source << ": " <<endl;
	
	for(int i=0;i<vertices;i++)
	{
		cout << "Node " << source << " to " << (i+1) << " is: ";
		mygraph.path(source,i);
	}
	
	return 0;
}

/*
****************Output 1**********************:
Enter the number of vertices:
50
Enter edge density in %:
20
Enter min and max distance range separated by space:
1 10
Enter the source node (from node 1 to node 50):
1
Shortest dist from node 1:
Node 1 to 1 is: No path exists
Node 1 to 2 is: Path distance: 0 :Path: Node 1-
Node 1 to 3 is: Path distance: 6 :Path: Node 1- Node 6- Node 2-
Node 1 to 4 is: Path distance: 5 :Path: Node 1- Node 10- Node 3-
Node 1 to 5 is: Path distance: 5 :Path: Node 1- Node 35- Node 4-
Node 1 to 6 is: Path distance: 6 :Path: Node 1- Node 10- Node 5-
Node 1 to 7 is: Path distance: 4 :Path: Node 1- Node 6-
Node 1 to 8 is: Path distance: 4 :Path: Node 1- Node 10- Node 7-
Node 1 to 9 is: Path distance: 6 :Path: Node 1- Node 6- Node 8-
Node 1 to 10 is: Path distance: 4 :Path: Node 1- Node 10- Node 9-
Node 1 to 11 is: Path distance: 1 :Path: Node 1- Node 10-
Node 1 to 12 is: Path distance: 9 :Path: Node 1- Node 6- Node 11-
Node 1 to 13 is: Path distance: 6 :Path: Node 1- Node 6- Node 12-
Node 1 to 14 is: Path distance: 5 :Path: Node 1- Node 13-
Node 1 to 15 is: Path distance: 7 :Path: Node 1- Node 35- Node 14-
Node 1 to 16 is: Path distance: 4 :Path: Node 1- Node 35- Node 15-
Node 1 to 17 is: Path distance: 6 :Path: Node 1- Node 10- Node 16-
Node 1 to 18 is: Path distance: 7 :Path: Node 1- Node 10- Node 17-
Node 1 to 19 is: Path distance: 2 :Path: Node 1- Node 10- Node 18-
Node 1 to 20 is: Path distance: 7 :Path: Node 1- Node 19-
Node 1 to 21 is: Path distance: 10 :Path: Node 1- Node 6- Node 20-
Node 1 to 22 is: Path distance: 4 :Path: Node 1- Node 35- Node 21-
Node 1 to 23 is: Path distance: 3 :Path: Node 1- Node 10- Node 22-
Node 1 to 24 is: Path distance: 3 :Path: Node 1- Node 35- Node 23-
Node 1 to 25 is: Path distance: 5 :Path: Node 1- Node 10- Node 24-
Node 1 to 26 is: Path distance: 7 :Path: Node 1- Node 35- Node 25-
Node 1 to 27 is: Path distance: 6 :Path: Node 1- Node 35- Node 26-
Node 1 to 28 is: Path distance: 7 :Path: Node 1- Node 6- Node 27-
Node 1 to 29 is: Path distance: 7 :Path: Node 1- Node 10- Node 28-
Node 1 to 30 is: Path distance: 7 :Path: Node 1- Node 35- Node 29-
Node 1 to 31 is: Path distance: 7 :Path: Node 1- Node 10- Node 22- Node 30-
Node 1 to 32 is: Path distance: 5 :Path: Node 1- Node 6- Node 31-
Node 1 to 33 is: Path distance: 4 :Path: Node 1- Node 10- Node 32-
Node 1 to 34 is: Path distance: 4 :Path: Node 1- Node 10- Node 33-
Node 1 to 35 is: Path distance: 4 :Path: Node 1- Node 10- Node 34-
Node 1 to 36 is: Path distance: 2 :Path: Node 1- Node 35-
Node 1 to 37 is: Path distance: 6 :Path: Node 1- Node 35- Node 36-
Node 1 to 38 is: Path distance: 8 :Path: Node 1- Node 10- Node 22- Node 37-
Node 1 to 39 is: Path distance: 1 :Path: Node 1- Node 38-
Node 1 to 40 is: Path distance: 7 :Path: Node 1- Node 6- Node 39-
Node 1 to 41 is: Path distance: 7 :Path: Node 1- Node 35- Node 40-
Node 1 to 42 is: Path distance: 5 :Path: Node 1- Node 10- Node 41-
Node 1 to 43 is: Path distance: 6 :Path: Node 1- Node 35- Node 42-
Node 1 to 44 is: Path distance: 5 :Path: Node 1- Node 10- Node 43-
Node 1 to 45 is: Path distance: 2 :Path: Node 1- Node 10- Node 44-
Node 1 to 46 is: Path distance: 9 :Path: Node 1- Node 10- Node 22- Node 45-
Node 1 to 47 is: Path distance: 7 :Path: Node 1- Node 10- Node 46-
Node 1 to 48 is: Path distance: 4 :Path: Node 1- Node 35- Node 47-
Node 1 to 49 is: Path distance: 5 :Path: Node 1- Node 35- Node 48-
Node 1 to 50 is: Path distance: 5 :Path: Node 1- Node 35- Node 49-


****************Output 2**********************
Enter the number of vertices:
50
Enter edge density in %:
40
Enter min and max distance range separated by space:
1 10
Enter the source node (from node 1 to node 50):
1
Shortest dist from node 1:
Node 1 to 1 is: No path exists
Node 1 to 2 is: Path distance: 0 :Path: Node 1-
Node 1 to 3 is: Path distance: 1 :Path: Node 1- Node 2-
Node 1 to 4 is: Path distance: 4 :Path: Node 1- Node 2- Node 3-
Node 1 to 5 is: Path distance: 2 :Path: Node 1- Node 4-
Node 1 to 6 is: Path distance: 3 :Path: Node 1- Node 5-
Node 1 to 7 is: Path distance: 3 :Path: Node 1- Node 2- Node 6-
Node 1 to 8 is: Path distance: 3 :Path: Node 1- Node 2- Node 7-
Node 1 to 9 is: Path distance: 1 :Path: Node 1- Node 8-
Node 1 to 10 is: Path distance: 5 :Path: Node 1- Node 2- Node 9-
Node 1 to 11 is: Path distance: 4 :Path: Node 1- Node 2- Node 10-
Node 1 to 12 is: Path distance: 4 :Path: Node 1- Node 11-
Node 1 to 13 is: Path distance: 4 :Path: Node 1- Node 2- Node 12-
Node 1 to 14 is: Path distance: 3 :Path: Node 1- Node 13-
Node 1 to 15 is: Path distance: 3 :Path: Node 1- Node 2- Node 14-
Node 1 to 16 is: Path distance: 2 :Path: Node 1- Node 15-
Node 1 to 17 is: Path distance: 3 :Path: Node 1- Node 2- Node 16-
Node 1 to 18 is: Path distance: 3 :Path: Node 1- Node 2- Node 17-
Node 1 to 19 is: Path distance: 3 :Path: Node 1- Node 2- Node 18-
Node 1 to 20 is: Path distance: 2 :Path: Node 1- Node 2- Node 19-
Node 1 to 21 is: Path distance: 3 :Path: Node 1- Node 20-
Node 1 to 22 is: Path distance: 2 :Path: Node 1- Node 2- Node 21-
Node 1 to 23 is: Path distance: 2 :Path: Node 1- Node 2- Node 22-
Node 1 to 24 is: Path distance: 4 :Path: Node 1- Node 2- Node 23-
Node 1 to 25 is: Path distance: 3 :Path: Node 1- Node 2- Node 24-
Node 1 to 26 is: Path distance: 4 :Path: Node 1- Node 25-
Node 1 to 27 is: Path distance: 2 :Path: Node 1- Node 26-
Node 1 to 28 is: Path distance: 2 :Path: Node 1- Node 27-
Node 1 to 29 is: Path distance: 2 :Path: Node 1- Node 28-
Node 1 to 30 is: Path distance: 3 :Path: Node 1- Node 4- Node 29-
Node 1 to 31 is: Path distance: 4 :Path: Node 1- Node 2- Node 30-
Node 1 to 32 is: Path distance: 1 :Path: Node 1- Node 31-
Node 1 to 33 is: Path distance: 2 :Path: Node 1- Node 32-
Node 1 to 34 is: Path distance: 1 :Path: Node 1- Node 33-
Node 1 to 35 is: Path distance: 3 :Path: Node 1- Node 2- Node 34-
Node 1 to 36 is: Path distance: 2 :Path: Node 1- Node 2- Node 35-
Node 1 to 37 is: Path distance: 2 :Path: Node 1- Node 2- Node 36-
Node 1 to 38 is: Path distance: 2 :Path: Node 1- Node 2- Node 37-
Node 1 to 39 is: Path distance: 5 :Path: Node 1- Node 2- Node 38-
Node 1 to 40 is: Path distance: 2 :Path: Node 1- Node 39-
Node 1 to 41 is: Path distance: 3 :Path: Node 1- Node 40-
Node 1 to 42 is: Path distance: 5 :Path: Node 1- Node 41-
Node 1 to 43 is: Path distance: 3 :Path: Node 1- Node 2- Node 42-
Node 1 to 44 is: Path distance: 1 :Path: Node 1- Node 43-
Node 1 to 45 is: Path distance: 2 :Path: Node 1- Node 2- Node 44-
Node 1 to 46 is: Path distance: 4 :Path: Node 1- Node 2- Node 45-
Node 1 to 47 is: Path distance: 2 :Path: Node 1- Node 46-
Node 1 to 48 is: Path distance: 3 :Path: Node 1- Node 47-
Node 1 to 49 is: Path distance: 4 :Path: Node 1- Node 48-
Node 1 to 50 is: Path distance: 3 :Path: Node 1- Node 49-
*/