#include "QuestionBank.hpp"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>

QuestionBank::QuestionBank() {
}

Question* QuestionBank::getQuestion(int num) {
    for(int i = 0; i < int(questions.size()); i++) {
        if(questions[i]->getNum() == num) {
            return questions[i];
        }
    }

    return NULL;
}

std::vector<Question*> QuestionBank::getQuestions() {
    return this->questions;
}

int QuestionBank::getLastNum() {
    return this->lastNum;
}

int QuestionBank::loadQuestionBankVector() {
    std::ifstream inFile;
    inFile.open("qbank.txt");

    if (!inFile) {
        std::cerr << "Error: Could not open bank text file." << std::endl;
        return -1;   
    }
    
    // read from stream
    char data[1024];
    inFile.getline(data, 1024);
    this->lastNum = atoi(data);
    

    inFile.getline(data, 1024);

    while(inFile) {
        char correct_answer;
        int q_num = atoi(data);
        inFile.getline(data, 1024);
        std::string q_tag = std::string(data);
        std::string q_text = "";
        std::vector<std::string> choices;

        inFile.getline(data, 1024);

        while(strcmp(data, ".") != 0) {
            q_text += std::string(data);
            inFile.getline(data, 1024);
        }
        
        inFile.getline(data, 1024);

         while(strcmp(data, "%%") != 0) {
            std::string ch = ""; // question
           while(strcmp(data, ".") != 0) {
               ch += std::string(data);
               inFile.getline(data, 1024);
           }

           choices.push_back(ch);

            inFile.getline(data, 1024);
            if(strcmp(data, "#") == 0) { // end of answer choices
                inFile.getline(data, 1024);
                correct_answer = data[0];
                inFile.getline(data, 1024);
            }
            
        }
        
        this->questions.push_back(new Question(q_num, q_tag, q_text, choices, correct_answer));
        inFile.getline(data, 1024);
    }

    inFile.close();
    
    return 1;
}

int QuestionBank::loadQuestionBankFile() {
    std::ofstream file;
    file.open ("qbank.txt");

    if(!file) {std::cerr << "Error opening bank text file." << std::endl; return -1;}
    
    file << std::to_string(this->lastNum);
    file << "\n";

    for(int i = 0; i < int(questions.size()); i++) {
        file << std::to_string(questions[i]->getNum()) << "\n";
        file << questions[i]->getTag();
        file << "\n";
        file << questions[i]->getText();
        file << "\n.\n";
        // add choices
        std::vector<std::string> answerChoices = questions[i]->getChoices();
        for(int i = 0; i < int(answerChoices.size()); i++) {
            file << answerChoices[i] << "\n";
            file << ".\n";
        }

        file << "#\n"; // end of answer choices
        file << questions[i]->getCorrectAnswer();
        file << "\n";
        file << "%%\n";
    }
    file.close();

    return 0;
}

void QuestionBank::addQuestion(Question* newQuestion) {
    questions.push_back(newQuestion);
}

int QuestionBank::deleteQuestion(int num) {
    for(int i = 0; i < int(questions.size()); i++) {
        if(questions[i]->getNum() == num) {
            questions.erase(questions.begin() + i);
            return 0;
        }
    }

    return -1;
}

int QuestionBank::incrementLastNum() {
    return ++lastNum;
}

int QuestionBank::getSize() {
    return int(questions.size());
}
