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

    steper.Add(utility::MakeStepExcutor([] (utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
    }));

    steper.Add(utility::MakeStepExcutor([] (utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return true;
    }));

    steper.Add(utility::MakeStepExcutor([] (utility::StepStation& guard) {
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

    steper.Add([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        for (int i = 0; i < 10; ++i)
        {
            guard.SubStep([i] { printf("SubStep %d\n", i); });
        }
    });

    steper.Add([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");

        guard.SubStep([](utility::StepStation& station) {
            station.Guard([] { printf("StationGuard\n"); });
        });

        return true;
    });

    steper.Add([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    });

    utility::StepEnd(&steper);
}
