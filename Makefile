all: qserver.cpp Server.cpp QuestionBank.cpp Question.cpp qclient.cpp Client.cpp ;
	g++ -o qserver Question.cpp QuestionBank.cpp Server.cpp qserver.cpp ;
	g++ -o qclient qclient.cpp Client.cpp