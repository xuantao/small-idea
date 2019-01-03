#include "test_util.h"
#include "utility/step_excutor.h"

void test_step_excutor()
{
    utility::MakeStepExcutor([] {
    });

    utility::MakeStepExcutor([] {
        return true;
    });

    utility::MakeStepExcutor([] {
        return utility::STEP_STATUS::Completed;
    });

    utility::MakeStepExcutor([] (utility::StepGuard& guard) {
    });

    //utility::MakeStepExcutor([] (utility::StepGuard& guard) {
    //    return true;
    //});

    utility::MakeStepExcutor([] (utility::StepGuard& guard) {
        return utility::STEP_STATUS::Completed;
    });
}
