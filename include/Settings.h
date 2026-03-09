#pragma once
#include <string>
#include <string_view>

class Settings
{
public:
    static Settings* GetSingleton();
    void             LoadSettings();
    void             LoadForms();

    RE::SpellItem* IsAttackingSpell                    = nullptr;
    RE::SpellItem* IsBlockingSpell                     = nullptr;
    RE::SpellItem* BowStaminaSpell                     = nullptr;
    RE::SpellItem* JumpSpell                           = nullptr;
    int         maxFrameCheck = 6;
    std::string FileName;

    static RE::FormID ParseFormID(const std::string& str);

    static RE::TESFile* LookupLoadedModByName(std::string_view a_modName)
    {
        for (auto& file : RE::TESDataHandler::GetSingleton()->compiledFileCollection.files) {
            if (a_modName.size() == strlen(file->fileName) && _strnicmp(file->fileName, a_modName.data(), a_modName.size()) == 0) {
                return file;
            }
        }
        return nullptr;
    }

    static const RE::TESFile* LookupLoadedLightModByName(std::string_view a_modName)
    {
        for (auto& smallFile : RE::TESDataHandler::GetSingleton()->compiledFileCollection.smallFiles) {
            if (a_modName.size() == strlen(smallFile->fileName) && _strnicmp(smallFile->fileName, a_modName.data(), a_modName.size()) == 0) {
                return smallFile;
            }
        }
        return nullptr;
    }

    template <typename T>
    T* LoadFormPointerFromIni(const std::string& formId, const std::string& filename)
    {
        if (formId.empty() || filename.empty()) {
            return nullptr;
        }
        const auto objectFormId = ParseFormID(formId);
        auto*      form         = RE::TESDataHandler::GetSingleton()->LookupForm(objectFormId, filename);
        return form ? form->As<T>() : nullptr;
    }
};
