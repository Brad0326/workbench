#ifndef __CARET_OBJECT_TRACKS_MODIFICATION__H_
#define __CARET_OBJECT_TRACKS_MODIFICATION__H_

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


#include "CaretObject.h"
#include "TracksModificationInterface.h"

namespace caret {

    class CaretObjectTracksModification : public CaretObject, public TracksModificationInterface {
        
    public:
        CaretObjectTracksModification();
        
        virtual ~CaretObjectTracksModification();
        
        CaretObjectTracksModification(const CaretObjectTracksModification& obj);

        CaretObjectTracksModification& operator=(const CaretObjectTracksModification& obj);
        
        virtual AString toString() const;
        
        virtual void setModified();
        
        virtual void clearModified();
        
        virtual bool isModified() const;
        
    private:
        void copyHelperCaretObjectTracksModification(const CaretObjectTracksModification& obj);
        
        bool modifiedFlag;
    };
    
#ifdef __CARET_OBJECT_TRACKS_MODIFICATION_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __CARET_OBJECT_TRACKS_MODIFICATION_DECLARE__

} // namespace
#endif  //__CARET_OBJECT_TRACKS_MODIFICATION__H_
