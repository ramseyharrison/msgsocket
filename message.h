#include <vector>
#include <string>
#include <string.h>

enum class MessageType {
  Online,
  Offline,
  Error
};


struct Message{
private:
  MessageType _t;
  std::size_t _size;
  std::string _msg_data; 
  
public:
  Message(MessageType t, std::string data):
    _t(t)    
    , _msg_data(data)
    , _size(data.length()) {}

  Message() : _size(0) {}

  std::string str();
  
  static Message from_string(std::string);
  static MessageType int_to_type(int);
  std::size_t getSize(){
    return _size;
  }

  MessageType getType(){
    return _t;
  }

  std::string getMsg(){
    return _msg_data;
  }

};


  
