#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> //std::random_shuffle
#include <ctime> //std::time
#include <cstdlib> //std::rand, srand
#include <cmath>
#include <cfloat>
#include <iterator>

using namespace std;


//Function Declearation
int randomit (int i){return rand()%i;} //Define the unary function
void getcoordinate(string);
vector<int> permutation(int);
void randomsearch();
void RMHC();
void EA();
int getIndex (vector<double>, double);
int getIndex_int (vector<int>, int);

//Global Variables
vector<float> x_cord;
vector<float> y_cord;
/********************************************/ //add to random search function (Annoying Stuff!!!!!!!!!)
vector <int> id;
vector <int> id_shorest;
double dis;
double total = 0;
double shorest = DBL_MAX;
//double shorest = 0; // Uncomment for the longest

int main()
{
    getcoordinate("tsp.txt");
    /************************************************************************/
    //
    // uncomment the one you want to run and comment the other two out
    //
    //
    /************************************************************************/
    //randomsearch();
    RMHC();
    //EA();
    return 0;
}

void RMHC()
{
    ofstream RMHC;
    ofstream cord_RMHC;
    RMHC.open("distancce_RMHC.csv");
    cord_RMHC.open("coordinate_RMHC.csv");
    vector<vector<int>> paths;
    double shorest;
    for (int i = 0; i<10; i++)
    {
        paths.push_back(permutation(i));
    }
/*********************************************************************/

    //Mutation
    vector<double> total_RMHC;
    for (int generation = 0; generation<5000; generation++)
    {
        double dis_RMHC = 0;
        double tot_dist = 0;
        vector<double> total_RMHC = {};
        for(int j = 0; j<10; j++)
        {
            vector<int> mutation = paths[j]; //get a copy of j_th path in the matrix paths
            int location = rand()%1000; //get a random location on the path
            //cout << location << endl;
            float temp = mutation[location];//Make one small change
            //cout << paths[j][location] << " "<< paths[j][location+1]<<" ";
            if (location == 999)
            {
                mutation[location] = mutation[0];
                mutation[0] = temp;
                paths.push_back(mutation);
            }
            else
            {
                mutation[location] = mutation[location+1];
                mutation[location+1] = temp;
                paths.push_back(mutation);
            }
        }//we have a 20*1000 matrix(paths) by the end of the loop

        for (int k = 0; k<20; k++)
        {
            for (int a = 0; a<1001; a++)
            {
                if (a == 1000)
                {
                    dis_RMHC = sqrt(pow((x_cord[paths[k][0]] - x_cord[paths[k][a]]) , 2.0) + pow((y_cord[paths[k][0]] - y_cord[paths[k][a]]) , 2.0));
                }
                else
                {
                    dis_RMHC = sqrt(pow((x_cord[paths[k][a+1]] - x_cord[paths[k][a]]) , 2.0) + pow((y_cord[paths[k][a+1]] - y_cord[paths[k][a]]) , 2.0));
                }
                tot_dist += dis_RMHC;
            }
            //cout << tot_dist << endl;
            total_RMHC.push_back(tot_dist); //We have 20 distances now
            tot_dist = 0;
        }

        vector <double> kl = total_RMHC;
        sort(total_RMHC.begin(), total_RMHC.end()); //Sort the 20 distances in ascending order
        //sort(total_RMHC.begin(), total_RMHC.end(), greater<int>()); //Uncomment for the longest
        vector<vector<int>> temp;
        for (int id = 0; id<10; id++ ) //Run 10 times to obtain the index of the top 10 paths
        {
            vector<int> top10_id;
            //cout << getIndex(kl, total_RMHC[id])<<endl;
            temp.push_back(paths[getIndex(kl, total_RMHC[id])]);
        }
        id_shorest = paths[0];
        paths.clear();
        paths = temp;
        shorest = total_RMHC[0];
        RMHC << shorest << endl;
        cout << shorest<<endl;
    }
    cord_RMHC << "x_coordinate" << " " << "y_coordinate" << endl;
    //cout << "x_coordinate" << " " << "y_coordinate" << endl;
    for (int k = 0; k < 1001; k++)
    {
        //cout << x_cord[id_shorest[k]] << " " << y_cord[id_shorest[k]] << endl;
        cord_RMHC << x_cord[id_shorest[k]] << " " << y_cord[id_shorest[k]] << endl;
    }
}

void EA()
{
    ofstream EA;
    ofstream cord_EA;
    EA.open("distancce_EA.csv");
    cord_EA.open("coordinate_EA.csv");
    vector<vector<int>> paths;
    double shorest;
    for (int i = 0; i<10; i++)
    {
        paths.push_back(permutation(i));
    }
/*********************************************************************/

    //Mutation
    vector<double> total_RMHC;
    for (int generation = 0; generation<5000; generation++)
    {
        double dis_RMHC = 0;
        double tot_dist = 0;
        vector<double> total_RMHC = {};
        for(int j = 0; j<10; j++)
        {
            srand( unsigned(j+generation) + unsigned(time(0)));
            int location = rand()%200+800;
            int mother_index = rand()%10;
            vector<int> mother = paths[mother_index]; //get a copy of one parent path in the matrix paths
            srand( unsigned(j+generation) + unsigned(time(0)));
            int father_index = rand()%10;
            while (father_index == mother_index)
            {
                father_index = rand()%10;
            }
            vector<int> father = paths[father_index];

            for (int m = 0; m<location; m++)
            {
                father.erase(find(father.begin(), father.end(), mother[m]));
            }


            vector<int> child1;
            child1.insert(child1.end(), mother.begin(), mother.begin()+location);
            child1.insert(child1.end(), father.begin(), father.end());
            child1[1001] = child1[0];

            int location2 = rand()%1000; //get a random location on the path
            //cout << location << endl;
            float tempo = child1[location2];//Make one small change
            //cout << paths[j][location] << " "<< paths[j][location+1]<<" ";
            if (location2 == 999)
            {
                child1[location2] = child1[0];
                child1[0] = tempo;
                paths.push_back(child1);
            }
            else
            {
                child1[location2] = child1[location2+1];
                child1[location2+1] = tempo;
                paths.push_back(child1);
            }
            child1.clear();
            mother.clear();
            father.clear();
        }//we have a 20*1000 matrix(paths) by the end of the loop

        for (int k = 0; k<20; k++)
        {
            for (int a = 0; a<1001; a++)
            {
                if (a == 1000)
                {
                    dis_RMHC = sqrt(pow((x_cord[paths[k][0]] - x_cord[paths[k][a]]) , 2.0) + pow((y_cord[paths[k][0]] - y_cord[paths[k][a]]) , 2.0));
                }
                else
                {
                    dis_RMHC = sqrt(pow((x_cord[paths[k][a+1]] - x_cord[paths[k][a]]) , 2.0) + pow((y_cord[paths[k][a+1]] - y_cord[paths[k][a]]) , 2.0));
                }
                tot_dist += dis_RMHC;
            }
            //cout << tot_dist << endl;
            total_RMHC.push_back(tot_dist); //We have 20 distances now
            tot_dist = 0;
        }

        vector <double> kl = total_RMHC;
        sort(total_RMHC.begin(), total_RMHC.end()); //Sort the 20 distances in ascending order
        //sort(total_RMHC.begin(), total_RMHC.end(), greater<int>()); //Uncomment for the longest
        vector<vector<int>> temp;
        for (int id = 0; id<10; id++ ) //Run 10 times to obtain the index of the top 10 paths
        {
            vector<int> top10_id;
            //cout << getIndex(kl, total_RMHC[id])<<endl;
            temp.push_back(paths[getIndex(kl, total_RMHC[id])]);
        }
        id_shorest = paths[0];
        paths.clear();
        paths = temp;
        shorest = total_RMHC[0];
        EA << shorest << endl;
        cout << shorest<<endl;
    }
    cord_EA << "x_coordinate" << " " << "y_coordinate" << endl;
    //cout << "x_coordinate" << " " << "y_coordinate" << endl;
    for (int k = 0; k < 1001; k++)
    {
        //cout << x_cord[id_shorest[k]] << " " << y_cord[id_shorest[k]] << endl;
        cord_EA << x_cord[id_shorest[k]] << " " << y_cord[id_shorest[k]] << endl;
    }
}

void randomsearch()
{
    ofstream myfile;
    ofstream cord;
    myfile.open("distancce_rs.csv");
    cord.open("coordinate_rs.csv");
    for (int i = 0; i<5000; i++)
    {
        id = permutation(i);
        for (int j = 0; j<1001; j++)
        {
            if (j == 1000)
            {
                dis = sqrt(pow((x_cord[id[0]] - x_cord[id[j]]) , 2.0) + pow((y_cord[id[0]] - y_cord[id[j]]) , 2.0));
            }
            else
            {
                dis = sqrt(pow((x_cord[id[j+1]] - x_cord[id[j]]) , 2.0) + pow((y_cord[id[j+1]] - y_cord[id[j]]) , 2.0));
            }
            total += dis;
        }
        if (total < shorest)
        {
            shorest = total;
            id_shorest = id;
            total = 0;
        }


        else
        {
            total = 0;
        }
        cout << shorest << endl;
        myfile << shorest << endl;
    }

    cord << "x_coordinate" << " " << "y_coordinate" << endl;
    //cout << "x_coordinate" << " " << "y_coordinate" << endl;
    for (int k = 0; k < 1001; k++)
    {
        //cout << x_cord[id_shorest[k]] << " " << y_cord[id_shorest[k]] << endl;
        cord << x_cord[id_shorest[k]] << " " << y_cord[id_shorest[k]] << endl;
    }
}

void getcoordinate(string filename)
{
    string line;
    ifstream myfile;

    myfile.open(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            x_cord.push_back(stof(line.substr(0, 6)));
            y_cord.push_back(stof(line.substr(8, 6)));
        }
        myfile.close();
    }

    /*for(int i = 0, j = 0; i <x_cord.size() && j < y_cord.size(); i++, j++)
    {
        cout << x_cord.at(i) << " ";
        cout << y_cord.at(j) << endl;
    }*/
}

vector<int> permutation(int seed)
{
    srand( unsigned(seed) + unsigned(time(0))); //Generate a seed for rand()
    vector <int> index;
    for (int i = 0; i<1000; i++)
    {
        index.push_back(i);
    }
    random_shuffle(index.begin(), index.end(), randomit);
    index.push_back(index[0]);

    /*for (vector<int>::iterator it=index.begin(); it!=index.end(); it++)
    {
        cout <<' '<<*it;
    }*/
    return index;
}

int getIndex (vector<double> distances, double dist)
{
    auto it = find(distances.begin(), distances.end(), dist);
    if (it != distances.end())
    {
        int index = it-distances.begin();
        return index;
    }
    else
    {
        return -1;
    }
}

int getIndex_int (vector<int> distances, int dist)
{
    auto it = find(distances.begin(), distances.end(), dist);
    if (it != distances.end())
    {
        int index = it-distances.begin();
        return index;
    }
    else
    {
        return -1;
    }
}
