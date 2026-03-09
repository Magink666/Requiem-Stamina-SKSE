#pragma once

#include "Hooks.h"
#include <Conditions.h>

using namespace Conditions;

class AnimationGraphEventHandler : public RE::BSTEventSink<RE::BSAnimationGraphEvent>,
                                   public RE::BSTEventSink<RE::TESObjectLoadedEvent>,
                                   public RE::BSTEventSink<RE::TESSwitchRaceCompleteEvent>
{
public:
    static AnimationGraphEventHandler* GetSingleton()
    {
        static AnimationGraphEventHandler singleton;
        return &singleton;
    }

      const char* jumpAnimEventString = "JumpUp";

    //Anims
    RE::BSEventNotifyControl ProcessEvent(const RE::BSAnimationGraphEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!a_event->tag.empty() && a_event->holder && a_event->holder->As<RE::Actor>()) {
            if (std::strcmp(a_event->tag.c_str(), jumpAnimEventString) == 0) {

                HandleJumpAnim();
            }
        }
        
        return RE::BSEventNotifyControl::kContinue;
    }

    void HandleJumpAnim()
    {
        auto settings = Settings::GetSingleton();
        auto player   = RE::PlayerCharacter::GetSingleton();
        if (!player->IsGodMode()) {
            Conditions::ApplySpell(player, player, settings->JumpSpell);
        }
    }


    RE::BSEventNotifyControl ProcessEvent(const RE::TESObjectLoadedEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_eventSource) override
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto actor = RE::TESForm::LookupByID<RE::Actor>(a_event->formID);
        if (!actor || !actor->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }


        actor->AddAnimationGraphEventSink(AnimationGraphEventHandler::GetSingleton());

        return RE::BSEventNotifyControl::kContinue;
    }


    RE::BSEventNotifyControl ProcessEvent(const RE::TESSwitchRaceCompleteEvent*                                a_event,
                                          [[maybe_unused]] RE::BSTEventSource<RE::TESSwitchRaceCompleteEvent>* a_eventSource) override
    {
        if (!a_event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto actor = a_event->subject->As<RE::Actor>();
        if (!actor || !actor->IsPlayerRef()) {
            return RE::BSEventNotifyControl::kContinue;
        }


        actor->AddAnimationGraphEventSink(AnimationGraphEventHandler::GetSingleton());

        return RE::BSEventNotifyControl::kContinue;
    }


    static void Register()
    {

        RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
        if (eventHolder) {
            eventHolder->AddEventSink<RE::TESObjectLoadedEvent>(GetSingleton());
            eventHolder->AddEventSink<RE::TESSwitchRaceCompleteEvent>(GetSingleton());
        }
    }
};

