#include <iostream>  // for debugging
#include "spreader.h"
using namespace std;


Spreader::Spreader(const Person *people, int population)
{
    day = 0;
    pop = population;
    p = new PersonTwo[population];
    valueOfUnknowns = new int[population];
    known = new bool[population];
    for(int i = 0; i < population; i++) {
        valueOfUnknowns[i] = INT_MAX;
        p[i] = PersonTwo(people[i].ID, people[i].peopleMet);
        p[i].visitors = new Visitor[people[i].peopleMet];
        memcpy(p[i].visitors,people[i].visitors, people[i].peopleMet*sizeof(Visitor));
    }
  // Students write this. 
  // Note that people will be deleted before simulate() is called.
} // Spreader()

int Spreader::simulate(int starterIDs[], int starterCount, int commandLineOption)
{
    BinaryHeap<PersonTwo> heap(pop/10);
    for(int i = 0; i < starterCount; i++)
    {
        PersonTwo starter(p[starterIDs[i]]);
        starter.tempDay = 0;
        starter.dayInfected = 0;
        heap.insert(starter);
    }
    int infectedCount = 0;
    while(infectedCount < pop)
    {
        PersonTwo current(heap.findMin());
        heap.deleteMin();
        if(known[current.ID])
            continue;
        infectedCount++;
        known[current.ID] = true;
       if(commandLineOption > 2)
          cout << "Infect Count: " << infectedCount << " day: " << current.tempDay << " ID: " << current.ID << endl;
        for(int i = 0; i < current.peopleMet; i++)
        {
            PersonTwo visitorInfected(p[current.visitors[i].ID]);
            int infectionDay = current.tempDay+INCUBATION_TIME;
            if(infectionDay%current.visitors[i].period != 0)
                infectionDay += (current.visitors[i].period-infectionDay%current.visitors[i].period);
            
            //if it already known and infection day is less then you have to recheck everyone
            if((!known[visitorInfected.ID] && valueOfUnknowns[visitorInfected.ID] == INT_MAX ) || (known[visitorInfected.ID] && infectionDay < valueOfUnknowns[visitorInfected.ID]))
            {
                visitorInfected.tempDay = infectionDay;
                valueOfUnknowns[visitorInfected.ID] = infectionDay;
                heap.insert(visitorInfected);
            }
            //next occurence
        }
        //checked all visitors
        p[current.ID].dayInfected = current.tempDay;
        if( p[current.ID].dayInfected > day)
            day = p[current.ID].dayInfected; //update longest day to get infected
    }
    // Students write this.  Returns days until of last person catches the virus
    
  return day;
}


