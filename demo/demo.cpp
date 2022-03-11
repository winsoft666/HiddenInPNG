/*******************************************************************************
*    Copyright (C) <2022>  <winsoft666@outlook.com>.
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
#include "akali_hpp/file.hpp"

int main()
{
    void* hiddenData = nullptr;
    size_t hiddenDataSize = 0;

    akl::File hiddenFile("../hidden.jpg");
    if (hiddenFile.open("rb")) {
        hiddenDataSize = hiddenFile.readAll(&hiddenData);
        hiddenFile.close();
    }

    const std::string key = "lilili";
    HiddenInPNG hip("../origin.png");
    if (hip.loadFile()) {
        bool hasHidden = hip.hasHiddenData(key);
        assert(hasHidden == false);

        bool result = hip.setHiddenData(key, (const unsigned char*) hiddenData, hiddenDataSize);
        assert(result == true);

        result = hip.flushFile("../target.png");
        assert(result == true);

        std::cout << "Hidden hidden.png to target.png" << std::endl;
    }

    return 0;
}
