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
#include "catsCradle.h"
using namespace std;

class process{
public:
	process(vector<catsCradle> &x, int p,int gg){
		slice = gg;
		theclock=0;
		name = x[0].getStr();
		pid = p;
		int i;
		priority = x[1].getInt();
		arrival = x[2].getInt();
		bool cpu = true;
		for(i=3;i<x.size()-1;i++){
			if(cpu== true){
				cpu = false;
				cpuBursts.push_back(x[i].getInt());
			}
			else{
				cpu = true;
				ioBursts.push_back(x[i].getInt());
			}
		}
		int totalCPUs=0;
		for(i=0;i<cpuBursts.size();i++){
			totalCPUs+=cpuBursts[i];
		}
		int totalIOs=0;
		for(i=0;i<ioBursts.size();i++){
			totalIOs+=ioBursts[i];
		}
		timeRem = totalIOs+totalCPUs;
		timeIORem = totalIOs;
		totalIO=totalIOs;
		totalCPU=totalCPUs;
		totalReady=0;
	}
	~process(){
	}
	int getPid(){return pid;}
	int getCPURem(){
		int i;
		int totalCPUs=0;
		for(i=0;i<cpuBursts.size();i++){
			totalCPUs+=cpuBursts[i];
		}
		int totalIOs=0;
		for(i=0;i<ioBursts.size();i++){
			totalIOs+=ioBursts[i];
		}
		return totalCPUs;
	}
	int getIORem(){
		int i;

		int totalIOs=0;
		for(i=0;i<ioBursts.size();i++){
			totalIOs+=ioBursts[i];
		}
		return totalIOs;
	}
	int useTimeSlice(){ //how much of the time slice was used
		int i;
		int temp;
		for(i=0;i<cpuBursts.size();i++){
			if(cpuBursts[i]!=0){
				if(cpuBursts[i]<=slice){
					timeRem -= cpuBursts[i];
					temp = cpuBursts[i];

					cpuBursts[i] = 0;
					return temp;
				}
				else{
					cpuBursts[i] -= slice;
					timeRem -= slice;
					return -1;
				}
			}
		}
	}
	int useNextJob(){
		int i;
		int temp;
		for(i=0;i<cpuBursts.size();i++){
			if(cpuBursts[i]!=0){
				timeRem -= cpuBursts[i];
				temp = cpuBursts[i];
				cpuBursts[i] = 0;
				return temp;
			}
		}
	}
	bool simulateWait(int timex){
		int i,j;
		for(i=0;i<ioBursts.size();i++){
			if(ioBursts[i]!=0){
				if(ioBursts[i] > timex){
					ioBursts[i] -= timex;
					return false;
				}
				else{
					totalReady = totalReady + timex-ioBursts[i];
					ioBursts[i] = 0;
					return true;
				}
			}
		}
	}
	int getNextJob(){
		int i;
		for(i=0;i<cpuBursts.size();i++){
			if(cpuBursts[i]!=0){
				return cpuBursts[i];
			}
		}
		return 0;
	}
	int getNextIO(){
		int i;
		for(i=0;i<ioBursts.size();i++){
			if(ioBursts[i]!=0){
				return ioBursts[i];
				break;
			}
		}
		return 0;
	}
	int getArrival(){
		return arrival;
	}
	void setArrival(int x){
		arrival=x;
	}
	void incrementReady(int x){
		totalReady=totalReady+x;
	}
	int getReady(){
		return totalReady;
	}
	void printBursts(){
		int i,j;
		cout << "CPU BURSTS: ";
		for(i=0;i<cpuBursts.size();i++){
			cout << cpuBursts[i] << "~";
		}
		cout << endl;
		cout << "IO BURSTS: ";
		for(i=0;i<ioBursts.size();i++){
			cout << ioBursts[i]<<"#";
		}
		cout << endl;
	}
	string getName(){
		return name;
	}
	int getClock(){
		return theclock;
	}
	void incrementClock(int x){
		theclock+=x;
	}
	void addTime(int thetime){
		startTime.push_back(thetime);
	}
	int getStartTime(){
		return startTime[0];
	}
	int getEndTime(){
		return startTime[startTime.size()-1];
	}
	int getPriority(){
		return priority;
	}
	void printResults(){
		cout << pid << '\t' << name << '\t' << arrival << '\t' << startTime[0] << '\t' << totalCPU << '\t' << totalIO << '\t'<<totalReady << '\t' << startTime[startTime.size()-1] << endl;
	}
	int totalReady;
	vector<int> cpuBursts;
	vector<int> ioBursts;
	vector<int> startTime;
	int timeRem;
	int timeIORem;
	int arrival;
	int theclock;

private:
	int totalCPU;
	int totalIO;
	string name;
	int pid;
	int priority;
	
	int slice;
	
};