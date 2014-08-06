#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <streambuf>
#include <deque>          // std::deque
#include <queue> 
#include <list>
#include "sortTest.h"

using namespace std;


vector<catsCradle> fixParens(catsCradle &x){
	int i,j,k;
	vector<catsCradle> splitByLeftParen;
	vector< vector<catsCradle> > splitByRightParen;
	vector<catsCradle> result;
	splitByLeftParen = x.separatebyDelim("(");
	int numCCobj=0;
	for(i=0;i<splitByLeftParen.size();i++){
		splitByRightParen.push_back(splitByLeftParen[i].separatebyDelim(")"));
	}
	for(i=0;i<splitByRightParen.size();i++){
		numCCobj += splitByRightParen[i].size();
	}
	vector<catsCradle> splitByParens;
	int counter=0;
	for(i=0;i<splitByRightParen.size();i++){
		for(j=0;j<splitByRightParen[i].size();j++){
			splitByParens.push_back(splitByRightParen[i][j]);
			counter++;
		}
	}
	//so now the cc objects are in one array: splitByParens
	int tester = numCCobj;
	for(i=0;i<numCCobj;i++){
		vector<catsCradle> coefficients = splitByParens[i].separatebyDelim(" "); //the even indexed values will hold coefficients as their last spot.
		i++;
		tester--;
		for(j=0;j<coefficients.size()-1;j++){
			result.push_back(coefficients[j]);
		}
		int theCoeff = -1;
		if(tester > 1){
			theCoeff = coefficients[coefficients.size()-1].getInt();
			vector<catsCradle> expression = splitByParens[i].separatebyDelim(" ");
			for(j=0;j<theCoeff;j++){
				for(k=0;k<expression.size();k++){
					result.push_back(expression[k]);
				}
			}
		}
		else{
			result.push_back(coefficients[coefficients.size()-1]);
		}
		tester--;
	}
	return result;
}
int main(){
	int theclock =0;
	string fileName;
	cout << "Enter the input file: ";
	cin >> fileName;
	vector<process> processes;

	catsCradle theInput(fileName,0);
	vector< vector<catsCradle> > grid;
	vector<catsCradle> rows = theInput.getAllRows();
	
	int i,j,k,l,procCounter=0;

	for(i=0;i<rows.size();i++){
		vector<catsCradle> rowFixed = fixParens(rows[i]);
		vector<catsCradle> tempo = rowFixed;
		grid.push_back(tempo);
	}
	
	int numProcesses = grid[0][0].getInt();
	int overhead = grid[0][1].getInt();
	string mode = grid[1][0].getStr();
	int timeSlice = grid[1][1].getInt();
	for(i=2;i<grid.size();i++){
		process temp(grid[i],i-1,timeSlice); //create a process
		processes.push_back(temp);
	}
	deque<process*> readyQueue;
	deque<process*> readyWait;
	deque<process*> doneQueue;

	bool lessProcThanLimit;
	if(processes.size() <= numProcesses){
		for(i=0;i<processes.size();i++){
			readyQueue.push_back(&processes[i]);
			lessProcThanLimit = true;
		}
	}
	else{
		while(procCounter<numProcesses){
		readyQueue.push_back(&processes[procCounter]);
		procCounter++;
		lessProcThanLimit = false;
		}
	}
	sort(readyQueue.begin(),readyQueue.end(), sortByArrival());
	for(i=0;i<readyQueue.size();i++){
		readyQueue[i]->addTime(theclock);
	}	
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	if(mode=="RR"){
		bool done=false;
		string temps;
		while(!done){
			int timeUsed;
			for(i=0;i<readyQueue.size();i++){ //IF ANY PROCESSES IN READY OR WAIT HAVE NO MORE JOBS, TERMINATE (NEED TO MAKE THIS DETERMINE WHAT TIME IT ENTERS)
				if(readyQueue[i]->getIORem()+readyQueue[i]->getCPURem()==0){
					doneQueue.push_back(readyQueue[i]);
					readyQueue.erase(readyQueue.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
			for(i=0;i<readyWait.size();i++){
				if(readyWait[i]->getIORem()+readyWait[i]->getCPURem()==0){
					doneQueue.push_back(readyWait[i]);
					readyWait.erase(readyWait.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
		

			if(readyQueue.size() ==0 && readyWait.size()==0){ //IF THE SIZE OF COMPLETED = SIZE OF PROCESSES WE ARE DONEZO!
				done = true;
			}
			else{ //THERE ARE STILL JOBS TO BE DONE!
				if(readyQueue.size() > 0){ //IF THERE ARE THINGS IN THE READY QUEUE
					timeUsed = readyQueue[0]->useTimeSlice();
					temps = readyQueue[0]->getName();
					readyQueue[0]->addTime(theclock);

					process* goingToWait;
					if(timeUsed	== -1){ //THE PROCESS DID NOT COMPLETE ITS CPU BURST
						process* tempproc = readyQueue[0];
						for(i=1;i<readyQueue.size();i++){ //INCREASE THE OTHER PROCESSES TIME IN READY
							readyQueue[i]->incrementReady(timeSlice);
						}
						timeUsed = timeSlice;
						theclock+=timeSlice;
						readyQueue[0]->addTime(theclock);
						readyQueue.pop_front();
						readyQueue.push_back(tempproc); //PUT THE PROCESS BACK INTO READY QUEUE

					}
					else{ //THE PROCESS COMPLETED ITS CPU BURST. INCREMENT ALL OTHER PROCESS TIME IN READY, THEN PUT THE PROCESS INTO WAIT QUEUE
						
						for(i=1;i<readyQueue.size();i++){
							readyQueue[i]->incrementReady(timeUsed);
						}
						
						theclock += timeUsed;
						readyQueue[0]->addTime(theclock);
						goingToWait=readyQueue[0];
						readyQueue.pop_front();
					}
					
					sort(readyWait.begin(),readyWait.end(),sortByWait());

					int numToErase=0;
					for(j=0;j<readyWait.size();j++){ //ITERATE THROUGH WAIT QUEUE SIMULATING THE AMOUNT OF WAIT TIME
						bool ready = readyWait[j]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
					

					readyWait.push_back(goingToWait);
				}
				else{
					int timeUsed=0;
					for(k=0;k<readyWait.size();k++){
						readyWait[k]->printBursts();
					}
					sort(readyWait.begin(),readyWait.end(),sortByWait());
					timeUsed = readyWait[0]->getNextIO();
					temps = readyWait[0]->getName();
					cout << readyWait.size() <<"~"<<readyQueue.size() << "~" << doneQueue.size()<< endl;

					int numToErase=0;
					for(k=0;k<readyWait.size();k++){
						bool ready = readyWait[k]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
				}
			}

		}
	}
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	if(mode=="Priority"){
		bool done=false;
		string temps;
		while(!done){
			int timeUsed = 0;
			for(i=0;i<readyQueue.size();i++){ //IF ANY PROCESSES IN READY OR WAIT HAVE NO MORE JOBS, TERMINATE (NEED TO MAKE THIS DETERMINE WHAT TIME IT ENTERS)
				if(readyQueue[i]->getIORem()+readyQueue[i]->getCPURem()==0){
					doneQueue.push_back(readyQueue[i]);
					readyQueue.erase(readyQueue.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
			for(i=0;i<readyWait.size();i++){
				if(readyWait[i]->getIORem()+readyWait[i]->getCPURem()==0){
					doneQueue.push_back(readyWait[i]);
					readyWait.erase(readyWait.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
		

			if(readyQueue.size() ==0 && readyWait.size()==0){ //IF THE SIZE OF COMPLETED = SIZE OF PROCESSES WE ARE DONEZO!
				done = true;
			}
			else{ //THERE ARE STILL JOBS TO BE DONE!
				sort(readyQueue.begin(),readyQueue.end(),sortByPriority()); //SORT THE READYQ BY PRIORITY.. THE HIGHEST PRIO WILL GO FIRST
				if(readyQueue.size() > 0){ //IF THERE ARE THINGS IN THE READY QUEUE
					timeUsed = readyQueue[0]->useNextJob();
					temps = readyQueue[0]->getName();
					readyQueue[0]->addTime(theclock);

					process* goingToWait;
					
					 //THE PROCESS COMPLETED ITS CPU BURST. INCREMENT ALL OTHER PROCESS TIME IN READY, THEN PUT THE PROCESS INTO WAIT QUEUE
						
					for(i=1;i<readyQueue.size();i++){
						readyQueue[i]->incrementReady(timeUsed);
					}
					
					theclock += timeUsed;
					readyQueue[0]->addTime(theclock);
					goingToWait=readyQueue[0];
					readyQueue.pop_front();
				
					
					sort(readyWait.begin(),readyWait.end(),sortByWait());

					int numToErase=0;
					for(j=0;j<readyWait.size();j++){ //ITERATE THROUGH WAIT QUEUE SIMULATING THE AMOUNT OF WAIT TIME
						bool ready = readyWait[j]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
					

					readyWait.push_back(goingToWait);
				}
				else{
					sort(readyWait.begin(),readyWait.end(),sortByWait());
					timeUsed = readyWait[0]->getNextIO();
					temps = readyWait[0]->getName();
					int numToErase=0;
					for(k=0;k<readyWait.size();k++){
						bool ready = readyWait[k]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
				}
			}

		}
	}
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	if(mode=="SJN"){
		bool done=false;
		string temps;
		while(!done){
			int timeUsed = 0;
			for(i=0;i<readyQueue.size();i++){ //IF ANY PROCESSES IN READY OR WAIT HAVE NO MORE JOBS, TERMINATE (NEED TO MAKE THIS DETERMINE WHAT TIME IT ENTERS)
				if(readyQueue[i]->getIORem()+readyQueue[i]->getCPURem()==0){
					doneQueue.push_back(readyQueue[i]);
					readyQueue.erase(readyQueue.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
			for(i=0;i<readyWait.size();i++){
				if(readyWait[i]->getIORem()+readyWait[i]->getCPURem()==0){
					doneQueue.push_back(readyWait[i]);
					readyWait.erase(readyWait.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
		

			if(readyQueue.size() ==0 && readyWait.size()==0){ //IF THE SIZE OF COMPLETED = SIZE OF PROCESSES WE ARE DONEZO!
				done = true;
			}
			else{ //THERE ARE STILL JOBS TO BE DONE!
				sort(readyQueue.begin(),readyQueue.end(),sortBySJN()); //SORT THE READYQ BY PRIORITY.. THE HIGHEST PRIO WILL GO FIRST
				if(readyQueue.size() > 0){ //IF THERE ARE THINGS IN THE READY QUEUE
					timeUsed = readyQueue[0]->useNextJob();
					temps = readyQueue[0]->getName();
					readyQueue[0]->addTime(theclock);

					process* goingToWait;
					
					 //THE PROCESS COMPLETED ITS CPU BURST. INCREMENT ALL OTHER PROCESS TIME IN READY, THEN PUT THE PROCESS INTO WAIT QUEUE
						
					for(i=1;i<readyQueue.size();i++){
						readyQueue[i]->incrementReady(timeUsed);
					}
					
					theclock += timeUsed;
					readyQueue[0]->addTime(theclock);
					goingToWait=readyQueue[0];
					readyQueue.pop_front();
				
					
					sort(readyWait.begin(),readyWait.end(),sortByWait());

					int numToErase=0;
					for(j=0;j<readyWait.size();j++){ //ITERATE THROUGH WAIT QUEUE SIMULATING THE AMOUNT OF WAIT TIME
						bool ready = readyWait[j]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
					

					readyWait.push_back(goingToWait);
				}
				else{
					sort(readyWait.begin(),readyWait.end(),sortByWait());
					timeUsed = readyWait[0]->getNextIO();
					temps = readyWait[0]->getName();
					int numToErase=0;
					for(k=0;k<readyWait.size();k++){
						bool ready = readyWait[k]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
				}
			}

		}
	}
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	if(mode=="SRTN"){
		bool done=false;
		string temps;
		while(!done){
			int timeUsed = 0;
			for(i=0;i<readyQueue.size();i++){ //IF ANY PROCESSES IN READY OR WAIT HAVE NO MORE JOBS, TERMINATE (NEED TO MAKE THIS DETERMINE WHAT TIME IT ENTERS)
				if(readyQueue[i]->getIORem()+readyQueue[i]->getCPURem()==0){
					doneQueue.push_back(readyQueue[i]);
					readyQueue.erase(readyQueue.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
			for(i=0;i<readyWait.size();i++){
				if(readyWait[i]->getIORem()+readyWait[i]->getCPURem()==0){
					doneQueue.push_back(readyWait[i]);
					readyWait.erase(readyWait.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
		

			if(readyQueue.size() ==0 && readyWait.size()==0){ //IF THE SIZE OF COMPLETED = SIZE OF PROCESSES WE ARE DONEZO!
				done = true;
			}
			else{ //THERE ARE STILL JOBS TO BE DONE!
				sort(readyQueue.begin(),readyQueue.end(),sortBySRTN()); //SORT THE READYQ BY PRIORITY.. THE HIGHEST PRIO WILL GO FIRST
				if(readyQueue.size() > 0){ //IF THERE ARE THINGS IN THE READY QUEUE
					timeUsed = readyQueue[0]->useNextJob();
					temps = readyQueue[0]->getName();
					readyQueue[0]->addTime(theclock);

					process* goingToWait;
					
					 //THE PROCESS COMPLETED ITS CPU BURST. INCREMENT ALL OTHER PROCESS TIME IN READY, THEN PUT THE PROCESS INTO WAIT QUEUE
						
					for(i=1;i<readyQueue.size();i++){
						readyQueue[i]->incrementReady(timeUsed);
					}
					
					theclock += timeUsed;
					readyQueue[0]->addTime(theclock);
					goingToWait=readyQueue[0];
					readyQueue.pop_front();
				
					
					sort(readyWait.begin(),readyWait.end(),sortByWait());

					int numToErase=0;
					for(j=0;j<readyWait.size();j++){ //ITERATE THROUGH WAIT QUEUE SIMULATING THE AMOUNT OF WAIT TIME
						bool ready = readyWait[j]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
					

					readyWait.push_back(goingToWait);
				}
				else{
					sort(readyWait.begin(),readyWait.end(),sortByWait());
					timeUsed = readyWait[0]->getNextIO();
					temps = readyWait[0]->getName();
					int numToErase=0;
					for(k=0;k<readyWait.size();k++){
						bool ready = readyWait[k]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
				}
			}

		}
	}
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	//#########################################################################################//
	if(mode=="FCFS"){
		bool done=false;
		string temps;
		while(!done){
			int timeUsed = 0;
			for(i=0;i<readyQueue.size();i++){ //IF ANY PROCESSES IN READY OR WAIT HAVE NO MORE JOBS, TERMINATE (NEED TO MAKE THIS DETERMINE WHAT TIME IT ENTERS)
				if(readyQueue[i]->getIORem()+readyQueue[i]->getCPURem()==0){
					doneQueue.push_back(readyQueue[i]);
					readyQueue.erase(readyQueue.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
			for(i=0;i<readyWait.size();i++){
				if(readyWait[i]->getIORem()+readyWait[i]->getCPURem()==0){
					doneQueue.push_back(readyWait[i]);
					readyWait.erase(readyWait.begin()+i);
					if(doneQueue.size()<processes.size() && lessProcThanLimit==false){
						readyQueue.push_back(&processes[procCounter]);
						processes[procCounter].addTime(theclock);
						procCounter++;
					}
				}
			}
		

			if(readyQueue.size() ==0 && readyWait.size()==0){ //IF THE SIZE OF COMPLETED = SIZE OF PROCESSES WE ARE DONEZO!
				done = true;
			}
			else{ //THERE ARE STILL JOBS TO BE DONE!
				sort(readyQueue.begin(),readyQueue.end(),sortByArrival()); //SORT THE READYQ BY PRIORITY.. THE HIGHEST PRIO WILL GO FIRST
				if(readyQueue.size() > 0){ //IF THERE ARE THINGS IN THE READY QUEUE
					timeUsed = readyQueue[0]->useNextJob();
					temps = readyQueue[0]->getName();
					readyQueue[0]->addTime(theclock);

					process* goingToWait;
					
					 //THE PROCESS COMPLETED ITS CPU BURST. INCREMENT ALL OTHER PROCESS TIME IN READY, THEN PUT THE PROCESS INTO WAIT QUEUE
						
					for(i=1;i<readyQueue.size();i++){
						readyQueue[i]->incrementReady(timeUsed);
					}
					
					theclock += timeUsed;
					readyQueue[0]->addTime(theclock);
					goingToWait=readyQueue[0];
					readyQueue.pop_front();
				
					
					sort(readyWait.begin(),readyWait.end(),sortByWait());

					int numToErase=0;
					for(j=0;j<readyWait.size();j++){ //ITERATE THROUGH WAIT QUEUE SIMULATING THE AMOUNT OF WAIT TIME
						bool ready = readyWait[j]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
					

					readyWait.push_back(goingToWait);
				}
				else{
					sort(readyWait.begin(),readyWait.end(),sortByWait());
					timeUsed = readyWait[0]->getNextIO();
					temps = readyWait[0]->getName();
					int numToErase=0;
					for(k=0;k<readyWait.size();k++){
						bool ready = readyWait[k]->simulateWait(timeUsed);
						if(ready){
							numToErase++;
							readyQueue.push_back(readyWait[j]);
						}
					}
					if(numToErase!=0){
						readyWait.erase(readyWait.begin(),readyWait.begin()+numToErase);
					}
				}
			}

		}
	}


	cout << "PID\tName\tArrival\tStart\tCPU\tIO\tReady\tEnd Time"<<endl;;
	for(i=0;i<processes.size();i++){
		processes[i].printResults();
	}
}

/*
GET PRIORITY DONE
GET FIRST COME FIRST SERVE DONE
GET SJN DONE
GET SHORTEST REM TIME NEXT DONE
CREATE PYTHON GUI
LINK THIS TO PYTHON GUI
MAKEFILE
*/

