// Author: Sean Davis
#ifndef evacH
#define evacH

#include "EvacRunner.h"

class City4
{
public:
      bool known, deadEnd, evacuatedCity;
      int *edgeID;
      int population; //population
      int evacPeople;
      char numEdges; //roadCount
      int depth;
      bool visited;
//      City4():known(false), deadEnd(true), evacuatedCity(false), evacPeople(0), visited(false) {}
};

class Road4
{
public:
      int destinationCityID;
      int peoplePerHour;
      int occupied;
//      Road4():occupied(0) {}
};

class EvacRoute4
{
public:
      int ID;
      float ratio;
};


class Evac
{
public:

    City4 *cities;
    Road4 *edge;
    EvacRoute4 *evacCities;
    int numCities;
    int roads;
    int timer; //change to timer
    int *queue;
    int back, front;
    int *visitedIDs;
    int visitedSize;
    int *roadsOccupied;
    int usedCount;
	/*
    City* cities;;
    int numCities;
    MaxFlow *maxflow; */
    /*
    int numRoads;
    int evacID;
    int currentCity;
    int timer; //need to set it equal to 1*/
	int dfs(int cityID, int needed, int sourceCityID);
   	Evac(City *cities, int numCities, int numRoads);
  	void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes, int &routeCount); // student sets evacRoutes and routeCount

//    void Graph(int numCities);
//    void bfs(int *evacIDs, int numEvacs);
}; // class Evac



#endif
