#pragma once

#include "Conditions.h"
#include "Hooks.h"

class UpdateManager
{
public:
    inline static int frameCount;

    inline static bool Install()
    {
        auto& trampoline = SKSE::GetTrampoline();
        _OnFrameFunction = trampoline.write_call<5>(Hooks::OnFrame_Update_Hook.address(), OnFrameUpdate);

        UpdateManager::frameCount = 0;
        logger::info("Installed hook for frame update");
        return true;
    }

private:
    static void TryAddSpell(RE::PlayerCharacter* player, RE::SpellItem* spell)
    {
        if (!player || !spell) {
            return;
        }
        if (!Conditions::HasSpell(player, spell)) {
            player->AddSpell(spell);
        }
    }

    static void TryRemoveSpell(RE::PlayerCharacter* player, RE::SpellItem* spell)
    {
        if (!player || !spell) {
            return;
        }
        if (Conditions::HasSpell(player, spell)) {
            player->RemoveSpell(spell);
        }
    }

    inline static std::int32_t OnFrameUpdate(std::int64_t a1)
    {
        auto* player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->Is3DLoaded()) {
            return _OnFrameFunction(a1);
        }

        auto* settings = Settings::GetSingleton();
        if (!settings) {
            return _OnFrameFunction(a1);
        }

        if (UpdateManager::frameCount > settings->maxFrameCheck) {
            UpdateManager::frameCount = 0;
        }

        auto* playerCamera = RE::PlayerCamera::GetSingleton();

        if (player->IsGodMode()) {
            TryRemoveSpell(player, settings->BowStaminaSpell);
            TryRemoveSpell(player, settings->IsAttackingSpell);
            TryRemoveSpell(player, settings->IsBlockingSpell);
            TryRemoveSpell(player, settings->JumpSpell);
        }
        else {
            switch (UpdateManager::frameCount) {
            case 1:
                if (playerCamera && IsBowDrawNoZoomCheck(player, playerCamera)) {
                    TryAddSpell(player, settings->BowStaminaSpell);
                }
                else {
                    TryRemoveSpell(player, settings->BowStaminaSpell);
                }
                break;
            case 2:
                if (Conditions::IsAttacking(player)) {
                    TryAddSpell(player, settings->IsAttackingSpell);
                }
                else {
                    TryRemoveSpell(player, settings->IsAttackingSpell);

                    if (Conditions::IsBlocking(player)) {
                        TryAddSpell(player, settings->IsBlockingSpell);
                    }
                    else {
                        TryRemoveSpell(player, settings->IsBlockingSpell);
                    }
                }
                break;
            case 3: {
                bool isJumping = false;
                bool success   = player->GetGraphVariableBool("bInJumpState", isJumping);
                if ((success && !isJumping) || !success) {
                    TryRemoveSpell(player, settings->JumpSpell);
                }
                break;
            }
            default:
                break;
            }
        }

        UpdateManager::frameCount++;
        return _OnFrameFunction(a1);
    }

    inline static REL::Relocation<decltype(OnFrameUpdate)> _OnFrameFunction;

    static bool IsBowDrawNoZoomCheck(RE::PlayerCharacter* player, RE::PlayerCamera* playerCamera)
    {
        if (!player || !playerCamera) {
            return false;
        }

        auto attackState = player->AsActorState()->GetAttackState();
        if (playerCamera->bowZoomedIn) {
            return false;
        }

        auto equippedWeapon = skyrim_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(false));
        if (!equippedWeapon) {
            return false;
        }

        switch (attackState) {
        case RE::ATTACK_STATE_ENUM::kBowDrawn:
        case RE::ATTACK_STATE_ENUM::kBowAttached:
            return equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow;
        default:
            return false;
        }
    }
};
