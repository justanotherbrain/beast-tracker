#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>

int main(){
	int status;
	struct addrinfo host_info;
	struct addrinfo *host_info_list;
	
	memset(&host_info, 0, sizeof host_info);

	std::cout << "Setting up the structs..." << std::endl;

	host_info.ai_family = AF_UNSPEC;
	host_info.ai_socktype = SOCK_STREAM;
	host_info.ai_flags = AI_PASSIVE;

	status = getaddrinfo(NULL, "5555", &host_info, &host_info_list);
	if (status != 0) std::cout << "getaddrinfo error" << gai_strerror(status);
	
	std::cout << "Creating a socket..." << std::endl;
	int socketfd;
	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (socketfd == -1) std::cout << std::endl;

	std::cout << "Binding socket..." << std::endl;
	int yes = 1;
	status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	status = bind(socketfd, host_info_list->ai_addr,host_info_list->ai_addrlen);
	if (status == -1) std::cout << "bind error" << std::endl;

	std::cout << "Listening for connections..." << std::endl;
	status = listen(socketfd,1);
	if (status == -1) std::cout << "listen error" << std::endl;

	int new_sd;
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);
	new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
	if (new_sd == -1){
		std::cout << "listen error" << std::endl;
	}
	else{
		std::cout << "Connection accepted. Using new socketfd: " << new_sd << std::endl;
	}

	ssize_t bytes_recieved;
	char incomming_data_buffer[100];
	
	for(;;){
		bytes_recieved = recv(new_sd, incomming_data_buffer,1000, 0);
		if (incomming_data_buffer[0] == 'q'){
			std::cout << "Eye-tracker closed" << std::endl;
			freeaddrinfo(host_info_list);
			shutdown(new_sd,2);
			shutdown(socketfd,2);
			return 0;
		}
		else{
			std::cout << incomming_data_buffer << std::endl;
		}
	}

}
