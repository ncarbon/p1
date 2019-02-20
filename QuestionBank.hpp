
#include "Question.hpp"

class QuestionBank {
    public:
        QuestionBank();
        Question* getQuestion(int num);
        int loadQuestionBankVector();
        int loadQuestionBankFile();
        std::vector<Question*> getQuestions();
        int getLastNum();
        int getSize();
        int incrementLastNum();
        void addQuestion(Question* newQuestion);
        int deleteQuestion(int num);
    private:
        int lastNum;
        std::vector<Question*> questions;
};