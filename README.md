Multithreaded C++ chat server and app built on top of [this C++ TCP wrapper](https://github.com/plasma-umass/simplesocket).

Messages are passed through the server, in which they are stored (see ```server.h``` for details)

The syntax for sending a message is :
```
chat [username] [message, which can include spaces].
```
You can see your message inbox by using the ```chats``` command.

And see list of online users with ```users```.

Additionally, messages can be sent to users even when they are not connected. 
