#include "simplesocket.h"
#include "clientsocket.h"
#include "message.h"

#include <pthread.h>
#include <sstream>
#include <iostream>
#include <regex>

using namespace std;

clientsocket * s;

void prompt()
{
  bool await_response = false;
  std::string response;
  regex chat_reg("chat [\\w]+ [\\S " "]+");
  std::smatch chat_matches;

  while(true){   
    std::string input;
    std::getline (std::cin,input);
    if(input == "users"){      
       Message reqUsers {MessageType::GetUsersReq};
       *s << reqUsers.to_bytes();
       await_response = true;
    }else if(std::regex_match(input,chat_matches,chat_reg)){
      std::string msg_data = std::string(input, 5);
      size_t sep = msg_data.find_first_of(' ');      
      std::string destination = std::string(msg_data,0,sep);
      std::string message = std::string(msg_data, sep + 1);
      Message chat {MessageType::Chat, destination + " " + message};
      *s << chat.to_bytes();
    }else if(input == "chats"){
      Message fetch {MessageType::FetchChat};
      *s << fetch.to_bytes();
      await_response = true;
    }
    if(await_response){
      *s >> response;
      cout << Message::from_string(response).str() << endl;
      await_response = false;
    }
  }
}

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
  s = new clientsocket("127.0.0.1", port);
  s->connect();
  stringstream output;
  Message m {MessageType::Online,id};
  *s << m.to_bytes();  
  string result;
  *s >> result;
  Message resp = Message::from_string(result);
  assert(resp.getType() == MessageType::OnlineAwk);
  cout <<  resp.str();
  prompt();
  s->close();  
  return 0;
}
