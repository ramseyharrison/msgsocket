#include "message.h"

std::string Message::str(){
  std::string str;
  str.push_back('0' + (static_cast<int>(_t)));
  std::string size_str = std::to_string(_size);
  //4 bytes for the size of the message
  size_str.insert(size_str.begin(), 4 - size_str.length(),'0');
  str += size_str;
  str += _msg_data;
  return str;
}

Message Message::from_string(std::string message){
  std::vector<unsigned char> data(message.begin(), message.end());
  std::string size(data.begin() + 1, data.begin() + 5);
  std::string msg(data.begin() + 5, data.end());
  return Message {int_to_type(data.at(0) - 48), msg};
}


MessageType Message::int_to_type(int t){
  switch(t){
  case 0:
    return MessageType::Online;
  case 1:
    return MessageType::Offline;
  default:
    return MessageType::Error;
  }
}
