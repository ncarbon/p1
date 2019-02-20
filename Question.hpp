#include <stdlib.h>
#include <string>
#include <vector>

class Question {
    public:
        Question(int num, std::string tag,  std::string text, std::vector<std::string> choices, char correct_answer);
        std::string getTag();
        std::string getText();
        char getCorrectAnswer();
        int getNum();
        std::vector<std::string> getChoices();
        bool checkAnswer(char answer);
        std::string toString();
    private:
        int num;
        std::string tag;
        std::string text;
        std::vector<std::string> choices;
        char correct_answer;
};