#include "CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenManager.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/PrimitiveType.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Resources.hpp"

#include "VRUIControls/VRPointer.hpp"

DEFINE_TYPE(QuestUI, FloatingScreen);

void QuestUI::FloatingScreen::set_bgGo(UnityEngine::GameObject* bg) { 
    bgGo = bg;
}

void QuestUI::FloatingScreen::updateHandle() {
    if(!handle) 
        return;
    switch(handleSide) {
        case 0:
            handle->get_transform()->set_localPosition(UnityEngine::Vector3(-screenSize.x / 2, 0, 0));
            handle->get_transform()->set_localScale(UnityEngine::Vector3(screenSize.x / 15, screenSize.y * 1, screenSize.x / 15));
            break;
        case 1:
            handle->get_transform()->set_localPosition(UnityEngine::Vector3(screenSize.x / 2, 0, 0));
            handle->get_transform()->set_localScale(UnityEngine::Vector3(screenSize.x / 15, screenSize.y * 0.8f, screenSize.x / 15));
            break;
        case 2:
            handle->get_transform()->set_localPosition(UnityEngine::Vector3(0, screenSize.y / 2, 0));
            handle->get_transform()->set_localScale(UnityEngine::Vector3(screenSize.x, screenSize.y / 15, screenSize.y / 15));
            break;
        case 3:
            handle->get_transform()->set_localPosition(UnityEngine::Vector3(0, -screenSize.y / 2, 0));
            handle->get_transform()->set_localScale(UnityEngine::Vector3(screenSize.x, screenSize.y / 15, screenSize.y / 15));
            break;
        case 4:
            handle->get_transform()->set_localPosition(UnityEngine::Vector3::get_zero());
            handle->get_transform()->set_localScale(UnityEngine::Vector3(screenSize.x, screenSize.y, screenSize.x / 15));
            break;
    }
    handle->GetComponent<UnityEngine::MeshRenderer*>()->set_enabled(handleSide != 4);
}

void QuestUI::FloatingScreen::createHandle() {
    auto vrpointer = UnityEngine::Resources::FindObjectsOfTypeAll<VRUIControls::VRPointer*>()[0];
    if(!handle) {
        handle = UnityEngine::GameObject::CreatePrimitive(UnityEngine::PrimitiveType::_get_Cube());
        handle->get_transform()->set_parent(get_transform());
        handle->get_transform()->set_localRotation(UnityEngine::Quaternion::get_identity());

        auto rend = handle->GetComponent<UnityEngine::MeshRenderer*>();
        auto static searchName = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("Custom/SimpleLit");
        auto mat = UnityEngine::Material::New_ctor(UnityEngine::Shader::Find(searchName));
        mat->set_color(UnityEngine::Color::get_white());
        rend->set_material(mat);
        GetComponent<QuestUI::FloatingScreenManager*>()->createMover(this, vrpointer);
    }
    updateHandle();
}

void QuestUI::FloatingScreen::set_showHandle(bool value) {
    showHandle = value;
    if(!handle) 
        createHandle();
    updateHandle();
}

void QuestUI::FloatingScreen::set_screenSize(UnityEngine::Vector2 size) {
    screenSize = size;
    GetComponent<UnityEngine::RectTransform*>()->set_sizeDelta(size);
    if(bgGo)
        bgGo->GetComponent<UnityEngine::RectTransform*>()->set_sizeDelta(size);
    updateHandle();
}

UnityEngine::GameObject* QuestUI::FloatingScreen::get_handle() {
    return handle;
}

int QuestUI::FloatingScreen::get_side() {
    return handleSide;
}

void QuestUI::FloatingScreen::set_side(int side) {
    handleSide = side;
}