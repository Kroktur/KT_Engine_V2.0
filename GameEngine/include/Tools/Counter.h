#pragma once

#include <stdexcept>
namespace KT
{
    template <typename type = int>
    class Counter {
    public:

        Counter(const type& min, const type& max)
            : m_maxCounter(max), m_minCounter(min), m_currentCounter(min) {
        }
        type GetCurrentCounter() const { return m_currentCounter; }

        type GetMinCounter() const { return m_minCounter; }

        type GetMaxCounter() const { return m_maxCounter; }

        void SetMinCounter(const type& val) {
            m_minCounter = val;
            VerifyMin();
        }

        void SetMaxCounter(const type& val) {
            m_maxCounter = val;
            VerifyMax();
        }

        void SetCurrentCounter(const type& val)
        {
            m_currentCounter = val;
            VerifyMax();
            VerifyMax();
        }

        void Next() {
            ++m_currentCounter;
            VerifyMax();
        }

        void Previous() {
            --m_currentCounter;
            VerifyMin();
        }

        void ResetBegin() { m_currentCounter = m_minCounter; }

        void ResetEnd() { m_currentCounter = m_maxCounter; }

        bool IsEnd() const { return m_currentCounter >= m_maxCounter; }

        bool IsBegin() const { return m_currentCounter <= m_minCounter; }

        type GetAutoCounterMinToMax() {
            VerifyMax();
            VerifyMin();
            type tmp = m_currentCounter;
            IsEnd() ? ResetBegin() : Next();
            return tmp;
        }

        type GetAutoCounterMaxToMin() {
            VerifyMax();
            VerifyMin();
            type tmp = m_currentCounter;
            IsBegin() ? ResetEnd() : Previous();
            return tmp;
        }

        type GetCounterMinToMax(const type& min, const type& max) {
            if (min < m_minCounter || max > m_maxCounter)
                throw std::out_of_range("Counter: Range out of bounds.");

            VerifyMin(min);
            VerifyMax(max);

            type tmp = m_currentCounter;
            m_currentCounter = (m_currentCounter >= max) ? min : m_currentCounter + 1;
            return tmp;
        }

        type GetCounterMaxToMin(const type& min, const type& max) {
            if (min < m_minCounter || max > m_maxCounter)
                throw std::out_of_range("Counter: Range out of bounds.");

            VerifyMin(min);
            VerifyMax(max);

            type tmp = m_currentCounter;
            m_currentCounter = (m_currentCounter <= min) ? max : m_currentCounter - 1;
            return tmp;
        }

        bool AutoIsTimeMinToMax() {
            VerifyMin();
            VerifyMax();
            if (IsEnd()) {
                ResetBegin();
                return true;
            }
            Next();
            return false;
        }

        bool AutoIsTimeMaxToMin() {
            VerifyMin();
            VerifyMax();
            if (IsBegin()) {
                ResetEnd();
                return true;
            }
            Previous();
            return false;
        }

        bool IsTimeMinToMax(const type& min, const type& max) {
            if (min < m_minCounter || max > m_maxCounter)
                throw std::out_of_range("Counter: Range out of bounds.");

            VerifyMin(min);
            VerifyMax(max);
            if (m_currentCounter >= max) {
                m_currentCounter = min;
                return true;
            }
            Next();
            return false;
        }

        bool IsTimeMaxToMin(const type& min, const type& max) {
            if (min < m_minCounter || max > m_maxCounter)
                throw std::out_of_range("Counter: Range out of bounds.");

            VerifyMin(min);
            VerifyMax(max);
            if (m_currentCounter <= min) {
                m_currentCounter = max;
                return true;
            }
            Previous();
            return false;
        }

        void Increment(const type& add)
        {
            m_currentCounter += add;
            VerifyMax();
        }

        void Decrement(const type& remove)
        {
            m_currentCounter -= remove;
            VerifyMin();
        }

    private:
        void VerifyMax() {
            if (m_currentCounter > m_maxCounter)
                m_currentCounter = m_maxCounter;
        }

        void VerifyMin() {
            if (m_currentCounter < m_minCounter)
                m_currentCounter = m_minCounter;
        }

        void VerifyMax(const type& max) {
            if (m_currentCounter > max)
                m_currentCounter = max;
        }

        void VerifyMin(const type& min) {
            if (m_currentCounter < min)
                m_currentCounter = min;
        }

        type m_maxCounter;
        type m_minCounter;
        type m_currentCounter;
    };
}
