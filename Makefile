####################################
#		  Jason Kholodnov		   #
#		Basketball Prediction	   #
####################################
CC = g++
CFLAGS = -o
LINK = g++
scheduler: scheduler.cpp
	$(CC) $(CFLAGS) scheduler scheduler.cpp $(libs)
##
##
clean:
	\rm scheduler
##
tar:
	tar cfv schedulerJasonKholodnov.tar Makefile scheduler.cpp catsCradle.h process.h sortTest.h