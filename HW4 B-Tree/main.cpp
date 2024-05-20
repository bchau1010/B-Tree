#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <stack>
#include <queue>
#include "ArgumentManager.h"
#include "bTree.h"
using namespace std;


int main(int argc, char* argv[]) {
	ArgumentManager am(argc, argv);
	string infilename = am.get("input");
	string outfilename = am.get("output"); 
	string commandname = am.get("command");

	ifstream input(infilename);
	ifstream command(commandname);
	ofstream output(outfilename);
/*
	ifstream input("test.txt");
	ifstream command("textCommand.txt");
	ofstream output("testOutPut.txt");
*/

  //get command
  string degreeString;
  int degree;
  bool inorder = false;
  vector<int> level;
  string temp;
  string line;
  
  while (command >> temp) {
    //cout << temp << endl;
    if (temp.find("Degree") != std::string::npos){
      degreeString = temp.back();
      stringstream s(degreeString);
      s >> degree;
    }
    else if(temp.find("Inorder") != std::string::npos){
      inorder = true;
    }
    else if(temp.find("Level") != std::string::npos){
      command >> temp;
      stringstream s(temp);
      int num;
      s >> num;
      level.push_back(num);
    }
  }

  
  //get input txt
	string dt;
	string line1;
	while (input >> line1){
    dt += line1 + " ";
	}

  stringstream s(dt);
  int number;
  vector<int> data;
  while ( s >> number ){
    data.push_back( number );
  }

/*
  cout<< degree << endl;
  cout << inorder << endl;
  cout << level[0] << endl;
  cout << data[0] << endl;
*/

  //get rid of duplication
  for(int i = data.size(); i >= 0; i--){
    for(int j =0; j< data.size();j++){
      if(i == j){
        continue;
      }
      else if(data[i] == data[j]){
        data.erase(data.begin() + i);
      }
    }
  }



  btree tree(degree);
  for(int i = 0; i <data.size();i++){
    tree.insert(data[i]);
  }

  if(inorder){
    tree.inorder2(output);
    output << "\n";
  }
  
  for(int i = 0 ; i < level.size();i++){
    if(level[i] < 1 || level[i] > tree.height()){
      output << "empty" ;
       // << "\n";
    }
    else if(i == level.size()-1){
      tree.printLevel(level[i],output);
      break;
    }
    tree.printLevel(level[i],output);
    output << "\n";
  }

  return 0;
}