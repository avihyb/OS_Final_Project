#include "../include/Graph.hpp"
#include "../include/KruskalMST.hpp"

#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <stack>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <thread>
#include <mutex>
#include <map> 

using namespace std;

#define PORT "9034"   // Port we're listening on

Graph graph;

void HandleCommand(const string& command, int client_fd){
    try {
        if(command.substr(0, 8) == "Newgraph"){
            int vertices = stoi(command.substr(9, command.find(' ', 9)));
            int edges = stoi(command.substr(command.find(' ', 9) + 1));
            graph.createGraph(vertices, edges);
        }
        
        else if(command == "Exit"){
            cout << "Client" << client_fd << "exited" << endl;
            send(client_fd, "Exit", 4, 0);
            close(client_fd);
        }
        else {
            cout << "Invalid command" << endl;
        }
    } catch (const exception& e) {
        cout << "Invalid command" << endl;
        send(client_fd, "Invalid command", 15, 0);
    }
}

void displayMenu(){
    cout << "1. Newgraph <vertices> <edges>" << endl;
    cout << "2. Exit" << endl;

}

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket() {
    int listener;     // Listening socket descriptor
    int yes = 1;      // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(vector<pollfd>& pfds, int newfd) {
    pfds.push_back({newfd, POLLIN, 0});
}

// Remove an index from the set
void del_from_pfds(vector<pollfd>& pfds, int i) {
    pfds.erase(pfds.begin() + i);
}

int main() {
    int listener;     // Listening socket descriptor

    int newfd;        // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char buf[256];    // Buffer for client data

    char remoteIP[INET6_ADDRSTRLEN];

    // Start off with room for 5 connections
    // (We'll realloc as necessary)
    vector<pollfd> pfds;
    pfds.reserve(5);

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1) {
        cerr << "error getting listening socket" << endl;
        exit(1);
    }

    // Add the listener to set
    pfds.push_back({listener, POLLIN, 0});

    // Main loop
    for (;;) {
        int poll_count = poll(pfds.data(), pfds.size(), -1);

        if (poll_count == -1) {
            perror("poll");
            exit(1);
        }

        // Run through the existing connections looking for data to read
        for (size_t i = 0; i < pfds.size(); i++) {
            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN) { // We got one!!
                if (pfds[i].fd == listener) {
                    // If listener is ready to read, handle new connection
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,
                        (struct sockaddr*)&remoteaddr,
                        &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        add_to_pfds(pfds, newfd);

                        cout << "pollserver: new connection from "
                             << inet_ntop(remoteaddr.ss_family,
                                          get_in_addr((struct sockaddr*)&remoteaddr),
                                          remoteIP, INET6_ADDRSTRLEN)
                             << " on socket " << newfd << endl;
                    }
                } else {
                    // If not the listener, we're just a regular client
                    int nbytes = recv(pfds[i].fd, buf, sizeof buf, 0);

                    int sender_fd = pfds[i].fd;

                    if (nbytes <= 0) {
                        // Got error or connection closed by client
                        if (nbytes == 0) {
                            // Connection closed
                            cout << "pollserver: socket " << sender_fd << " hung up" << endl;
                        } else {
                            perror("recv");
                        }

                        close(pfds[i].fd); // Bye!

                        del_from_pfds(pfds, i);

                    } else {
                        // We got some good data from a client
                        string command(buf, nbytes);
                        HandleCommand(command, sender_fd);
                    }
                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    return 0;
}