#include<iostream>
#include<string>
#include<iomanip>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<cstring>
#include<set>
#include<iterator>
using namespace std;

class Leg
{
  const char* const startCity;
  const char* const endCity;
  const double distance;

public:
  Leg(const char* const, const char* const, const double);
  Leg& operator=(const Leg&);
  double getDistance() const;
  void output(ostream&) const;
  friend class Route;
  friend class ShortestRoute;
};

class Route
{
private:
  vector<const Leg*> legs;
  const double totalDis;
public:
  Route(const Leg&);
  Route(const Route&,const Leg&);
  Route& operator=(const Route&);
  double getDistance() const;
  void output(ostream&) const;
  friend bool operator<(const Route&, const Route&);
};

class ShortestRoute
{
  static const int legSize;
  static const Leg allLegs[];

public:
  static const Route anyRoute(const char* const, const char* const);
  static const Route shortestRoute(const char* const, const char* const);
};

const Leg ShortestRoute::allLegs[] =
{
  Leg("Memphis", "Orlando", 684),
  Leg("Miles City", "Fargo", 430),
  Leg("Fargo", "Minneapolis", 214),
  Leg("Fargo", "Chicago", 569),
  Leg("Denver", "Omaha", 484),
  Leg("Denver", "Oklahoma City", 504),
  Leg("Memphis", "Washington", 763),
  Leg("New Orleans", "Orlando", 533),
  Leg("San Francisco", "Las Angeles", 352),
  Leg("Minneapolis", "Cleveland", 630),
  Leg("Chicago", "Cleveland", 307),
  Leg("Cleveland", "Pittsburgh", 115),
  Leg("Pittsburgh", "New York City", 315),
  Leg("Chicago", "Pittsbrugh", 409),
  Leg("San Francisco", "Portland", 536),
  Leg("Reno", "Salt Lake City", 427),
  Leg("Las Angeles", "Phoenix", 357),
  Leg("San Francisco", "Reno", 186),
  Leg("El Paso", "Houston", 674),
  Leg("El Paso", "Oklahoma City", 576),
  Leg("Helena", "Miles City", 294),
  Leg("Helena", "Fargo", 721),
  Leg("Miles City", "Omaha", 606),
  Leg("Reno", "Phoenix", 601),
  Leg("Portland", "Helena", 516),
  Leg("Salt Lake City", "Denver", 371),
  Leg("Salt Lake City", "Miles City", 494),
  Leg("Omaha", "Chicago", 435),
  Leg("Omaha", "St. Louis", 357),
  Leg("Omaha", "Memphis", 531),
  Leg("Oklahoma City", "Memphis", 421),
  Leg("Houston", "Memphis", 485),
  Leg("Houston", "New Orleans", 318),
  Leg("St. Louis", "Philadelphia", 809),
  Leg("Reno", "Phoenix", 601),
  Leg("Phoenix", "Denver", 586),
  Leg("Phoenix", "El Paso", 345),
  Leg("Chicago", "Philadelphia", 664),
  Leg("Memphis", "Philadelphia", 879),
  Leg("Orlando", "Washington", 759),
  Leg("Washington", "New York City", 203),
  Leg("Philadelphia", "New York City", 80),
  Leg("San Francisco", "New York City", 21000)
};

const int ShortestRoute::legSize = sizeof(allLegs) / sizeof(allLegs[0]);

ostream& roundingOne(ostream&);
ostream& roundingOff(ostream&);

int main()
{
  cout << "Programmer: Younghyun Kwon\n"
  << "Programmer's ID: 1651849\n"
  << "File: " << __FILE__ << "\n\n";

  Route anyRoute = ShortestRoute::anyRoute("San Francisco", "New York City");
  cout << "Any route: \n";
  anyRoute.output(cout);
  cout << endl;

  Route shortestRoute = ShortestRoute::shortestRoute("San Francisco", "New York City");
  cout << "Shortest route: \n";
  shortestRoute.output(cout);
  cout << endl;

}

Leg::Leg(const char* const startCity, const char* const endCity, const double distance)
: startCity(startCity), endCity(endCity), distance(distance)
{
}

Leg& Leg::operator=(const Leg& leg)
{
  Leg& host = *this;
  if(this != &leg)
  {
    const_cast<const char*&>(host.startCity) = leg.startCity;
    const_cast<const char*&>(host.endCity) = leg.endCity;
    const_cast<double&>(host.distance) = leg.distance;
  }
  return host;
}

double Leg::getDistance() const
{
  return distance;
}

void Leg::output(ostream& out) const
{
  roundingOne(out);
  out << "Leg: " << startCity << " to " << endCity << ", " << distance << " miles\n";
  roundingOff(out);
}

Route::Route(const Leg& leg)
:totalDis(leg.distance)
{
  legs.push_back(&leg);
}

Route::Route(const Route& route,const Leg& leg)
:legs(route.legs), totalDis((strcmp(route.legs.back()->endCity,leg.startCity) == 0)? route.totalDis + leg.distance: route.totalDis)
{
  try
  {
    if(strcmp(route.legs.back()->endCity,leg.startCity) != 0)
    {
      throw "Cities don't match!";
    }
    legs.push_back(&leg);
  }
  catch(const char* ex)
  {
    cout << ex << endl;
  }
}

Route& Route::operator=(const Route& route)
{
  Route& host = *this;
  if(this != &route)
  {
    host.legs = route.legs;
    const_cast<double&>(host.totalDis) = route.totalDis;
  }
  return host;
}

double Route::getDistance() const
{
  return totalDis;
}

void Route::output(ostream& out) const
{
  roundingOne(out);
  out << legs[0]->startCity;
   for(int i = 0; i < legs.size(); i++)
   {
     out << " to "<< legs[i]->endCity;
   }
   out << ", " << totalDis << " miles.\n";
   roundingOff(out);
}

bool operator<(const Route &r1,const Route &r2)
{
  return (r1.totalDis < r2.totalDis) ? true : false;
}

const Route ShortestRoute::anyRoute(const char* const start, const char* const end)
{
  for(int i = 0; i < ShortestRoute::legSize; i++)
  {
    if(strcmp(ShortestRoute::allLegs[i].endCity, end) == 0)
    {
      if(strcmp(ShortestRoute::allLegs[i].startCity, start) == 0)
      {
       Route tempR(allLegs[i]);
       return tempR;
      }
      else
      {
        Route tempR(ShortestRoute::anyRoute(start, ShortestRoute::allLegs[i].startCity), allLegs[i]);
        return tempR;
      }
    }
  }
  throw "Can't find a possible route!";
}

const Route ShortestRoute::shortestRoute(const char* const start, const char* const end)
{
  set<Route> allRoutes;

  for(int i = 0; i < ShortestRoute::legSize; i++)
  {
    if(strcmp(ShortestRoute::allLegs[i].endCity, end) == 0)
    {
      if(strcmp(ShortestRoute::allLegs[i].startCity, start) == 0)
      {
        Route tempR(allLegs[i]);
        allRoutes.insert(tempR);
      }
      else
      {
        Route tempR(ShortestRoute::anyRoute(start, ShortestRoute::allLegs[i].startCity), allLegs[i]);
        allRoutes.insert(tempR);
      }
    }
  }

  if (!allRoutes.empty()) return *(allRoutes.begin());
    throw "Can't find the shortest route!";
}

ostream& roundingOne(ostream& out)
  {
    out.setf(ios::fixed);
    out.precision(1);
    return out;
  }

ostream& roundingOff(ostream& out)
  {
    out.unsetf(ios::fixed);
    out.precision(6);
    return out;
  }
