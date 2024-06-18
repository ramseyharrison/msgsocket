#include <string>
#include <iostream>
#include <list>

int main(){
  std::list<std::string> connections; 
  connections.push_back("ramsey");
  std::list<std::string>::iterator it = connections.begin();
  connections.insert(connections.begin(),"john");
  printf("first element %s\n",connections.front().c_str());
  connections.erase(connections.begin());
  printf("element at iterator : %s\n",(*it).c_str());
}
