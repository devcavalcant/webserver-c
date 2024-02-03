<h3 align="center">Web Server in C with sockets</h3>

The project consists of implementing a pure web server in C, using only standard libraries. The server has the ability to return a *fallback.html* page in cases where the page requested by the client is not found or does not exist. In addition, the server has the ability to find various web pages that are desired and, in cases where a specific page is not requested (i.e. searching via the "/" route), the *index.html* page is returned.

A web server is one of a number of services on the web, ranging from simple to complex applications. To communicate these services, a client-server relationship is used, which works in a more centralized way (unlike p2p). The server works reactively to external interactions, so the first communication is usually made by the client, where the communication begins (*3way handshake*).

#### Motivation

The study of a web server is often seen and applied in a high-level and facilitated way, which helps developers and companies wishing to create a service. However, for a more significant and low-level study, the C language was chosen, known for its characteristic of being closer to the ideas of the hardware compared to others on the market.