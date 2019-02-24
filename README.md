# FTP-Server-and-Client

This is a simple implementation of passive mode ftp (File Transfer protocol) Server-Client.

User-id - inro
Password - joker

In passive mode FTP the client initiates both connections to the server, solving the problem of firewalls filtering the incoming data port connection to the client from the server. When opening an FTP connection, the client opens two random unprivileged ports locally (N > 1023 and N+1). The first port contacts the server on port 21, but instead of then issuing a PORT command and allowing the server to connect back to its data port, the client will issue the PASV command. The result of this is that the server then opens a random unprivileged port (P > 1023) and sends P back to the client in response to the PASV command. The client then initiates the connection from port N+1 to port P on the server to transfer data.

From the server-side firewall's standpoint, to support passive mode FTP the following communication channels need to be opened:

FTP server's port 21 from anywhere (Client initiates connection)
FTP server's port 21 to ports > 1023 (Server responds to client's control port)
FTP server's ports > 1023 from anywhere (Client initiates data connection to random port specified by server)
FTP server's ports > 1023 to remote ports > 1023 (Server sends ACKs (and data) to client's data port)

In step 1, the client contacts the server on the command port and issues the PASV command. The server then replies in step 2 with PORT 2024, telling the client which port it is listening to for the data connection. In step 3 the client then initiates the data connection from its data port to the specified server data port. Finally, the server sends back an ACK in step 4 to the client's data port.

Reference - http://slacksite.com/other/ftp.html#passive
