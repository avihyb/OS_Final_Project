#include <iostream>
#include <SFML/Graphics.hpp>
#include "Graph.hpp"
#include "Tree.hpp"
#include "ActiveObject.hpp"
#include "Pipeline.hpp"
#include "MSTAlgorithmFactory.hpp"
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <thread>
#include "Pipeline.hpp"

using namespace std;

#define PORT "9034"

Graph graph;
Tree mstTree;
ActiveObject activeObject;

void showLoadingBar(int duration) {
    const int barWidth = 50;
    for (int i = 0; i <= barWidth; ++i) {
        std::cout << "[";
        int pos = barWidth * i / barWidth;
        for (int j = 0; j < barWidth; ++j) {
            if (j < pos) std::cout << "=";
            else if (j == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int((i / (float)barWidth) * 100.0) << " %\r";
        std::cout.flush();

        std::this_thread::sleep_for(std::chrono::milliseconds(duration / barWidth));
    }
    std::cout << std::endl;
}

void handleInit(int client_fd) {
    string response = "Enter edges in format: <from> <to> <weight>, -1 to finish.\n";
    send(client_fd, response.c_str(), response.size(), 0);
    cout << "Client " << client_fd << " used init command." << endl;
    
    while (true) {
        char buf[256];
        int nbytes = recv(client_fd, buf, sizeof buf, 0);
        if (nbytes <= 0) {
            cerr << "Error receiving data from client" << endl;
            break;
        }
        buf[nbytes] = '\0';
        
        int from, to, weight;
        sscanf(buf, "%d %d %d", &from, &to, &weight);
        if (from == -1) {
            response = "Successfully inputed all edges.\n";
            send(client_fd, response.c_str(), response.size(), 0);
            break;
        }
        graph.addEdge(from, to, weight);
    }
    cout << "Graph initialized." << endl;
}

void handleKruskal(int client_fd) {
    cout << "Client #" << client_fd << " requested Kruskal algorithm.\n";
    string response = "Searching MST with Kruskal algorithm...\n";
    send(client_fd, response.c_str(), response.size(), 0);

    // Create pipeline and add steps
    Pipeline pipeline;
    pipeline.addStep([client_fd] {
        mstTree = MSTAlgorithmFactory::createAlgorithm("Kruskal")->findMST(graph);
    });
    pipeline.addStep([client_fd] {
        string response = "MST found.\n";
        send(client_fd, response.c_str(), response.size(), 0);
    });

    // Enqueue the pipeline task
    activeObject.enqueueTask([pipeline] {
        pipeline.execute();
    });
}

void handlePrim(int client_fd) {
    cout << "Client #" << client_fd << " requested Prim algorithm.\n";
    string response = "Searching MST with Prim algorithm...\n";
    send(client_fd, response.c_str(), response.size(), 0);

    // Create pipeline and add steps
    Pipeline pipeline;
    pipeline.addStep([client_fd] {
        mstTree = MSTAlgorithmFactory::createAlgorithm("Prim")->findMST(graph);
    });
    pipeline.addStep([client_fd] {
        string response = "MST found.\n";
        send(client_fd, response.c_str(), response.size(), 0);
    });

    // Enqueue the pipeline task
    activeObject.enqueueTask([pipeline] {
        pipeline.execute();
    });
}

void handleShow(int client_fd) {
    cout << "Showing graph" << endl;
    string response = "Showing graph\n";
    send(client_fd, response.c_str(), response.size(), 0);

    // Create pipeline and add steps
    Pipeline pipeline;
    pipeline.addStep([] {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualization");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::White);
            graph.drawGraph(window, mstTree.getEdges());
            window.display();
        }
    });

    // Enqueue the pipeline task
    activeObject.enqueueTask([pipeline] {
        pipeline.execute();
    });
}

void HandleCommand(const string& command, int client_fd) {
    try {
        if (command.substr(0, 4) == "init") {
            activeObject.enqueueTask([client_fd] {
                handleInit(client_fd);
            });
        } else if (command.substr(0, 7) == "kruskal") {
            activeObject.enqueueTask([client_fd] {
                handleKruskal(client_fd);
            });
        } else if (command.substr(0, 4) == "prim") {
            activeObject.enqueueTask([client_fd] {
                handlePrim(client_fd);
            });
        } else if (command.substr(0, 4) == "show") {
            activeObject.enqueueTask([client_fd] {
                handleShow(client_fd);
            });
        } else {
            cout << "Unknown command" << endl;
            string response = "unknown command\n";
            send(client_fd, response.c_str(), response.size(), 0);
        }
    } catch (const std::exception& e) {
        cerr << "Error handling command: " << e.what() << endl;
        string response = "error with command";
        send(client_fd, response.c_str(), response.size(), 0);
    }
}

void menu(int clientfd) {
    
    // Construct the menu string
    std::string menu = "AVAILABLE CMDS:\n";
    menu += "1. init - to initialize a new graph";
       // Add graph status message
    if (!graph.isEmpty()) {
        menu += "\n(Graph is already initialized!)\n";
    } else {
        menu += "\n";
        menu += "2. prim - to run Prim's algorithm\n";
        menu += "3. kruskal - to run Kruskal's algorithm\n";
        menu += "4. show - open GUI to show the graph\n";
    }
    
    
    
 
    
    // Send the menu to the client
    send(clientfd, menu.c_str(), strlen(menu.c_str()), 0);
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
    std::cout << "Starting server, Please wait... " << std::endl;
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
    showLoadingBar(1000);

     std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Server started succesfully! Awaiting connections... " << std::endl;
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
                             
                             send(newfd, "Welcome!\n", 9, 0);
                             menu(newfd);
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
