//////////////////////////////////////////////////////////////////////////////////////
//																					//
//											//
//																					//
//////////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <streambuf>
using namespace std;

class catsCradle{
	public:
		catsCradle(string textfile, int Openthefile){
			ifstream in;
			string str;
			in.open(textfile.c_str());
			if(in.is_open()){ 
				in.seekg(0,ios::end);  
				str.reserve(in.tellg());
				in.seekg(0,ios::beg);
				 
				str.assign((istreambuf_iterator<char>(in)),
						istreambuf_iterator<char>());
			}
			in.close();
			name = str;
		}
		catsCradle(string x){
			name = x;
		}
		void openFile(string textfile){
			ifstream in;
			string str;
			in.open(textfile.c_str());
			if(in.is_open()){ 
				in.seekg(0,ios::end);  
				str.reserve(in.tellg());
				in.seekg(0,ios::beg);
				 
				str.assign((istreambuf_iterator<char>(in)),
						istreambuf_iterator<char>());
			}
			in.close();
			name = str;
		}
		string findBeforeDelim(string delim1, string delim2){//Returns what is between the two delimeters in the webpage.
			size_t pos;
			string temp = name;
			pos = temp.find(delim1);
			if(pos!=string::npos){
				temp.erase(0,pos+delim1.length());
				pos = temp.find(delim2);
				if(pos!=string::npos){
					string temp1 = temp.substr(0,pos);
					return temp1;
				}
			}
		}
		string findBeforeDelim(int delim, string delim2){//Returns what is between the two delimeters in the webpage.
			size_t pos;
			string tempstring;
			string temp = name;
			pos = temp.find(delim2);
			if(pos!=string::npos){
				tempstring = temp.substr(delim,pos);
				return tempstring;
			}
		}
		vector<catsCradle> getAllRows(){
			size_t pos;
			vector<catsCradle> theReturn;
			string temp = name;
			string tempstring;
			while(temp.length()>0){
				pos = temp.find('\n');
				if(pos!=string::npos){
					tempstring = temp.substr(0,pos);
					catsCradle getpushed(tempstring);
					theReturn.push_back(getpushed);
					temp.erase(0,pos+1);
				}
				else{
					tempstring = temp.substr(0,temp.length());
					catsCradle getpushed(tempstring);
					theReturn.push_back(getpushed);
					temp.erase(0,temp.length());
				}
			}
			return theReturn;
		}
		vector<catsCradle> separatebyDelim(string delim) const{	//get blank space separated strings
			size_t pos;
			vector<catsCradle> theReturn;
			string tempstring;
			string temp = name;
			while(temp.length() > 0){
				pos = temp.find(delim);
				if(pos!=string::npos){
					tempstring = temp.substr(0,pos);
					catsCradle getpushed(tempstring);
					theReturn.push_back(getpushed);
					temp.erase(0,pos+delim.length());
				}
				else{
					tempstring = temp.substr(0,temp.length());
					catsCradle getpushed(tempstring);
					theReturn.push_back(getpushed);
					temp.erase(0,temp.length());
				}
			}
			return theReturn;
		}

		string findBetween(string delim1, string delim2){//Returns what is between the two delimeters in the webpage.
			size_t pos;
			string temp = name;
			pos = temp.find(delim1);
			if(pos!=string::npos){
				temp.erase(0,pos+delim1.length());
				pos = temp.find(delim2);
				if(pos!=string::npos){
					string temp1 = temp.substr(0,pos);
					return temp1;
				}
			}
		}
		
		vector<string> findBetweenAll(string delim1, string delim2){
			size_t pos, pos1;
			vector<string> results;
			string temp = name;
			pos = temp.find(delim1);
			while(pos!=string::npos){
				temp.erase(0,pos+delim1.length());
				pos1 = temp.find(delim2);
				if(pos1!=string::npos){
					string temp1 = temp.substr(0,pos1);
					temp.erase(0,pos1);
					results.push_back(temp1);
				}
				pos = temp.find(delim1);
			}
			return results;
		}
		int count(string x){

			size_t pos;
			int i = 0;
			string temp=name;
			pos = temp.find(x);
			while(pos!=string::npos){
				i++;
				temp.erase(0,pos+x.length());
				pos = temp.find(x);
			}
			return i;
		}

		string eraseFirst(string x){
			size_t pos;
			pos = name.find(x);
			if(pos==string::npos){
				return name;
			}
			else{
				name.erase(pos,pos+x.length());
			}
			return name;
		}
		string eraseAll(string x){
			size_t pos;
			pos = name.find(x);
			while(pos!=string::npos){
				name.erase(pos,pos+x.length());
				pos = name.find(x);
			}
			return name;
		}

		string getName(){
			return name;
		}
		
		int getLength(){
			return name.length();
		}

	private:
		string name;
};

/*
This is a library file made to parse c++ strings with ease. You may pass strings, document names, **website urls**
Constructor(string,mode) 0=work with string, 1=open and read file
getName() : Returns the string that is stored in the object
getLength() : returns the length of the string
count(string) : returns how many times that string is in the object
findBetween(delim1,delim2) : returns the first occurrance of the string that is contained by the two delimeters
findBetweenAll(delim1,delim2) : returns a vector with all strings contained by these delimeters
eraseFirst(string) : Erases the first occurrance of the string from the catsCradle object. Returns the modified string
eraseAll(string) : Erases all occurrances of the string from the catsCradle object. Returns modified string //THE catsCradle IS MODIFIED WITH THIS METHOD
*/