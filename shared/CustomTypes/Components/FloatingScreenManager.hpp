#include "CustomTypes/Components/FloatingScreenMoverPointer.hpp"
#include "CustomTypes/Components/FloatingScreen.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

#include "custom-types/shared/macros.hpp"
DECLARE_CLASS_CODEGEN(QuestUI, FloatingScreenManager, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, createMover, QuestUI::FloatingScreen* screen, VRUIControls::VRPointer* vrPointer);

    REGISTER_FUNCTION(FloatingScreenManager,
        REGISTER_METHOD(createMover);
    )
)