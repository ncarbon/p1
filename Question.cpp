#include "Question.hpp"

Question::Question(int num, std::string tag, std::string text, std::vector<std::string> choices, char correct_answer) {
    this->num = num;
    this->tag = tag;
    this->text = text;
    this->correct_answer = correct_answer;
    this->choices = choices;
}

int Question::getNum() {
    return this->num;
}

std::string Question::getTag() {
    return this->tag;
}

std::string Question::getText() {
    return this->text;
}

char Question::getCorrectAnswer() {
    return this->correct_answer;
}

std::vector<std::string> Question::getChoices() {
    return this->choices;
}


bool Question::checkAnswer(char answer) {
    return this->correct_answer == answer;
}

std::string Question::toString() {
    std::string choices_str;
    for(int i = 0; i < int(choices.size()); i++) {
        choices_str += choices[i];
        choices_str += "\n";
    }
    return "\n" + std::to_string(num) + "\n" +  tag + "\n" + text + "\n" + choices_str + correct_answer;
}

