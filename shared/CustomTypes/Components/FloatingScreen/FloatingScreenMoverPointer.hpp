#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Physics.hpp"
#include "UnityEngine/RaycastHit.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Mathf.hpp"

#include "GlobalNamespace/VRController.hpp"

#include "FloatingScreen.hpp"
#include "VRUIControls/VRPointer.hpp"

DECLARE_CLASS_CODEGEN(QuestUI, FloatingScreenMoverPointer, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_FIELD(VRUIControls::VRPointer*, vrPointer);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::VRController*, grabbingController);
    DECLARE_INSTANCE_FIELD(::QuestUI::FloatingScreen*, floatingScreen);
    DECLARE_INSTANCE_FIELD(UnityEngine::Transform*, screenHandle);
    DECLARE_INSTANCE_FIELD(UnityEngine::Vector3, grabPos);
    DECLARE_INSTANCE_FIELD(UnityEngine::Quaternion, grabRot);
    DECLARE_INSTANCE_FIELD(UnityEngine::Vector3, realPos);
    DECLARE_INSTANCE_FIELD(UnityEngine::Quaternion, realRot);
    DECLARE_INSTANCE_FIELD(UnityEngine::RaycastHit, hit);

    DECLARE_METHOD(void, Init, ::QuestUI::FloatingScreen* screen, VRUIControls::VRPointer* pointer);
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, LateUpdate);

    REGISTER_FUNCTION(FloatingScreenMoverPointer,
        REGISTER_FIELD(vrPointer);
        REGISTER_FIELD(grabbingController);
        REGISTER_FIELD(floatingScreen);
        REGISTER_FIELD(screenHandle);
        REGISTER_FIELD(grabPos);
        REGISTER_FIELD(grabRot);
        REGISTER_FIELD(realPos);
        REGISTER_FIELD(realRot);
        REGISTER_FIELD(hit);

        REGISTER_METHOD(Init);
        REGISTER_METHOD(Update);
        REGISTER_METHOD(LateUpdate);
    )
)