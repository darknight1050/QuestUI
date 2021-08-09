#include "FloatingScreenMoverPointer.hpp"
#include "FloatingScreen.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "custom-types/shared/macros.hpp"
DECLARE_CLASS_CODEGEN(QuestUI, FloatingScreenManager, UnityEngine::MonoBehaviour,

    DECLARE_INSTANCE_METHOD(void, createMover, QuestUI::FloatingScreen* screen, VRUIControls::VRPointer* vrPointer);
    
)