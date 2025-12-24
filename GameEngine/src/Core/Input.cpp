#include "Core/Input.h"


KT::Input::container_type KT::Input::m_key = Input::container_type{};
KT::Input::container_type KT::Input::m_pressed = Input::container_type{};

void KT::Input::Update()
{
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        m_key[i] = GetAsyncKeyState(i) & 0x8000;
        m_pressed[i] = m_key[i];
    }
}