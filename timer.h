#ifndef TIMER_H
#define TIMER_H


#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

/*!
 * \brief Timer klassen er til hjelp for å kunne operere ryddig med tidsvariabler og funksjoner
 */
class Timer
{
public:
    Timer();

    void start()
        {
            m_StartTime = std::chrono::system_clock::now();
            m_bRunning = true;
        }

        void stop()
        {
            m_EndTime = std::chrono::system_clock::now();
            m_bRunning = false;
        }

        double elapsedMilliseconds()
        {
            std::chrono::time_point<std::chrono::system_clock> endTime;

            if(m_bRunning)
            {
                endTime = std::chrono::system_clock::now();
            }
            else
            {
                endTime = m_EndTime;
            }

            return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
        }

        double elapsedSeconds()
        {
            return elapsedMilliseconds() / 1000.0;
        }

    private:
        std::chrono::time_point<std::chrono::system_clock> m_StartTime;
        std::chrono::time_point<std::chrono::system_clock> m_EndTime;
        bool                                               m_bRunning = false;
};

#endif // TIMER_H
