#pragma once
#include "Cache.h"
#include "Settings.h"


namespace Conditions
{

    static bool IsAttacking(RE::Actor* actor)
    {
        using func_t = decltype(&Conditions::IsAttacking);
        REL::Relocation<func_t> func{ Cache::IsAttackingAddress };
        return func(actor);
    }

    inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;


    static bool IsBlocking(RE::Actor* actor)
    {
        using func_t = decltype(&Conditions::IsBlocking);
        REL::Relocation<func_t> func{ Cache::IsBlockingAddress };
        return func(actor);
    }

    inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;


    static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
    {
        using func_t = decltype(&Conditions::HasSpell);
        REL::Relocation<func_t> func{ Cache::HasSpellAddress };
        return func(actor, spell);
    }

    inline static REL::Relocation<decltype(HasSpell)> _HasSpell;


	static bool IsMoving(RE::PlayerCharacter* player)
    {
        auto playerState = player->AsActorState();
        return (static_cast<bool>(playerState->actorState1.movingForward) || static_cast<bool>(playerState->actorState1.movingBack)
                || static_cast<bool>(playerState->actorState1.movingLeft) || static_cast<bool>(playerState->actorState1.movingRight));
    }


static bool IsRunning(RE::PlayerCharacter* player)
    {
        if (!player)
            return false;

        const auto* state = player->AsActorState();
        if (!state)
            return false;

        if (state->IsSprinting() || state->IsSwimming() || state->IsSneaking())
            return false;

        return player->IsRunning();
    }


    inline static bool PlayerHasActiveMagicEffect(RE::EffectSetting* a_effect)
    {
        auto player        = RE::PlayerCharacter::GetSingleton();
        auto activeEffects = player->AsMagicTarget()->GetActiveEffectList();
        if (!activeEffects)
            return false;

        for (auto& effect : *activeEffects) {
            auto setting = effect ? effect->GetBaseObject() : nullptr;
            if (setting && setting == a_effect) {
                return true;
            }
        }
        return false;
    }


    static bool IsPermanent(RE::MagicItem* item)
    {
        switch (item->GetSpellType()) {
        case RE::MagicSystem::SpellType::kDisease:
        case RE::MagicSystem::SpellType::kAbility:
        case RE::MagicSystem::SpellType::kAddiction:
            return true;
        default:
            return false;
        }
    }

    inline static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
    {
        if (!caster || !target || !spell)
            return;
        if (IsPermanent(spell)) {
            target->AddSpell(spell);
        }
        else {
            auto magicCaster = caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
            if (magicCaster) {
                magicCaster->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
            }
        }
    }
}; // namespace Conditions
