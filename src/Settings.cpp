#include "Settings.h"
#include "Cache.h"
#include <SimpleIni.h>
#include <sstream>

Settings* Settings::GetSingleton()
{
    static Settings settings;
    return &settings;
}

void Settings::LoadSettings()
{
    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(R"(.\Data\SKSE\Plugins\Requiem.ini)");

    std::string fileName(ini.GetValue("", "sModFileName", ""));
    FileName = fileName;
}

RE::FormID Settings::ParseFormID(const std::string& str)
{
    RE::FormID         result;
    std::istringstream ss{ str };
    ss >> std::hex >> result;
    return result;
}

void Settings::LoadForms()
{
    logger::info("Loading essential forms...");
    auto        dataHandler = RE::TESDataHandler::GetSingleton();
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(R"(.\Data\SKSE\Plugins\Requiem.ini)");

    // ==========================================
    // 1. 保留的原版核心状态
    // ==========================================
    std::string attackingSpellFormID((ini.GetValue("", "IsAttackingSpellFormId", "")));
    IsAttackingSpell = LoadFormPointerFromIni<RE::SpellItem>(attackingSpellFormID, FileName);

    std::string blockingSpellFormID((ini.GetValue("", "IsBlockingSpellFormId", "")));
    IsBlockingSpell = LoadFormPointerFromIni<RE::SpellItem>(blockingSpellFormID, FileName);

    std::string bowStaminaSpellFormID((ini.GetValue("", "BowStaminaSpellFormId", "")));
    BowStaminaSpell = LoadFormPointerFromIni<RE::SpellItem>(bowStaminaSpellFormID, FileName);

    // ==========================================
    // 2. 添加你的全新状态 (对应 INI 截图)
    // ==========================================
    std::string runningSpellFormId((ini.GetValue("", "IsRunningSpellFormid", "")));
    IsRunningSpell = LoadFormPointerFromIni<RE::SpellItem>(runningSpellFormId, FileName);

    std::string jumpSpellFormID((ini.GetValue("", "JumpSpellFormID", "")));
    JumpSpell = LoadFormPointerFromIni<RE::SpellItem>(jumpSpellFormID, FileName);

    std::string swimmingSpellFormID((ini.GetValue("", "SwimmingSpellFormId", "")));
    IsSwimmingSpell = LoadFormPointerFromIni<RE::SpellItem>(swimmingSpellFormID, FileName);

    std::string untrainedHeavyArmorSpellFormID((ini.GetValue("", "UntrainedHeavyArmorSpellFormid", "")));
    UntrainedHeavyArmorSpell = LoadFormPointerFromIni<RE::SpellItem>(untrainedHeavyArmorSpellFormID, FileName);

    std::string isSprintingUntrainedHeavyArmorSpellFormID((ini.GetValue("", "IsSprintingUntrainedHeavyArmorSpellFormId", "")));
    IsSprintingUntrainedHeavyArmorSpell = LoadFormPointerFromIni<RE::SpellItem>(isSprintingUntrainedHeavyArmorSpellFormID, FileName);


    logger::info("Essential forms loaded successfully.");
}
