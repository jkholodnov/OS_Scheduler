#include "process.h" 

class sortByArrival{
public: 
bool operator ()(process* address1,process* address2){
	return address1->getArrival()<address2->getArrival(); } 
};

class sortByWait{
public: 
bool operator ()(process* address1,process* address2){
	return address1->getNextIO()<address2->getNextIO(); } 
};

class sortByPriority{
public: 
bool operator ()(process* address1,process* address2){
	return address1->getPriority()>address2->getPriority(); } 
};
class sortBySJN{
public: 
bool operator ()(process* address1,process* address2){
	return address1->getNextJob()<address2->getNextJob(); } 
};
class sortBySRTN{
public: 
bool operator ()(process* address1,process* address2){
	return address1->getCPURem()<address2->getCPURem(); } 
};
