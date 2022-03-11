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

#ifndef HIDDEN_IN_PNG_H__
#define HIDDEN_IN_PNG_H__
#include <string>
#include <vector>

class HiddenInPNG {
   public:
    HiddenInPNG(const std::string& pngFilePath);
    ~HiddenInPNG();

    std::string pngFilePath() const;

    bool loadFile();
    bool reloadFile();
    bool flushFile(const std::string& targetPngFilePath);

    bool hasHiddenData(const std::string& key);
    bool getHiddenData(const std::string& key, std::vector<unsigned char>& data);
    bool setHiddenData(const std::string& key, const unsigned char* data, size_t dataSize);
    bool setHiddenData(const std::string& key, const std::vector<unsigned char>& data);
    bool removeHiddenData(const std::string& key);

   protected:
    std::string toFixedKey(const std::string& key);
    bool isLoaded_ = false;
    std::string pngFilePath_;
    std::vector<unsigned char> pngData_;
};

#endif  // !HIDDEN_IN_PNG_H__