#include "test_util.h"
#include "step_excutor.h"
#include "async.h"
#include <thread>

using namespace utility;

static void test_normal_steps()
{
    //return;
    {
        StepExcutorStation<> station;
        auto ctrl = station.GetCtrl();
        ctrl.SubStep([] {
            printf("[] () {}\n");
        });

        ctrl.SubStep([] {
            printf("[] () { return true; }\n");
            return true;
        });

        ctrl.SubStep([] {
            printf("[] () { STEP_STATUS::Completed; }\n");
            return STEP_STATUS::Completed;
        });

        if (StepEnd(&station) == STEP_STATUS::Failed)
            station.Rollback();
    }

    {
        StepExcutorStation<> station;
        auto ctrl = station.GetCtrl();
        ctrl.SubStep([] {
            printf("[] () {}\n");
        });

        ctrl.SubStep([] {
            printf("[] () { return true; }\n");
            return true;
        });

        ctrl.SubStep([] {
            printf("[] () { STEP_STATUS::Completed; }\n");
            return STEP_STATUS::Completed;
        });

        ctrl.SubStep([] {
            printf("[] () { STEP_STATUS::Failed; }\n");
            return STEP_STATUS::Failed;
        });

        if (StepEnd(&station) == STEP_STATUS::Failed)
            station.Rollback();
    }
}

static void test_step_excutor_station()
{
    StepExcutorStation<> station;
    auto ctrl = station.GetCtrl();
    ctrl.SubStep(MakeStepExcutor([] {
        printf("[] () {}\n");
    }));

    ctrl.SubStep(MakeStepExcutor([] {
        printf("[] () { return true; }\n");
        return true;
    }));

    ctrl.SubStep(MakeStepExcutor([] {
        printf("[] () { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    }));

    ctrl.SubStep(MakeStepExcutor([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
    }));

    ctrl.SubStep(MakeStepExcutor([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
        return true;
    }));

    ctrl.SubStep(MakeStepExcutor([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    }));

    ctrl.SubStep([] {
        printf("[] () {}\n");
    });

    ctrl.SubStep([] {
        printf("[] () { return true; }\n");
        return true;
    });

    ctrl.SubStep([] {
        printf("[] () { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    });

    ctrl.SubStep([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { }\n");
        for (int i = 0; i < 10; ++i)
        {
            guard.SubStep([i] { printf("SubStep %d\n", i); });
            guard.Guard([i] { printf("SubStep %d\n", i); });
        }
    });

    ctrl.SubStep([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { return true; }\n");

        guard.SubStep([](StepCtrl station) {
            station.Guard([] { printf("test substep guard\n"); });
            printf("test substep guard\n");
            return false;
        });

        return true;
    });

    ctrl.SubStep([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    });

    if (StepEnd(&station) == STEP_STATUS::Failed)
        station.Rollback();
}

static void test_queue_step_excutor()
{
    StepExcutorStation<> station;
    auto steper = station.GetCtrl();

    steper.SubStep(MakeStepExcutor([] {
        printf("[] () {}\n");
    }));

    steper.SubStep(MakeStepExcutor([] {
        printf("[] () { return true; }\n");
        return true;
    }));

    steper.SubStep(MakeStepExcutor([] {
        printf("[] () { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    }));

    steper.SubStep(MakeStepExcutor([] (StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
    }));

    steper.SubStep(MakeStepExcutor([] (StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
        return true;
    }));

    steper.SubStep(MakeStepExcutor([] (StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    }));

    steper.SubStep([] {
        printf("[] () {}\n");
    });

    steper.SubStep([] {
        printf("[] () { return true; }\n");
        return true;
    });

    steper.SubStep([] {
        printf("[] () { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    });

    steper.SubStep([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { }\n");
        for (int i = 0; i < 10; ++i)
        {
            guard.SubStep([i] { printf("SubStep %d\n", i); });
            guard.Guard([i] { printf("SubStep %d\n", i); });
        }
    });

    steper.SubStep([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { return true; }\n");

        guard.SubStep([](StepCtrl station) {
            station.Guard([] { printf("test substep guard\n"); });
            printf("test substep guard\n");
            return false;
        });

        return true;
    });

    steper.SubStep([](StepCtrl guard) {
        printf("[] (StepGuard& guard) { STEP_STATUS::Completed; }\n");
        return STEP_STATUS::Completed;
    });

    if (StepEnd(&station) == STEP_STATUS::Failed)
        station.Rollback();
}

static void test_parallel_step_excutor()
{
    ParallelStepExcutor paral;

    paral.AddStep(MakeStepExcutor([] {
        printf("paralle step 1\n");
    }));

    paral.AddStep(MakeStepExcutor([] (StepCtrl station) {
        printf("paralle step 2\n");
        station.SubStep([] {
            printf("paralle step 4\n");
        });
        station.Guard([] { printf("111111111111 guard\n"); });
        return false;
    }));

    paral.AddStep(MakeStepExcutor([] {
        printf("paralle step 3\n");
    }));

    if (StepEnd(&paral) == STEP_STATUS::Failed)
        paral.Rollback();

    //auto f4 = Async::ExpectRun([] { std::this_thread::yield(); printf("asyn run 4\n"); });
    //station.SubStep(std::move(f4), [] { printf("step excutor 4\n"); });
}

static void test_step_async()
{
    StepExcutorStation<> station;
    Async::Startup(4);
    auto ctrl = station.GetCtrl();

    auto f1 = Async::Run([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        printf("asyn run 1\n");
    });
    ctrl.SubStep(std::move(f1), [] {
        printf("step excutor 1\n");
    });

    auto f2 = Async::Run([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        printf("asyn run 2\n");
    });
    ctrl.SubStep(std::move(f2), [] {
        printf("step excutor 2\n");
    });

    auto f3 = Async::Run([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        printf("asyn run 3\n");
    }).Share();
    ctrl.SubStep(f3, [] {
        printf("step excutor 3\n");
    });

    int i = 0;
    ctrl.SubStep([i]() mutable {
        while (i < 2) // 不能使用 for, for的累加操作会被跳过
        {
            printf("step excutor 4 sub:{%d}\n", i++);
            return STEP_STATUS::Busy;
        }
        return STEP_STATUS::Completed;
    });

    if (StepEnd(&station) == STEP_STATUS::Failed)
        station.Rollback();
    Async::Shutdown();
}

void test_step_excutor()
{
    test_normal_steps();
    test_step_excutor_station();
    test_queue_step_excutor();
    test_parallel_step_excutor();
    test_step_async();
}
