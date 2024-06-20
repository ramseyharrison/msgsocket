#include "message.h"

std::string Message::to_bytes(){
  std::string str;
  str.push_back('0' + (static_cast<int>(_t)));
  std::string size_str = std::to_string(_size);
  //4 bytes for the size of the message
  size_str.insert(size_str.begin(), 4 - size_str.length(),'0');
  str += size_str;
  str += _msg_data;
  return str;
}

std::string Message::str(){ //printing nice response, only called by client
  switch(_t){
  case MessageType::OnlineAwk :    
      return std::string("Connection Succesful\n");
  case MessageType::GetUsersResp:
  case MessageType::FetchChatResp:
    return getMsg();
  default:
    printf("error : user shouldn't be here\n");
    return NULL;
  }
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
    return MessageType::OnlineAwk;
  case 2:
    return MessageType::GetUsersReq;
  case 3:
    return MessageType::GetUsersResp;
  case 4:
    return MessageType::Chat;
  case 5:
    return MessageType::FetchChat;
  case 6:
    return MessageType::FetchChatResp;
  default:
    return MessageType::Error;
  }
}
