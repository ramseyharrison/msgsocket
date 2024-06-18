#include "simplesocket.h"
#include "clientsocket.h"
#include "message.h"

#include <pthread.h>
#include <sstream>
#include <iostream>

using namespace std;

int main (int argc, char * argv[])
{
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <port> <id>" << endl;
    exit (-1);
  }
  int port;
  std::string id;
  stringstream (argv[1]) >> port;
  stringstream (argv[2]) >> id;
  clientsocket * s = new clientsocket("127.0.0.1", port);
  s->connect();
  stringstream output;
  Message m {MessageType::Online,id};
  cout << "trying to send " << m.str().length() << endl;
  //output << "CLIENT MESSAGE FROM " << id;
  //cout << "client about to send: " << output.str().c_str() << endl;
  std::string to_send = m.str();
  cout << to_send << endl;
  *s << m.str();
  std::string test;
  std::getline(std::cin,test);
  /*string result;
  *s >> result;
  cout << "Received: {" << result << "}" << endl; */

  s->close();
  return 0;
}
