#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"

using namespace std;

int RatioCmp(const void *ptr, const void *ptr2)
{
    if(((EvacRoute4*)ptr)->ratio > ((EvacRoute4*)ptr2)->ratio)
        return -1;
    else
    if(((EvacRoute4*)ptr)->ratio < ((EvacRoute4*)ptr2)->ratio)
        return 1;
    else
        return 0;
}

Evac::Evac(City *citie, int numCitie, int numRoads) : numCities(numCitie)
{
    numCities = numCitie;
    cities = new City4[numCities];
    roads = numRoads;
    edge = new Road4[roads * 2]; //roads = new Road *[numRoads * 2];

    roadsOccupied = new int[numRoads * 5]; // each road max 5

    visitedIDs = new int[numCities];

    for(int i = 0; i < numCitie; i++)
    {;
        cities[citie[i].ID].evacPeople = 0; //cities.ID = citie.ID;
        cities[citie[i].ID].population = citie[i].population; //cities.population = citie.population;
        cities[citie[i].ID].numEdges = citie[i].roadCount; //cities.roadCount = citie.roadCount;
        cities[citie[i].ID].edgeID = new int[citie[i].roadCount];

        for(int j = 0; j < citie[i].roadCount; j++)
        {
            cities[citie[i].ID].edgeID[j] = citie[i].roads[j].ID;  //cities[i].roads[j].ID = citie[i].roads[j].ID;
            edge[citie[i].roads[j].ID].destinationCityID = citie[i].roads[j].destinationCityID;
            //cities[i].roads[j].destinationCityID = citie[i].roads[j].destinationCityID;
            edge[citie[i].roads[j].ID].peoplePerHour =  citie[i].roads[j].peoplePerHour;
            //cities[i].roads[j].peoplePerHour = citie[i].roads[j].peoplePerHour;
        }
    }

} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
    timer = 1; //timer

    routeCount = 0;
    queue = new int[numCities]; //change name ot queue
    front = back = 0;
    evacCities = new EvacRoute4[numEvacs]; //evacCities = new EvacRoute[numEvacs];

    for(int i = 0; i < numEvacs; i++)
    {
        cities[evacIDs[i]].known = true;
        cities[evacIDs[i]].deadEnd = false;
        cities[evacIDs[i]].evacuatedCity = true;
        cities[evacIDs[i]].depth = 1;
        queue[back] = evacIDs[i];
        evacCities[i].ID = evacIDs[i];
        back++;
    } // enqueue evac cities
    bool peopleRemaining = true; //evacMore
    visitedSize = 0;
    while(peopleRemaining)
    {
        int back2 = back;
        int ID, destinationCityID;
        for(int i = 0; i < back; i++)
        {
            cities[queue[i]].deadEnd = false;
        }
        while(front < back2)
        {
            ID = queue[front++];
            for(int j = 0; j < cities[ID].numEdges;)
            {
                destinationCityID = edge[cities[ID].edgeID[j]].destinationCityID;
                if(!cities[destinationCityID].known)
                {
                    ID = destinationCityID;
                    cities[ID].depth = timer + 1;
                    cities[destinationCityID].known = true;
                }
                if(cities[destinationCityID].depth != 0 && cities[destinationCityID].depth < timer - 1)
                {
                    cities[ID].numEdges--;
                    cities[ID].edgeID[j] = cities[ID].edgeID[cities[ID].numEdges];
                    cities[ID].edgeID[cities[ID].numEdges] = cities[ID].edgeID[j];
                } // remove edge
                else {
                  j++;
                }
            } // for connected edges
        } // while queue not end
        int capacity;
        if(peopleRemaining)
        {
              //mark false if done
              peopleRemaining = false;
              for(int i = 0; i < numEvacs; i++)
              {
        //          int ID = evacCities[i].ID;
                  if(cities[evacCities[i].ID].evacPeople < cities[evacCities[i].ID].population)
                  {
                      peopleRemaining = true;
                      capacity = 0;
                      for(int j = 0; j < cities[evacCities[i].ID].numEdges; j++)
                      {
                          if(edge[cities[evacCities[i].ID].edgeID[j]].peoplePerHour < cities[edge[cities[evacCities[i].ID].edgeID[j]].destinationCityID].population){
                                capacity += edge[cities[evacCities[i].ID].edgeID[j]].peoplePerHour;
                          }
                          else {
                                capacity += cities[edge[cities[evacCities[i].ID].edgeID[j]].destinationCityID].population;
                          }
                      } // calculate each edge capacity
                      evacCities[i].ratio = (cities[evacCities[i].ID].population - cities[evacCities[i].ID].evacPeople) / capacity;
                  } // not done
              } // for remaining cities
              if(peopleRemaining)
              {
                  qsort(evacCities, numEvacs, sizeof(EvacRoute4), RatioCmp); // sort by max ratio
              }
        }
        usedCount = 0;
        for(int i = 0; i < numEvacs; i++)
        {
            int cityID = evacCities[i].ID;
            int sum = 0, remaining;
            int required = cities[cityID].population - cities[cityID].evacPeople;
            while(visitedSize > 0) { // mark backroad not visited
                cities[visitedIDs[--visitedSize]].visited = false;
            }
            visitedIDs[visitedSize++] = cityID;
            cities[cityID].visited = true;
            int edgeID, capacityMax;
            for(int i = 0; i < cities[cityID].numEdges && sum < required; i++)
            {
                edgeID = cities[cityID].edgeID[i];
                capacityMax = edge[edgeID].peoplePerHour - edge[edgeID].occupied;
                remaining = required - sum;
                if(capacityMax > remaining)
                {
                    capacityMax = remaining;
                }
                capacityMax = dfs(edge[edgeID].destinationCityID, capacityMax, cityID);
                edge[edgeID].occupied += capacityMax;
                sum += capacityMax;
                if(capacityMax > 0)
                {
                    roadsOccupied[usedCount++] = edgeID;
                }
            }
            cities[cityID].evacPeople += sum;
        }
        for(int i = 0; i < numCities; i++)
        {
            cities[i].visited = false;
        }

        for(int i = 0; i < usedCount; i++)
        {
            evacRoutes[routeCount].roadID = roadsOccupied[i];
            evacRoutes[routeCount].numPeople = edge[roadsOccupied[i]].occupied;
            evacRoutes[routeCount++].time = timer;
            edge[roadsOccupied[i]].occupied = false;
            cities[edge[roadsOccupied[i]].destinationCityID].visited = false;
        }
          timer++;
    }

} // evacuate


int Evac::dfs(int cityID, int required, int sourceCityID)
{
    if(!cities[cityID].visited)
    {
        cities[cityID].visited = true;
        visitedIDs[visitedSize++] = cityID;
    }
    else
    {
        return 0;
    } // store as visited
    int sum = 0;
    int remaining;
    if(!cities[cityID].evacuatedCity) // if not marked evac then return amount
    {
        remaining = cities[cityID].population - cities[cityID].evacPeople;
        if(remaining >= required)
        {
            cities[cityID].evacPeople = cities[cityID].evacPeople + required;
            return required;
        }
        else
        {
            sum = remaining;
            cities[cityID].evacPeople = cities[cityID].population;
        }
    }

    if(cities[cityID].deadEnd)
    {
        return sum;
    }
    int capacityMax;
    for(int i = 0; i < cities[cityID].numEdges && sum < required; i++)
    {
        if(edge[cities[cityID].edgeID[i]].destinationCityID == sourceCityID)
        {
            continue;
        }
        capacityMax = edge[cities[cityID].edgeID[i]].peoplePerHour - edge[cities[cityID].edgeID[i]].occupied;
        if(capacityMax > required - sum)
        {
            capacityMax = required - sum;
        }
        capacityMax = dfs(edge[cities[cityID].edgeID[i]].destinationCityID, capacityMax, cityID);
        edge[cities[cityID].edgeID[i]].occupied = edge[cities[cityID].edgeID[i]].occupied + capacityMax;
        sum = sum + capacityMax;
        if(capacityMax > 0)
        {
            roadsOccupied[usedCount++] = cities[cityID].edgeID[i];
        }
    }
    return sum;
}
