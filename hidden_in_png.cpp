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

#include "hidden_in_png.h"
#include <cassert>
#include "lodepng/lodepng.h"
#include "lodepng/lodepng_util.h"
#include "akali_hpp/byteorder.hpp"

#define KEY_FIXED_LEN 32

HiddenInPNG::HiddenInPNG(const std::string& pngFilePath) :
    pngFilePath_(pngFilePath) {
}

HiddenInPNG::~HiddenInPNG() {
}

std::string HiddenInPNG::pngFilePath() const {
    return pngFilePath_;
}

bool HiddenInPNG::hasHiddenData(const std::string& key) {
    assert(key.length() > 0 && key.length() <= KEY_FIXED_LEN);
    if (!isLoaded_ || pngData_.empty() || !(key.length() > 0 && key.length() <= KEY_FIXED_LEN))
        return false;
    if (!isLoaded_ || pngData_.empty())
        return false;

    const std::string fixedKey = toFixedKey(key);

    bool result = false;
    std::vector<std::string> names[3];
    std::vector<std::vector<unsigned char> > chunks[3];
    if (0 == lodepng::getChunks(names, chunks, pngData_)) {
        for (size_t i = 0; i < names[2].size(); i++) {
            if (names[2][i] == "tEXt") {
                const unsigned char* pChunkStart = &chunks[2][i][0];
                const unsigned int dataLen = akl::ByteOrder::GetBE32(pChunkStart);
                const unsigned char* pChunkDataStart = pChunkStart + 8;

                if (dataLen < fixedKey.length())
                    continue;

                std::string strFlag;
                strFlag.assign((const char*)pChunkDataStart, fixedKey.length());

                if (strFlag != fixedKey)
                    continue;

                result = true;
                break;
            }
        }
    }

    return result;
}

bool HiddenInPNG::getHiddenData(const std::string& key, std::vector<unsigned char>& data) {
    assert(key.length() > 0 && key.length() <= KEY_FIXED_LEN);
    if (!isLoaded_ || pngData_.empty() || !(key.length() > 0 && key.length() <= KEY_FIXED_LEN))
        return false;

    const std::string fixedKey = toFixedKey(key);

    bool result = false;
    std::vector<std::string> names[3];
    std::vector<std::vector<unsigned char> > chunks[3];
    if (0 == lodepng::getChunks(names, chunks, pngData_)) {
        for (size_t i = 0; i < names[2].size(); i++) {
            if (names[2][i] == "tEXt") {
                const unsigned char* pChunkStart = &chunks[2][i][0];
                const unsigned int dataLen = akl::ByteOrder::GetBE32(pChunkStart);
                const unsigned char* pChunkDataStart = pChunkStart + 8;

                if (dataLen < fixedKey.length())
                    continue;

                std::string strFlag;
                strFlag.assign((const char*)pChunkDataStart, fixedKey.length());

                if (strFlag != fixedKey)
                    continue;

                data.resize(dataLen - fixedKey.length());
                memcpy(&data[0], pChunkDataStart + fixedKey.length(), dataLen - fixedKey.length());

                result = true;
                break;
            }
        }
    }

    return result;
}

bool HiddenInPNG::setHiddenData(const std::string& key, const unsigned char* data, size_t dataSize) {
    assert(key.length() > 0 && key.length() <= KEY_FIXED_LEN);
    if (!isLoaded_ || pngData_.empty() || !(key.length() > 0 && key.length() <= KEY_FIXED_LEN))
        return false;
    if (!isLoaded_ || pngData_.empty())
        return false;

    const std::string fixedKey = toFixedKey(key);

    if (hasHiddenData(key)) {
        // TODO
    }
    else {
        std::vector<std::vector<unsigned char> > chunks[3];
        const size_t size = fixedKey.length() + 1 + dataSize;

        std::vector<unsigned char> writtenData;
        writtenData.resize(size);
        memcpy(&writtenData[0], fixedKey.c_str(), fixedKey.size());
        memcpy((&writtenData[0] + fixedKey.size()), data, dataSize);

        unsigned char* newChunk = nullptr;
        size_t newChunkSize = 0;
        unsigned int ret = lodepng_chunk_create(&newChunk, &newChunkSize, size, "tEXt", &writtenData[0]);
        if (ret != 0)
            return false;

        std::vector<unsigned char> newChunkVector;
        newChunkVector.resize(newChunkSize);
        memcpy(&newChunkVector[0], newChunk, newChunkSize);

        chunks[2].push_back(newChunkVector);

        ret = lodepng::insertChunks(pngData_, chunks);
        if (ret != 0)
            return false;

        return true;
    }
}

bool HiddenInPNG::setHiddenData(const std::string& key, const std::vector<unsigned char>& data) {
    return setHiddenData(key, &data[0], data.size());
}

bool HiddenInPNG::removeHiddenData(const std::string& key) {
    assert(key.length() > 0 && key.length() <= KEY_FIXED_LEN);
    if (!isLoaded_ || pngData_.empty() || !(key.length() > 0 && key.length() <= KEY_FIXED_LEN))
        return false;
    if (!isLoaded_ || pngData_.empty())
        return false;

    const std::string fixedKey = toFixedKey(key);

    bool result = false;
    std::vector<std::string> names[3];
    std::vector<std::vector<unsigned char> > chunks[3];
    if (0 == lodepng::getChunks(names, chunks, pngData_)) {
        for (size_t i = 0; i < names[2].size(); i++) {
            if (names[2][i] == "tEXt") {
                const unsigned char* pChunkStart = &chunks[2][i][0];
                const unsigned int dataLen = akl::ByteOrder::GetBE32(pChunkStart);
                const unsigned char* pChunkDataStart = pChunkStart + 8;

                if (dataLen < fixedKey.length())
                    continue;

                std::string strFlag;
                strFlag.assign((const char*)pChunkDataStart, fixedKey.length());

                if (strFlag != fixedKey)
                    continue;

                // TODO

                result = true;
                break;
            }
        }
    }

    return result;
}

std::string HiddenInPNG::toFixedKey(const std::string& key) {
    std::string str = key;
    str.append(KEY_FIXED_LEN - key.length(), 'j');
    return str;
}

bool HiddenInPNG::loadFile() {
    if (!isLoaded_ && !pngFilePath_.empty()) {
        pngData_.clear();
        if (lodepng::load_file(pngData_, pngFilePath_) == 0) {
            isLoaded_ = true;
        }
    }
    return isLoaded_;
}

bool HiddenInPNG::reloadFile() {
    if (!pngFilePath_.empty()) {
        pngData_.clear();
        if (lodepng::load_file(pngData_, pngFilePath_) == 0) {
            isLoaded_ = true;
        }
        else {
            isLoaded_ = false;
        }
    }
    return isLoaded_;
}

bool HiddenInPNG::flushFile(const std::string& targetPngFilePath) {
    assert(isLoaded_);
    if (isLoaded_ && !targetPngFilePath.empty()) {
        return (lodepng::save_file(pngData_, targetPngFilePath) == 0);
    }
    return false;
}