#pragma once

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "GlobalNamespace/CustomPreviewBeatmapLevel.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/UI/Image.hpp"

#include "TMPro/TMP_Text.hpp"

#include <string_view>
#include <vector>

DECLARE_CLASS_CODEGEN(QuestUI, ProgressBar, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, OnEnable);
    DECLARE_INSTANCE_METHOD(void, OnDisable);
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Update);

        DECLARE_INSTANCE_FIELD(UnityEngine::Canvas*, canvas);
        DECLARE_INSTANCE_FIELD(TMPro::TMP_Text*, subText1);
        DECLARE_INSTANCE_FIELD(TMPro::TMP_Text*, subText2);
        DECLARE_INSTANCE_FIELD(TMPro::TMP_Text*, headerText);
        DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, loadingBackground);
        DECLARE_INSTANCE_FIELD(UnityEngine::UI::Image*, loadingBar);
        DECLARE_INSTANCE_FIELD(bool, inited);
    private:
        bool showingMessage = false;
    private:
        void SceneManagerOnActiveSceneChanged(UnityEngine::SceneManagement::Scene oldScene, UnityEngine::SceneManagement::Scene newScene);
        custom_types::Helpers::Coroutine DisableCanvasRoutine(float time);

    public:
        void ShowMessage(StringW, float time);
        void ShowMessage(StringW);
        void SetProgress(float progress);
        void SetProgress(float progress, bool showBar);

)