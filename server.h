#include <vector>
#include <unordered_map>
#include <list>
#include <mutex>

struct ChatMsg{
  std::string from;
  std::list<std::string> messages;
  
};

class Server{
 private:
  std::list<std::string> connections;  
  std::unordered_map<std::string,std::list<std::string>::iterator> userindex;
  int _count;
  std::mutex conn_mutex;
  std::unordered_map<std::string, std::list<std::string>> _chats;
  std::mutex chat_mutex;
  std::unordered_map<std::string,std::unordered_map<std::string, ChatMsg>> _chat_map;

 public:

  void add_chat(std::string from, std::string to, std::string chat){
    if(!_chat_map.contains(to))
      _chat_map[to] = std::unordered_map<std::string,ChatMsg>();
    if(!_chat_map[to].contains(from))
      _chat_map[to][from] = ChatMsg();
    _chat_map[to][from].messages.push_back(chat);
  }

  std::string chats_to_bytes(std::string to){
    std::string str;
    std::unordered_map<std::string, ChatMsg> to_chats = _chat_map[to];
    for(auto& it: to_chats){
      str += "From : " + it.first + "\n";
      for(auto &msg : it.second.messages){
	str += "- " + msg + "\n";
      }
    }
    return str;
  }

  void new_chat(std::string destination, std::string chat){
    const std::lock_guard<std::mutex> lock(chat_mutex);    
    if(!_chats.contains(destination))
      _chats[destination] = std::list<std::string>();
    
    _chats[destination].push_back(chat);
  }

  std::string fetch_chats(std::string user){
    std::string chats;
    if(_chats.contains(user)){
      for(std::string chat : _chats[user])
	chats += chat + "\n";
    }
    return chats;
  }
  
  int connection_count(){
    const std::lock_guard<std::mutex> lock(conn_mutex);
    return _count;
  }
  
  void add_connection(std::string user){
    const std::lock_guard<std::mutex> lock(conn_mutex);
    connections.push_front(user);
    userindex[user] = connections.begin();
    _count++;
  }
  
  void remove_connection(std::string user){
    const std::lock_guard<std::mutex> lock(conn_mutex);
    connections.erase(userindex[user]);
    userindex.erase(user);
    _count--;
    printf("user %s has disconnected\n",user.c_str());
  }
  bool is_connected(std::string user){
    const std::lock_guard<std::mutex> lock(conn_mutex);
    return userindex.contains(user);
  }

  std::string userlist(){
    const std::lock_guard<std::mutex> lock(conn_mutex);
    std::string msg;
    int counter = 1;
    for(auto i = connections.begin(); i != connections.end(); i++){
      msg += std::to_string(counter++);
      msg += ".";
      msg += *i;
      msg += "\n";
    }
    return msg;
  }
};
