#include "utility/step_excutor.h"
#include <chrono>

UTILITY_NAMESPACE_BEGIN

STEP_STATUS StepFor(IStepExcutor* pSteper, size_t nDuration)
{
    if (pSteper == nullptr)
        return STEP_STATUS::Completed;

    STEP_STATUS eStatus;
    auto endTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(nDuration);
    do
    {
        eStatus = pSteper->Step();
    } while (eStatus == STEP_STATUS::Busy && endTime > std::chrono::high_resolution_clock::now());

    return eStatus;
}

STEP_STATUS StepEnd(IStepExcutor* pSteper)
{
    if (pSteper == nullptr)
        return STEP_STATUS::Completed;

    STEP_STATUS eStatus;
    do
    {
        eStatus = pSteper->Step();
    } while (!IS_STEP_STOPPED(eStatus));

    return eStatus;
}

//////////////////////////////////////////////////////////////////////////
// class ParallelStepExcutor
STEP_STATUS ParallelStepExcutor::Step()
{
    if (IsEmpty())
        return STEP_STATUS::Completed;

    STEP_STATUS eRet = STEP_STATUS::Completed;
    size_t nIndex = 0;
    size_t nBusy = 0;
    while (nIndex < steps_.size())
    {
        eRet = steps_[nIndex]->Step();
        switch (eRet)
        {
        case STEP_STATUS::Busy:
            ++nBusy;
        case STEP_STATUS::Idle:
            ++nIndex;
            break;
        case STEP_STATUS::Failed:
            steps_[nIndex]->Rollback();
        case STEP_STATUS::Completed:
            steps_.erase(steps_.begin() + nIndex);
            break;
        }
    }

    return nBusy ? STEP_STATUS::Busy : STEP_STATUS::Idle;
}

namespace StepExcutor_Internal
{
    //////////////////////////////////////////////////////////////////////////
    // class QueuedStepImpl
    void QueuedStepImpl::Push(StepExcutorPtr ptr)
    {
        void* mem = alloc_->Alloc(sizeof(StepNode));
        auto node = new (mem) StepNode(ptr);

        if (tail_)
            tail_->next_node = node;
        else
            head_ = node;

        tail_ = node;
    }

    void QueuedStepImpl::Clear()
    {
        if (!IsEmpty())
            Rollback(); // not success completed

        while (head_)
        {
            auto node = head_;
            head_ = head_->next_node;
            node->~StepNode();
        }

        while (step_)
        {
            auto node = step_;
            step_ = step_->next_node;
            node->~StepNode();
        }

        head_ = tail_ = step_ = nullptr;
    }

    STEP_STATUS QueuedStepImpl::Step()
    {
        if (head_ == nullptr)
            return STEP_STATUS::Completed;

        STEP_STATUS status = head_->val->Step();
        if (status == STEP_STATUS::Completed)
        {
            auto node = head_;
            head_ = head_->next_node;

            node->next_node = step_;
            step_ = node;

            return STEP_STATUS::Busy;
        }
        return status;
    }

    void QueuedStepImpl::Rollback()
    {
        if (head_)
            head_->val->Rollback();

        while (step_)
        {
            auto node = step_;
            step_ = step_->next_node;

            node->val->Rollback();
            node->next_node = head_;
            head_ = node;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // class StackedGuardImpl
    void StackedGuardImpl::Rollback()
    {
        while (head_)
        {
            GuardNode* node = head_;
            head_ = head_->next_node;

            node->val->Call();
            node->val->~ICallable<void()>();
            node->~GuardNode();
        }
    }

    void StackedGuardImpl::Clear()
    {
        while (head_)
        {
            GuardNode* node = head_;
            head_ = head_->next_node;

            node->val->~ICallable<void()>();
            node->~GuardNode();
        }
    }
} // StepExcutor_Internal

UTILITY_NAMESPACE_END
