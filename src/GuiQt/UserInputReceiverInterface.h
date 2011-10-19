#ifndef __USER_INPUT_RECEIVER_INTERFACE__H_
#define __USER_INPUT_RECEIVER_INTERFACE__H_

/*LICENSE_START*/
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 


namespace caret {

    class BrainOpenGLWidget;
    class BrowserTabContent;
    class MouseEvent;
    
    /**
     * \class UserInputReceiverInterface
     * \brief Provides an interface for user input events
     *
     * Classes implementing this interface receive
     * user input events from the OpenGL graphics
     * region of a BrowserWindow containing brain
     * models.
     */
    class UserInputReceiverInterface {
        
    public:
        /**
         * Called when a mouse events occurs for 'this' 
         * user input receiver.
         *
         * @param mouseEvent
         *     The mouse event.
         * @param browserTabContent
         *     Content of the browser window's tab.
         * @param openGLWidget
         *     OpenGL Widget in which mouse event occurred.
         */
        virtual void processMouseEvent(MouseEvent* mouseEvent,
                               BrowserTabContent* browserTabContent,
                               BrainOpenGLWidget* openGLWidget) = 0;
        
        /**
         * Called when 'this' user input receiver is set
         * to receive events.
         */
        virtual void initialize() = 0;
        
        /**
         * Called when 'this' user input receiver is no
         * longer set to receive events.
         */
        virtual void finish() = 0;
        
    protected:
        UserInputReceiverInterface() { }
        
        virtual ~UserInputReceiverInterface() { }
        
    private:
        UserInputReceiverInterface(const UserInputReceiverInterface&) { }

        UserInputReceiverInterface& operator=(const UserInputReceiverInterface& uiri) {
            if (this != &uiri) {
            }
            return *this;
        }
        
    };
    
#ifdef __USER_INPUT_RECEIVER_INTERFACE_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __USER_INPUT_RECEIVER_INTERFACE_DECLARE__

} // namespace
#endif  //__USER_INPUT_RECEIVER_INTERFACE__H_
