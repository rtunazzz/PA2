#include "quiz.h"
#include "window.h"

int main(int argc, char *argv[]) {
    CQuiz<CSDLWindow> quiz;
    quiz.Start();

    return 0;
}
