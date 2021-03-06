#ifndef __VIEW_MODE_ENUM_H__
#define __VIEW_MODE_ENUM_H__

/*LICENSE_START*/
/*
 *  Copyright (C) 2014  Washington University School of Medicine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*LICENSE_END*/


#include <stdint.h>
#include <vector>
#include <QString>

namespace caret {

/**
 * View Mode Types.
 */
class ViewModeEnum {

public:
    /**  View Mode Types.
 */
    enum Enum {
        /** Invalid */
        VIEW_MODE_INVALID,
        /** View Surface */
        VIEW_MODE_SURFACE,
        /** View Volume Slices */
        VIEW_MODE_VOLUME_SLICES,
        /** View Whole Brain */
        VIEW_MODE_WHOLE_BRAIN 
    };


    ~ViewModeEnum();

    static QString toName(Enum e);
    
    static Enum fromName(const QString& s, bool* isValidOut);
    
    static QString toGuiName(Enum e);
    
    static Enum fromGuiName(const QString& s, bool* isValidOut);
    
    static int32_t toIntegerCode(Enum e);
    
    static Enum fromIntegerCode(const int32_t integerCode, bool* isValidOut);

private:
    ViewModeEnum(const Enum e, 
                 const int32_t integerCode, 
                 const QString& name,
                 const QString& guiName);

    static const ViewModeEnum* findData(const Enum e);

    static std::vector<ViewModeEnum> enumData;

    static void initialize();

    static bool initializedFlag;

    Enum e;

    int32_t integerCode;

    QString name;
    
    QString guiName;
};

#ifdef __VIEW_MODE_DECLARE__
std::vector<ViewModeEnum> ViewModeEnum::enumData;
bool ViewModeEnum::initializedFlag = false;
#endif // __VIEW_MODE_DECLARE__

} // namespace

#endif // __VIEW_MODE_ENUM_H__
