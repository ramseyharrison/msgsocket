#include <vector>
#include <unordered_map>
#include <list>
#include <mutex>
class Server{
 private:
  std::list<std::string> connections;  
  std::unordered_map<std::string,std::list<std::string>::iterator> userindex;
  int _count;
  std::mutex conn_lock;
  
 public:
  int connection_count(){
    conn_lock.lock();
    return _count;
    conn_lock.unlock();
  }
  
  void add_connection(std::string user){
    conn_lock.lock();
    connections.push_front(user);
    userindex[user] = connections.begin();
    _count++;
    conn_lock.unlock();
  }
  
  void remove_connection(std::string user){
    conn_lock.lock();
    connections.erase(userindex[user]);
    userindex.erase(user);
    _count--;
    conn_lock.unlock();
  }
  bool is_connected(std::string user){
    conn_lock.lock();
    return userindex.contains(user);
    conn_lock.unlock();
  }
  
};
