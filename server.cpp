#include "simplesocket.h"
#include "serversocket.h"
#include "server.h"
#include "message.h"
#include <pthread.h>
#include <sstream>
#include <iostream>

using namespace std;

Server server;

int messageNumber () {
  static int number = 0;
  static pthread_mutex_t numberLock = PTHREAD_MUTEX_INITIALIZER;
  int v;
  pthread_mutex_lock (&numberLock);
  v = number;
  number++;
  pthread_mutex_unlock (&numberLock);
  return v;
}

void * serve (void * cv)
{
  // Just read exactly one string.
  auto * c = (simplesocket*) cv;
  string buffer;
  std::string username;
  while (true) {
    *c >> buffer;
    if (buffer == "") {
      break;
    }
    int num = messageNumber(); 
    //    cout << "[" << num << "] " << buffer << endl;
    Message m = Message::from_string(buffer);
    switch(m.getType()){
    case MessageType::Online:
      username = m.getMsg();
      cout << "user " << username << " is now online" << endl;
      server.add_connection(username);
      printf("%d\n",server.connection_count());
      break;
    case MessageType::Offline:
      cout << "user " << m.getMsg() << " is now offline" << endl;
      break;
    default:
      cout << "error\n" << endl;
      break;
    }
    // Now reply.
    stringstream str;
    str << "Received message " << num;
    *c << str.str();
  }
  server.remove_connection(username);
  delete c;  
  return NULL;
}
 
int main (int argc, char * argv[])
{

  Server server {};
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <port>" << endl;
    exit (-1);
  }
  int port;
  stringstream (argv[1]) >> port;
  serversocket * s = new serversocket (port);
      
  cout << "Entering loop.\n" << endl;
  while (true) {
    // Create one thread per connection.
    auto c = s->accept();
    pthread_t * t = new pthread_t;
    pthread_create (t, NULL,serve, (void *) c);
  }
  // We will never get here...
  s->close();
  return 0;
}
