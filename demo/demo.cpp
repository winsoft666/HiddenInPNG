/*******************************************************************************
*    Hidden anything into PNG file and NOT affect original image.
*    -------------------------------------------------------------------------
*    Copyright (C) 2022 JiangXueqiao <winsoft666@outlook.com>.
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#include <cassert>
#include <iostream>
#include "../hidden_in_png.h"
#include "jhc/file.hpp"

int main() {
    void* hiddenData = nullptr;
    size_t hiddenDataSize = 0;

    // Read hidden.jpg file content to buffer
    jhc::File hiddenFile("../hidden.jpg");
    if (hiddenFile.open("rb")) {
        hiddenDataSize = hiddenFile.readAll(&hiddenData);
        hiddenFile.close();
    }

    const std::string key = "lilili";

    // Hidden hidden.jpg into target_with_hidden.png
    //
    HiddenInPNG hip("../origin.png");
    if (hip.loadFile()) {
        const bool hasHidden = hip.hasHiddenData(key);
        assert(hasHidden == false);

        bool result = hip.setHiddenData(key, (const unsigned char*)hiddenData, hiddenDataSize);
        assert(result == true);

        result = hip.flushFile("../target_with_hidden.png");
        assert(result == true);

        std::cout << "OK, Hidden hidden.jpg to target_with_hidden.png" << std::endl;
    }
    else {
        std::cout << "Failed, unable to load ../origin.png";
    }

    // Remove hidden data from target_with_hidden.png
    //
    HiddenInPNG hip2("../target_with_hidden.png");
    if (hip2.loadFile()) {
        const bool hasHidden = hip2.hasHiddenData(key);
        assert(hasHidden == true);

        bool result = hip2.removeHiddenData(key);
        assert(result == true);

        result = hip2.flushFile("../target_without_hidden.png");
        assert(result == true);

        std::cout << "OK, Remove hidden.jpg, save new file to target_without_hidden.png" << std::endl;
    }
    else {
        std::cout << "Failed, unable to load ../target_with_hidden.png";
    }

    // Check whether hidden data has been removed form target_without_hidden.png or not
    //
    HiddenInPNG hip3("../target_without_hidden.png");
    if (hip2.loadFile()) {
        const bool hasHidden = hip2.hasHiddenData(key);
        assert(hasHidden == false);
        if (!hasHidden)
            std::cout << "Yes, hidden.jpg has been removed, save new file to target_without_hidden.png" << std::endl;
    }
    else {
        std::cout << "Failed, unable to load ../target_without_hidden.png";
    }

    return 0;
}
