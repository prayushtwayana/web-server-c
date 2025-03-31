C compound literals
------------------------
unnamed object of specified type.
*Example: `(int) {69}`*


pointers on how it works
--------------------------
- the server should specify the network interface it will be listening on. (BROADCAST, MULTICAST, LOOPBACK, NO CARRIER, UP, LOWER_UP)
  (*what is a network interface? why does a device has many? how does docker become a network interface?*)

- to connect to a  remote address, private IPs don't work

(*HTTP built on top of TCP/IP*)
browsers are HTTP clients. they send HTTP requests. they send requests from an ephemeral port.
servers listen for HTTP requests on port 80.
a HTTP `request` is a connection that between a server and a client.
a server is simply a socket listening for a connection.


whiteboard
------------
server reads N bytes to the buffer.
how to read efficiently:
    - even if the capacity of the buffer is N, read K bytes at a time
conditions:
    - client sends more than the buffer size (*invalid, no way to actually store the entirety of the message*)
    - client sends less than the buffer size (**)


edge cases
------------
- server close the connection but not the client
- 
