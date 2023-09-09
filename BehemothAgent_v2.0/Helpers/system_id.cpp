#include <Windows.h>
#include <string>
#include <array>
#include <algorithm>
#include <vector>
#include "system_id.h"


bool timzone_id() {
    const std::array<std::wstring, 18> USCATimes = {
        L"Eastern Standard Time", L"Eastern Daylight Time",
        L"Central Standard Time", L"Central Daylight Time",
        L"Mountain Standard Time", L"Mountain Daylight Time",
        L"Pacific Standard Time", L"Pacific Daylight Time",
        L"Alaskan Standard Time", L"Alaskan Daylight Time",
        L"Hawaiian Standard Time", L"Hawaiian Daylight Time",
        L"Aleutian Standard Time", L"Aleutian Daylight Time",
        L"Atlantic Standard Time", L"Atlantic Daylight Time",
        L"Newfoundland Standard Time", L"Newfoundland Daylight Time"
    };

    TIME_ZONE_INFORMATION tzInfo;
    GetTimeZoneInformation(&tzInfo);

    std::wstring timeZoneName(tzInfo.StandardName);
    std::wstring daylightTimeZoneName(tzInfo.DaylightName);

    return std::find(USCATimes.begin(), USCATimes.end(), timeZoneName) != USCATimes.end() ||
        std::find(USCATimes.begin(), USCATimes.end(), daylightTimeZoneName) != USCATimes.end();
}


bool keyboard_id() {
    // Get the list of keyboard layouts for the current process.
    int size = GetKeyboardLayoutList(0, NULL);
    std::vector<HKL> layouts(size);
    GetKeyboardLayoutList(size, layouts.data());

    // The low word of the HKL (handle to keyboard layout) is the language identifier.
    // 0x0409 is the language identifier for English (United States), and 0x1009 is the language identifier for English (Canada).
    for (HKL layout : layouts) {
        LANGID langId = LOWORD(layout);
        if (langId == 0x0409 || langId == 0x1009) {
            return true;
        }
    }
    return false;
}


bool language_id() {
    // Get the user default locale:
    WCHAR localeName[LOCALE_NAME_MAX_LENGTH];
    int ret = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
    if (ret == 0) {
        // Failed to get the locale:
        return false;
    }

    // Check if the locale is English (United States) or English (Canada):
    if (wcscmp(localeName, L"en-US") == 0 || wcscmp(localeName, L"en-CA") == 0) {
        return true;
    }

    return false;
}


bool friendly() {
    int score = 0;

    if (timzone_id()) {
        score += 1;
    }

    if (keyboard_id()) {
        score += 1;
    }

    if (language_id()) {
        score += 1;
    }

    return score >= 2;
}
