#include "MTime.h"

MTime::MTime(int hrs, int min, int sec)
{
    SetTime(hrs, min, sec);
}

void MTime::SetTime(int hrs, int min, int sec) {
    if (hrs >= 0 && hrs < 24) {
        hours = hrs;
    } else {
        hours = 0;
    }

    if (min >= 0 && min < 60) {
        minutes = min;
    } else {
        minutes = 0;
    }

    if (sec >= 0 && sec < 60) {
        seconds = sec;
    } else {
        seconds = 0;
    }
}

void MTime::DisplayTime() { // 24 hour format
    // Display in hh:mm:ss format, with trailing 0s
    std::cout << (hours < 10 ? "0" : "") << hours << ":" << (minutes < 10 ? "0" : "") << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;
}

bool MTime::operator==(MTime & OtherTime) {
    return (hours == OtherTime.hours && minutes == OtherTime.minutes && seconds == OtherTime.seconds);
}

bool MTime::operator>(MTime &OtherTime) {
    if(hours > OtherTime.hours) {
        return true;
    } else if(hours == OtherTime.hours) {
        if(minutes > OtherTime.minutes) {
            return true;
        } else if(minutes == OtherTime.minutes) {
            return seconds > OtherTime.seconds;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

ostream & operator<<(ostream & out, const MTime &timeObj) { // 12 hour format
    // Display in hh:mm:ss format, with trailing 0s
    bool isPM = false;
    if(timeObj.hours >= 12) {
        isPM = true;
    }
    int displayHours = 0;
    if(timeObj.hours > 12)
        displayHours = timeObj.hours - 12;
    else
        displayHours = timeObj.hours;
    out << displayHours << ":" << (timeObj.minutes < 10 ? "0" : "") << timeObj.minutes << ":" << (timeObj.seconds < 10 ? "0" : "") << timeObj.seconds << (isPM ? " PM" : " AM");
    return out;
}