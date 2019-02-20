#include "QuestionBank.hpp"
#include "Server.hpp"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

int deleteQuestion(Server &server, QuestionBank &questionBank, std::string &request, std::string &response) {
    std::vector<std::string> tokens = split(request, ' ');
    int ret = questionBank.deleteQuestion(atoi(tokens[1].c_str()));
    if(ret == -1) {
        response = "Question could not be deleted. Question does not exist. ";
        return -1;
    } 
    response = "Question successfully.";
    return server.sendClient(response);
}

int putQuestion(Server &server, std::string &request, std::string &response, QuestionBank &questionBank) {
    std::vector<std::string> questionInfo;
    std::string s = request.substr(1);
    std::string delimiter = "%%";
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        questionInfo.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    questionInfo.push_back(s.substr(0, 1));

    int num = questionBank.incrementLastNum();
    std::string tag = questionInfo[0];
    std::string text = questionInfo[1];
    std::vector<std::string> choices = split(questionInfo[2], '\n');
    std::string correctAnswer = questionInfo[3];

    Question* newQuestion = new Question(num, tag, text, choices, correctAnswer.at(0));
    questionBank.addQuestion(newQuestion);
    
    response = std::to_string(num);
    int ret = server.sendClient(response);

    return ret;
}

int getQuestion(Server &server, QuestionBank &questionBank, std::string &request, std::string &response) {
    std::vector<std::string> tokens = split(request, ' ');
    Question* question = questionBank.getQuestion(atoi(tokens[1].c_str()));
    if(question == NULL) {
        response = "Question " + tokens[1] + " not found.";
    } else {
        response = question->toString();
    }
    return server.sendClient(response);
}

int checkAnswer(Server &server, QuestionBank &questionBank, std::string &request, std::string &response) {
    std::vector<std::string> tokens = split(request, ' ');
    int num = atoi(tokens[1].c_str());
    char answer = tokens[2].at(0);
    Question* question = questionBank.getQuestion(num);
    if(question == nullptr) {
        std::cerr << "Error: question does not exist" << std::endl;
        response = "Error: question does not exist";
        server.sendClient(response);
        return -1;
    }
    if(question->checkAnswer(answer)) {
        response = "Correct!";
    } else {
        response = "Incorrect!";
    }
    return server.sendClient(response);
}

Question* randomQuestion(QuestionBank &questionBank, std::string &response) {
    int bankLen = int(questionBank.getSize());
    if(bankLen == 0) {
        fprintf(stderr, "Error: no questions in bank.\n");
        response = "Error: no questions in bank.\n";
        return NULL;
    }

    int randNum = rand() % (bankLen-1) + 0;
    std::vector<Question*> questions = questionBank.getQuestions();
    Question *randomQuestion = questions[randNum];
    response += "\n";
    response += std::to_string(randomQuestion->getNum());
    response += "\n";
    response += randomQuestion->getTag();
    response += "\n";
    response += randomQuestion->getText();
    response += "\n";
    std::vector<std::string> choices = randomQuestion->getChoices();
    for(int i = 0; i < int(choices.size()); i++) {
        response += choices[i];
        response += "\n";
    }
    return randomQuestion;
}

int randomQuestionResponse(QuestionBank &questionBank, std::string &request, std::string &response, Server &server) {
    Question *randQuestion = randomQuestion(questionBank, response);
    if(randQuestion == NULL) {
        std::cerr << "Error creating question" << std::endl;
        return -1;
    }
    int ret = server.sendClient(response);
    if(ret == -1) {return -1;}

    ret = server.readClient(request);
    if(ret == -1) {return -1;}

    if(randQuestion->getCorrectAnswer() == request.at(0)) {
        response = "Correct!";
    } else {
        response = "Incorrect!";
    }

    ret = server.sendClient(response);
    if(ret == -1) {return -1;}

    return ret;
}

int main() {
    Server server;
    int ret = server.createSocket();

    if(ret == -1) {
        fprintf(stderr, "Error: socket could not be created");
        return -1;
    }
    ret = server.bindSocket();
    if(ret == -1) {
        fprintf(stderr, "Error: could not bind socket to IP/PORT");
        return -1;
    }

    server.displayHostInfo();

    ret = server.listenToConnections();

    if(ret == -1) {
        std::cerr << "Error listening to connections" << std::endl; 
        return -1;
    }

    // load file content to vector
    std::vector<Question*> questionBankList;
    QuestionBank questionBank;
    ret = questionBank.loadQuestionBankVector();
    if(ret == -1) { std::cerr << "Error loading question bank" << std::endl; return -1;}
    
    bool running = true;

    while(running) {
        int client_socket = server.acceptConnection();
        std::cout << "New client connection" << std::endl;
        if(client_socket == -1) {std::cerr << "Error could not connect to client" << std::endl;break;}

        while(running) {
            std::string request = "";
            std::string response = "";

            int bytesRecvd = server.readClient(request);

            if(bytesRecvd == -1) {
                std::cerr << "Error reading from client" << std::endl;
                break;
            }

            if(bytesRecvd == 0) {
                std::cout << " The client disconnected " << std::endl;
                break;
            }

            switch(request.at(0)) {
                case 'k' :
                    running = false;
                    break;

                case 'p' :
                    putQuestion(server, request, response, questionBank);
                    break;

                case 'r':
                    std::cout << "random" << std::endl;
                    ret = randomQuestionResponse(questionBank, request, response, server);
                    if(ret == -1) {return -1;}
                    break;

                case 'c':
                    ret = checkAnswer(server, questionBank, request, response);
                    break;

                case 'g':
                    ret = getQuestion(server, questionBank, request, response);
                    if(ret == -1) {return -1;}
                    break;

                case 'd':
                    deleteQuestion(server, questionBank, request, response);
                    break;
                
                case 't':
                    response = "ok";
                    server.sendClient(response);
                    running = false;
                    break;

                default:
                std::cout << "REQUEST:  " << request << std::endl;
                    response = "Error: invalid request";
                    server.sendClient(response);
            }
        }
    }

    // update question bank
    questionBank.loadQuestionBankFile();

    std::cout << "closing client socket..." << std::endl;
    server.closeClientSocket();

    return 0;
        
}