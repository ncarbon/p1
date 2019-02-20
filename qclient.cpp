#include "Client.hpp"

int putQuestion(Client &client, std::string &request, std::string &response) {
	char data[1024];
	std::cin.getline(data, 1024);
	std::cin.getline(data, 1024);
	std::string tag = data;
	std::string text;
	std::string choices;
	std::string correctAnswer;
	std::cin.getline(data, 1024);
	while(strcmp(data, ".") != 0) {
		text += std::string(data);
		std::cin.getline(data, 1024);
	}
	
	std::cin.getline(data, 1024);

	while(strcmp(data, ".") != 0) {
		std::string ch = ""; // question
		while(strcmp(data, ".") != 0) {
			ch += std::string(data);
			std::cin.getline(data, 1024);
		}

		choices += ch;
		choices += "\n";

		std::cin.getline(data, 1024);
	}
	std::cin.getline(data, 1024);
	correctAnswer = data;
	request = "p" + tag + "%%" + text + "%%" + choices + "%%" + correctAnswer;
	

	int ret = client.sendServer(request);
	if(ret == -1) {std::cout << "error sending to server: " << request << std::endl;return -1;}

	request = "";

	ret = client.readServer(response);
	if(ret == -1) {std::cout << "error reading from server: " << request << std::endl;return -1;}

	std::cout << "Response: " << response << std::endl;

	return ret;
}

int getQuestion(Client &client, std::string &request, std::string &response) {
	request = "g";
	std::string in;
	std::cin >> in;
	request += " ";
	request += in;
	int ret = client.sendServer(request);
	if(ret == -1) {return -1;}
	ret = client.readServer(response);
	std::cout << response << std::endl;
	
	return ret;
}

int checkAnswer(Client &client, std::string &request, std::string &response) {
	request = "c";
	std::string in;
	std::cin >> in;
	request += " ";
	request += in;
	std::cin >> in;
	request += " ";
	request += in;
	client.sendServer(request);
	client.readServer(response);
	std::cout << "response: " << response << std::endl;

	return 0;
}

void displayInstructions() {
	std::string instructions = "Enter a command: \np: put a question in the server.p followed by question tag,text, choices, and correct answer each in its own line.\nd <n>: delete a question from the server. d followed by <n> where <n> is the question number.\ng <n>: get question from bank. g followed by <n> where <n> is the question number.\nr: display a random question from the bank and provide the answer in a new line.\nc <n> <x>: check answer <x> for question number <n>\nk: kills/terminates the server\nq: terminate the client\nh: help\n";

	std::cout << instructions << std::endl;
}

int main(int argc, char const *argv[]) {
	int port;
	std::string hostname;

	if(argc < 3) {
		std::cerr << "Error: Please enter a port number followed by hostname" << std::endl;
		return -1;
	}

	port = atoi(argv[1]);
	hostname = argv[2];

    Client client;
	int res = client.startClient(port, hostname);
	
	std::string command;

	bool running = true;
	
	while(running) {
		std::cout << "> ";
		char cmd;
		std::cin >> cmd;
		std::string request = "";
		std::string response = "";
		switch(cmd) {
			case 'q': {
				running = false;
				break;
			}

			case 'k': {
				request = "k";
				client.sendServer(request);
				std::cout << "Terminating server..." << std::endl;
				running = false;
				break;
			}

			case 'p': {
				putQuestion(client, request, response);
				break;
			}

			case 'r': {
				int ret = client.randomQuestion();
				if(ret == -1) {std::cerr << "Error: Could not get random question. " << std::endl;}
				break;
			}

			case 'h': {
				displayInstructions();
				break;
			}

			case 'g': {
				getQuestion(client, request, response);
				break;
			}

			case 'd': {
				request = "d";
				std::string q_num;
				std::cin >> q_num;
				request += " " + q_num;
				client.sendServer(request);
				client.readServer(response);
				std::cout << response << std::endl;
				break;
			}
				
			case 'c' : {
				int ret = checkAnswer(client, request, response);
				if(ret == -1) {return -1;}
				break;
			}

			case 't' : {
				request = "t";
				client.sendServer(request);
				client.readServer(response);
				if(response == "ok") {
					std::cout << "Server Terminating..." << std::endl;
					break;
				} else {
					std::cout << "Server could not be terminated" << std::endl;
					break;
				}
			}

			default:
				std::cout << "Invalid command, try again." << std::endl;
				break;
		}
		request.clear();
		response.clear();
	}

	
	client.closeClient();
	std::cout << "Terminating client..." << std::endl;

	return 0;
}