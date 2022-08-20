#pragma once

#include "custom-types/shared/macros.hpp"
#include "ModSettingsInfos.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, SettingsHost, Il2CppObject,
    DECLARE_INSTANCE_FIELD(List<Il2CppObject*>*, _settings);
    DECLARE_INSTANCE_METHOD(ListWrapper<Il2CppObject*>, get_settings);
    DECLARE_INSTANCE_METHOD(bool, get_anySettings);

    public:
        static SettingsHost* get_instance();
        static SafePtr<SettingsHost> instance;

        void AddSubSetting(int index);
        DECLARE_DEFAULT_CTOR();

)

DECLARE_CLASS_CODEGEN(QuestUI, SubSettingsHost, Il2CppObject,
    DECLARE_INSTANCE_FIELD(int, index);
    DECLARE_INSTANCE_METHOD(StringW, get_title);
    DECLARE_INSTANCE_METHOD(StringW, get_message);
    DECLARE_INSTANCE_METHOD(void, Open);
    public:
        DECLARE_CTOR(ctor, int index);
)