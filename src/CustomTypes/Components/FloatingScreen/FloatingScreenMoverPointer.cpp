#include "CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "CustomTypes/Components/FloatingScreen/FloatingScreenMoverPointer.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Physics.hpp"
#include "UnityEngine/RaycastHit.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Mathf.hpp"

#include "GlobalNamespace/VRController.hpp"

#include "VRUIControls/VRPointer.hpp"

#include <limits>

DEFINE_TYPE(QuestUI, FloatingScreenMoverPointer);

void QuestUI::FloatingScreenMoverPointer::Init(QuestUI::FloatingScreen* screen, VRUIControls::VRPointer* pointer){
    vrPointer = pointer;
    floatingScreen = screen;
    realPos = floatingScreen->get_transform()->get_position();
    realRot = floatingScreen->get_transform()->get_rotation();
    screenHandle  = floatingScreen->get_handle()->get_transform();
}

void QuestUI::FloatingScreenMoverPointer::Update(){
    if(screenHandle) {
        if(vrPointer->get_vrController()->get_triggerValue() > 0.9f) {
            if(grabbingController) 
                return;
            if(UnityEngine::Physics::Raycast(vrPointer->get_vrController()->get_position(), vrPointer->get_vrController()->get_forward(), hit, 100)){
                if(hit.get_collider()->get_transform() != screenHandle) 
                    return;
                grabbingController = vrPointer->get_vrController();
                grabPos = vrPointer->get_vrController()->get_transform()->InverseTransformPoint(floatingScreen->get_transform()->get_position());
                grabRot = UnityEngine::Quaternion::Inverse(vrPointer->get_vrController()->get_transform()->get_rotation()) * floatingScreen->get_transform()->get_rotation();
            }
        }
    }
    if(!grabbingController || grabbingController->get_triggerValue() > 0.9f) 
        return;
    grabbingController = nullptr;
}

void QuestUI::FloatingScreenMoverPointer::LateUpdate(){
    if(grabbingController) {
        float diff = grabbingController->get_verticalAxisValue() * UnityEngine::Time::get_unscaledDeltaTime();
        if(grabPos.get_magnitude() > 0.25f) {
            grabPos = grabPos - (UnityEngine::Vector3::get_forward() * diff);
        } else {
            grabPos = grabPos - (UnityEngine::Vector3::get_forward() * UnityEngine::Mathf::Clamp(diff, std::numeric_limits<float>::lowest(), 0.0f));
        }
        realPos = grabbingController->get_transform()->TransformPoint(grabPos);
        realRot = grabbingController->get_transform()->get_rotation() * grabRot;
    } else 
        return;

    floatingScreen->get_transform()->set_position(UnityEngine::Vector3::Lerp(floatingScreen->get_transform()->get_position(), realPos, 10 * UnityEngine::Time::get_unscaledDeltaTime()));
    floatingScreen->get_transform()->set_rotation(UnityEngine::Quaternion::Slerp(floatingScreen->get_transform()->get_rotation(), realRot, 5 * UnityEngine::Time::get_unscaledDeltaTime()));
}