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

/*
Output of the program is:
Avg path length with edge density 20% is: 4.5
Avg path length with edge density 40% is: 4
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
		pair <bool, int> graph[50][50]; 
		int edge_density,min_dist,max_dist;
		
		Graph(int edge_density, int min_dist,int max_dist): edge_density(edge_density), min_dist(min_dist), max_dist(max_dist)
		{
			//initialization
			for(int i=0;i<50;i++)
			{
				for(int j=0;j<50;j++)
				{
					graph[i][j].first = false;
					graph[i][j].second = 0;
				}
			}
			
			for(int i=0;i<50;i++)
			{
				for(int j=0;j<50;j++)
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
		
		//function for calculating avg path length
		void avgpathlength()
		{
			float counter = 0;
			float sum = 0;
			
			for(int i=1;i<50;i++)
			{
				if(graph[0][i].first == true)
				{
					sum+=graph[0][i].second;
					counter++;
				}
			}
			cout << "Avg path length with edge density " << edge_density << "% is: " << (sum / counter) << endl;
		}
		//function for counting number of edges
		void getedges()
		{
			int counter =0;
			for(int i=0;i<50;i++)
			{
				for(int j=0;j<50;j++)
				{
					if(graph[i][j].first == true)
						counter++;
				}
			}
			cout << "Number of edges: " << (counter/2) << endl;
		}
		//function for checking whether a edge exists or not between x and y
		void adjacent(int x, int y)
		{
			if(graph[x-1][y-1].first == true)
				cout << "Edge exists" << endl;
			else
				cout << "Edge does not exist" << endl;
		}
		//function for finding all the neighbors of node x
		void neighbors(int x)
		{
			cout << "Neighbors are: ";
			for(int i=0;i<50;i++)
			{
				if(i==(x-1))
					continue;
				else if(graph[x-1][i].first == true)
					cout << i+1 << " ";
			}
			cout << "\n";
		}
		//function for adding a edge between node x and node y
		void add(int x, int y)
		{
			if(graph[x-1][y-1].first == false)
			{
				graph[x-1][y-1].first = true;
				graph[y-1][x-1].first = true;
			}
		}
		//function for deleting a edge between node x and y if the edge exists
		void delete_edge(int x, int y)
		{
			if(graph[x-1][y-1].first == true)
			{
				graph[x-1][y-1].first = false;
				graph[y-1][x-1].first = false;
			}
		}
		//function returns weight of the edge between node x and node y
		void get_edge_value(int x, int y)
		{
			if(graph[x-1][y-1].first == true)
				cout << "Edge value: " << graph[x-1][y-1].second << endl;
		}
		//function sets the weight of the edge between node x and node y if the edge exists
		void set_edge_value(int x, int y, int value)
		{
			if(graph[x-1][y-1].first == true)
				graph[x-1][y-1].second = value;
		}
};
//Derived class
class ShortestPath:public Graph
{
	public:
		ShortestPath(int edge_density, int min_dist, int max_dist): Graph(edge_density, min_dist, max_dist){}
		
		//function for finding the min value in the open set according to the algorithm
		pair <int,int> find_min(vector <pair <int, int> > &open_set)
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
		int findmin(vector <int> &temp_weight)
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
			for(int i=0; i<50; i++)
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
				for(int i=0;i<50;i++)
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
				cout << "Path distance: " << closed_set[closed_set.size() -1].second << endl;
				//backtrack all the nodes to find the path
				vector <int> result;
				vector <int> temp_index;
				vector <int> temp_weight;
				
				result.push_back(closed_set[closed_set.size() -1].first); // place the destination in result vector
				//iterate untill result vector contains source node
				while(result[result.size()-1]!=(source-1))
				{
					//puts all the indexes of the nodes leading to 'the most recently placed node in result vector' in temp_index vector
					for(int i=0;i<50;i++)
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
				for(int i=((int)result.size() -1); i>=0; i--)
					cout << "Node " << result[i]+1 << "-";
				
				cout << "\n";
			}
		}
};

int main()
{	
	int edge_density,min_dist = 1,max_dist = 10;
	
	edge_density = 20;
	ShortestPath mygraph(edge_density,min_dist,max_dist);
	mygraph.avgpathlength();
	
	edge_density = 40;
	ShortestPath mygraph1(edge_density,min_dist,max_dist);
	mygraph1.avgpathlength();
	
	return 0;
}