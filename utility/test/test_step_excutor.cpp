#include "test_util.h"
#include "utility/step_excutor.h"

static void test_step_excutor_station()
{
    utility::StepExcutorStation<> station;

    station.SubStep(utility::MakeStepExcutor([] {
        printf("[] () {}\n");
    }));

    station.SubStep(utility::MakeStepExcutor([] {
        printf("[] () { return true; }\n");
        return true;
    }));

    station.SubStep(utility::MakeStepExcutor([] {
        printf("[] () { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    }));

    station.SubStep(utility::MakeStepExcutor([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
    }));

    station.SubStep(utility::MakeStepExcutor([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return true;
    }));

    station.SubStep(utility::MakeStepExcutor([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    }));

    station.SubStep([] {
        printf("[] () {}\n");
    });

    station.SubStep([] {
        printf("[] () { return true; }\n");
        return true;
    });

    station.SubStep([] {
        printf("[] () { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    });

    station.SubStep([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { }\n");
        for (int i = 0; i < 10; ++i)
        {
            guard.SubStep([i] { printf("SubStep %d\n", i); });
            guard.Guard([i] { printf("SubStep %d\n", i); });
        }
    });

    station.SubStep([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { return true; }\n");

        guard.SubStep([](utility::StepStation& station) {
            station.Guard([] { printf("test substep guard\n"); });
            printf("test substep guard\n");
            return false;
        });

        return true;
    });

    station.SubStep([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    });

    if (utility::StepEnd(&station) == utility::STEP_STATUS::Failed)
        station.Rollback();
}

static void test_queue_step_excutor()
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
        printf("[] (StepGuard& guard) { }\n");
        for (int i = 0; i < 10; ++i)
        {
            guard.SubStep([i] { printf("SubStep %d\n", i); });
            guard.Guard([i] { printf("SubStep %d\n", i); });
        }
    });

    steper.Add([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { return true; }\n");

        guard.SubStep([](utility::StepStation& station) {
            station.Guard([] { printf("test substep guard\n"); });
            printf("test substep guard\n");
            return false;
        });

        return true;
    });

    steper.Add([](utility::StepStation& guard) {
        printf("[] (StepGuard& guard) { utility::STEP_STATUS::Completed; }\n");
        return utility::STEP_STATUS::Completed;
    });

    if (utility::StepEnd(&steper) == utility::STEP_STATUS::Failed)
        steper.Rollback();
}

static void test_parallel_step_excutor()
{
    utility::ParallelStepExcutor paral;

    paral.AddStep(utility::MakeStepExcutor([] {
        printf("paralle step 1\n");
    }));

    paral.AddStep(utility::MakeStepExcutor([] (utility::StepStation station) {
        printf("paralle step 2\n");
        station.SubStep([] {
            printf("paralle step 4\n");
        });
    }));

    paral.AddStep(utility::MakeStepExcutor([] {
        printf("paralle step 3\n");
    }));

    if (utility::StepEnd(&paral) == utility::STEP_STATUS::Failed)
        paral.Rollback();
}

void test_step_excutor()
{
    test_step_excutor_station();
    test_queue_step_excutor();
    test_parallel_step_excutor();
}
