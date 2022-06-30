#include "CustomTypes/Components/ProgressBar/ProgressBar.hpp"
#include "UnityEngine/GameObject.hpp"

#include "UnityEngine/WaitForSecondsRealtime.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Time.hpp"
#include "CustomTypes/Components/MainThreadScheduler.hpp"
#include "BeatSaberUI.hpp"
#include "HMUI/CurvedCanvasSettings.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"

DEFINE_TYPE(QuestUI, ProgressBar);



namespace QuestUI
{


    void ProgressBar::ShowMessage(StringW message, float time)
    {
        this->ShowMessage(message);
        StartCoroutine(custom_types::Helpers::CoroutineHelper::New(DisableCanvasRoutine(time)));
    }

    void ProgressBar::ShowMessage(StringW message)
    {
        StopAllCoroutines();
        if (!inited) return;
        showingMessage = true;
        headerText->set_text(message);
        //loadingBar->set_enabled(false);
        //loadingBackground->set_enabled(false);
        get_gameObject()->SetActive(true);
    }

    void ProgressBar::OnEnable()
    {

    }

    void ProgressBar::OnDisable()
    {
        if (!inited) return;
        loadingBar->set_fillAmount(0);
    }

    void ProgressBar::Awake()
    {

        
    }

    void ProgressBar::SetProgress(float progress, bool showBar)
    {
        if (!inited) return;
        loadingBar->set_fillAmount(progress < 0.0f ? 0.0f : (progress < 1.0f ? progress : 1.0f));  
        get_gameObject()->set_active(showBar);
    }

    void ProgressBar::SetProgress(float progress)
    {
        if (!inited) return;
        loadingBar->set_fillAmount(progress < 0.0f ? 0.0f : (progress < 1.0f ? progress : 1.0f));
        if(!get_gameObject()->get_active()) get_gameObject()->set_active(true);
    }

    void ProgressBar::Update()
    {
        if (!canvas || !canvas->get_enabled()) return;
        float pong = UnityEngine::Time::get_time() * 0.35f;

        float graph = std::fmod(pong, 2);
        if (graph > 1) graph = (2 - graph);
        auto color = UnityEngine::Color::HSVToRGB(graph,1,1);
        loadingBar->set_color(color);
        headerText->set_color(color);
    }

    void ProgressBar::SceneManagerOnActiveSceneChanged(UnityEngine::SceneManagement::Scene oldScene, UnityEngine::SceneManagement::Scene newScene)
    {
        if (!inited) return;
        get_gameObject()->SetActive(("MenuCore" == newScene.get_name()) && showingMessage);
    }

    custom_types::Helpers::Coroutine ProgressBar::DisableCanvasRoutine(float time)
    {
        static constexpr const float increment = 0.2f;
        float accumulation = 0.0f;
        while (accumulation < time)
        {
            SetProgress(accumulation / time);
            co_yield reinterpret_cast<System::Collections::IEnumerator*>(UnityEngine::WaitForSecondsRealtime::New_ctor(increment));
            accumulation += increment;
        }

        get_gameObject()->SetActive(false);
        showingMessage = false;
        co_return;
    }
}
