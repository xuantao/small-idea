#include "test_util.h"
#include "utility/step_excutor.h"

void test_step_excutor()
{
    utility::QueueStepExcutor steper;

    steper.Add(utility::MakeStepExcutor([] {
        printf("[] () {}\n");
    }));

    steper.Add(utility::MakeStepExcutor([] {
        printf("[] () { return true; }\n");
        return true;
    }));

    steper.Add(utility::MakeStepExcutor([] {
        printf("[] () { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    }));

    steper.Add(utility::MakeStepExcutor([] (utility::StepGuard& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
    }));

    steper.Add(utility::MakeStepExcutor([] (utility::StepGuard& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return true;
    }));

    steper.Add(utility::MakeStepExcutor([] (utility::StepGuard& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    }));

    steper.Add([] {
        printf("[] () {}\n");
    });

    steper.Add([] {
        printf("[] () { return true; }\n");
        return true;
    });

    steper.Add([] {
        printf("[] () { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    });

    steper.Add([](utility::StepGuard& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
    });

    steper.Add([](utility::StepGuard& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return true;
    });

    steper.Add([](utility::StepGuard& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    });

    utility::StepEnd(&steper);
}
