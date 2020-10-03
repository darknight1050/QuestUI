#include <sstream>
#include <string>
#include <map>

#include "beatsaber-hook/shared/utils/utils.h"
#include "CustomTypes/Components/Backgroundable.hpp"
#include "ArrayUtil.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/HideFlags.hpp"

DEFINE_CLASS(QuestUI::Backgroundable);

std::map<std::string, std::string> backgrounds =  {
    { "round-rect-panel", "RoundRectPanel" },
    { "panel-bottom", "PanelBottom" },
    { "panel-top", "PanelTop" },
    { "round-rect-panel-shadow", "RoundRectPanelShadow"} 
};

std::map<std::string, std::string> objectNames =  {
    { "round-rect-panel", "MinScoreInfo" },
    { "panel-bottom", "BG" },
    { "panel-top", "HeaderPanel" },
    { "round-rect-panel-shadow", "Shadow"}
};

void QuestUI::Backgroundable::ApplyBackground(Il2CppString* name) {
    UnityEngine::UI::Image* search = ArrayUtil::Last(UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::UI::Image*>(), [&name](UnityEngine::UI::Image* x){ 
        UnityEngine::Sprite* sprite = x->get_sprite();
        if(sprite && to_utf8(csstrtostr(sprite->get_name())) != backgrounds[to_utf8(csstrtostr(name))])
            return false;
        return to_utf8(csstrtostr(x->get_name())) == objectNames[to_utf8(csstrtostr(name))]; 
    });
    background = get_gameObject()->AddComponent<UnityEngine::UI::Image*>();
    //Copy Image: some methods are probably not needed
    background->set_alphaHitTestMinimumThreshold(search->get_alphaHitTestMinimumThreshold());
    background->set_color(search->get_color());
    background->set_eventAlphaThreshold(search->get_eventAlphaThreshold());
    background->set_fillAmount(search->get_fillAmount());
    background->set_fillCenter(search->get_fillCenter());
    background->set_fillClockwise(search->get_fillClockwise());
    background->set_fillMethod(search->get_fillMethod());
    background->set_fillOrigin(search->get_fillOrigin());
    background->set_hideFlags(search->get_hideFlags());
    background->set_maskable(search->get_maskable());
    background->set_material(search->get_material());
    background->set_name(search->get_name());
    background->set_onCullStateChanged(search->get_onCullStateChanged());
    background->set_overrideSprite(search->get_overrideSprite());
    background->set_pixelsPerUnitMultiplier(search->get_pixelsPerUnitMultiplier());
    background->set_preserveAspect(search->get_preserveAspect());
    background->set_raycastTarget(search->get_raycastTarget());
    background->set_sprite(search->get_sprite());
    background->set_tag(search->get_tag());
    background->set_type(search->get_type());
    background->set_useGUILayout(search->get_useGUILayout());
    background->set_useLegacyMeshGeneration(search->get_useLegacyMeshGeneration());
    background->set_useSpriteMesh(search->get_useSpriteMesh());

    background->set_enabled(true);
}