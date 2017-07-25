//implemented MST using adjacency matrix representation using Jarnik-Prim MST.

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <iterator>

using namespace std;

//base class
class Graph
{
	public:
		int vertices;
		//graph of nodes where first quantity defines wheher a path exists or not
		//and second one defines its weight if its exists
		pair <bool, int> graph[100][100];
		
		//reading the graph
		Graph()
		{
			ifstream data_file("data.txt");
			istream_iterator <int> start(data_file),end;
			vector <int> numbers(start,end);
			vertices = numbers[0];
			
			//initialization
			for(int i=0;i<vertices;i++)
			{
				for(int j=0;j<vertices;j++)
				{
					graph[i][j].first = false;
					graph[i][j].second = 0;
				}
			}
			
			for(int i=1; i<(int)numbers.size(); i+=3)
			{
				graph[numbers[i]][numbers[i+1]].first = true;
				graph[numbers[i]][numbers[i+1]].second = numbers[i+2];
			}
			
			numbers.resize(0);
		}
		
		virtual void start() = 0;
		
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

//Derived class for calculating the minimum spanning tree using Jarnik -Prim algorithm
class MST:public Graph
{
	public:
		MST(): Graph(){}
		
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
		
		/*void print(vector<pair <int,int> > v)
		{
		  for(int i = 0; i < (int)v.size(); i++) std::cout << v[i].first << " " << v[i].second << "\n";
		  std::cout << std::endl;
		}*/
		
		void start ()
		{
			bool exhaust = false;
			bool not_in_closed_set;
			bool not_in_open_set;
			int sum =0;
			vector <int> result_path;
			
			vector <pair <int, int> > closed_set; //store node index and weight
			vector <pair <int, int> > open_set;	  //store node index and weight
			closed_set.push_back(make_pair(0,0)); //place the source in closed set
			result_path.push_back(0);
			
			//place all the edges of source in the open set
			for(int i=1; i<vertices; i++)
			{
				if(graph[0][i].first == true)
					open_set.push_back(make_pair(i,graph[0][i].second));
			}
			//run the loop till open set is empty or closed set size = total number of vertices
			while(((int)closed_set.size()) != vertices)
			{		
				/*cout << "Open set:\n";
				print(open_set);
				cout << "Closed set:\n";
				print(closed_set);*/
				if(open_set.size() == 0)
				{
					exhaust = true;
					break;
				}
				//index of the vector where min node was located and weight of node with min weight
				pair <int,int> min = find_min(open_set);
				
				int node_to_move = open_set[min.first].first; //actual index of the node
				//cout << sum << endl;
				sum+=open_set[min.first].second; //accumulating weight 
				result_path.push_back(node_to_move);
				
				// on finding the min weight node, delete it in the open set and place in closed set
				open_set.erase(open_set.begin() + min.first);
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
						//checks whether if it is already present in open set and updates it if we found a lesser path node	
						if(not_in_closed_set)
						{
							for(int j=0;j<(int)open_set.size();j++)
								if(open_set[j].first == i)
								{
									if((open_set[j].second) > ((graph[node_to_move][i].second)))
										open_set[j].second = ((graph[node_to_move][i].second));
									
									not_in_open_set = false;
									break;
								}
						}
						//if not in closed set and not in open set, place the edge in open set
						if(not_in_open_set && not_in_closed_set)
						{
							open_set.push_back(make_pair(i,((graph[node_to_move][i].second))));
						}				
					}				
				}	
			}
				
			if(exhaust && ((int)closed_set.size()!=vertices))
				cout << "Tree is unconnected/incomplete" << endl;
			else
			{
				cout << "MST cost: " << sum << "\n";
				//Printing all the nodes leading to the path
				for(int i=0; i<(int)result_path.size(); i++)
					cout << result_path[i] << " ";
				cout << "\n";
			}
		}
};

int main()
{	
	cout << "Reading the graph: \n";
	MST mygraph; //initializing and reading graph
	mygraph.start();
	return 0;
}
