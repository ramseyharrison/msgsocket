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
    case MessageType::Online : {
      username = m.getMsg();
      cout << "user " << username << " is now online" << endl;
    
      Message resp {MessageType::OnlineAwk};
      printf("sending %s\n", resp.to_bytes().c_str());
      *c << resp.to_bytes();
      server.add_connection(username);
      break;
    }
    case MessageType::GetUsersReq : {
      Message respUsers {MessageType::GetUsersResp, server.userlist()};
      *c << respUsers.to_bytes();
      break;
    }
    case MessageType::Chat : {
      std::string chat = std::string(m.to_bytes(), 5);
      std::string destination =
	std::string(chat.begin(), chat.begin() + chat.find_first_of(' '));
      std::string text = std::string(chat, chat.find_first_of(' ') + 1);
      server.add_chat(username, destination, text);
      cout << server.chats_to_bytes(destination) << endl;
      //server.new_chat(destination,text);
      //cout << server.fetch_chats(destination);
      break;
    }
    case MessageType::FetchChat : {
      Message respChats {MessageType::FetchChatResp, server.chats_to_bytes(username)};
       *c << respChats.to_bytes();
      break;
    }
    default: {
      cout << "error\n" << endl;
      server.remove_connection(username);
      delete c;
      return NULL;
    }
    }    
  }
  server.remove_connection(username);
  delete c;  
  return NULL;
}
 
int main (int argc, char * argv[])
{

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
