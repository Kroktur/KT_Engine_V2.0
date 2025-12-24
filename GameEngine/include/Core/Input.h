#pragma once
#include <Windows.h>
#include <type_traits>
#include <unordered_map>

namespace KT
{

    enum class KEY : int
    {
        UNUSED = 00,
        A = 'A',
        Z = 'Z',
        E = 'E',
        R = 'R',
        T = 'T',
        Y = 'Y',
        U = 'U',
        I = 'I',
        O = 'O',
        P = 'P',
        Q = 'Q',
        S = 'S',
        D = 'D',
        F = 'F',
        G = 'G',
        H = 'H',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        W = 'W',
        X = 'X',
        C = 'C',
        V = 'V',
        B = 'B',
        N = 'N',
        ZERO = 0x30,
        ONE = 0x31,
        TWO = 0x32,
        THREE = 0x33,
        FOUR = 0x34,
        FIVE = 0x35,
        SIX = 0x36,
        SEVEN = 0x37,
        EIGHT = 0x38,
        NINE = 0x39,
        SPACE = ' ',
        LSHIFT = VK_LSHIFT,
        LCTRL = VK_LCONTROL,
        ALT = VK_MENU,
        TAB = VK_TAB
    };

    enum class MOUSE : int
    {
        LHS = VK_LBUTTON,
        RHS = VK_RBUTTON,
        BSCROLL = VK_SCROLL
    };


    struct Input
    {
    public:
        static constexpr int KEY_COUNT = 256;
        using container_type = std::unordered_map<int, bool>;
        static void Update();

        template<typename KEYTYPE> requires std::is_enum_v<KEYTYPE>
        static bool isPressed(const KEYTYPE& key);

    private:
        static container_type m_key;
        static container_type m_pressed;

    };






    template<typename KEYTYPE> requires std::is_enum_v<KEYTYPE>
    bool Input::isPressed(const KEYTYPE& key)
    {
        return m_pressed[static_cast<int>(key)];

    }
};