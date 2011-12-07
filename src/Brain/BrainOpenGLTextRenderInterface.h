#ifndef __BRAIN_OPEN_G_L_TEXT_RENDER_INTERFACE__H_
#define __BRAIN_OPEN_G_L_TEXT_RENDER_INTERFACE__H_

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


#include "CaretObject.h"

namespace caret {

    /// An interface for a system that renders text in OpenGL commands
    class BrainOpenGLTextRenderInterface : public CaretObject {
        
    protected:
        /**
         * Constructor.
         */
        BrainOpenGLTextRenderInterface() { }
        
    public:
        /**
         * Style of the text
         */
        enum TextStyle {
            BOLD,
            ITALIC,
            NORMAL
        };
        
        /**
         * Alignment of the text in X
         */
        enum TextAlignmentX {
            /** Text is centered at X-coordinate */
            X_CENTER,
            /** First character starts at X-coordinate */
            X_LEFT,
            /** Last character ends at X-coordinate */
            X_RIGHT
        };
        
        /**
         * Alignment of the text in Y
         */
        enum TextAlignmentY {
            /** Bottom of characters is at Y-coordinate */
            Y_BOTTOM,
            /** Text is centered at Y-coordinate */
            Y_CENTER,
            /** Top of characters is at Y-coordinate */
            Y_TOP
        };
        
        /**
         * Destructor.
         */
        virtual ~BrainOpenGLTextRenderInterface() { }
        
        /**
         * Draw text at the given window coordinates.
         *
         * @param viewport
         *   The current viewport.
         * @param windowX
         *   X-coordinate in the window of first text character
         *   using the 'alignment'
         * @param windowY
         *   Y-coordinate in the window at which bottom of text is placed.
         * @param text
         *   Text that is to be drawn.
         * @param alignmentX
         *   Alignment of text in X
         * @param alignmentY
         *   Alignment of text in Y
         * @param textStyle
         *   Style of the text.
         * @param fontHeight
         *   Height of the text.
         * @param fontName
         *   Name of the font.
         */
        virtual void drawTextAtWindowCoords(const int viewport[4],
                                            const int windowX,
                                            const int windowY,
                                            const QString& text,
                                            const TextAlignmentX alignmentX,
                                            const TextAlignmentY alignmentY,
                                            const TextStyle textStyle = NORMAL,
                                            const int fontHeight = 14,
                                            const AString& fontName = "times") = 0;
        
        /**
         * Draw text at the given model coordinates.
         *
         * @param modelX
         *   X-coordinate in model space of first text character
         * @param modelY
         *   Y-coordinate in model space.
         * @param modelZ
         *   Z-coordinate in model space.
         * @param text
         *   Text that is to be drawn.
         * @param textStyle
         *   Style of the text.
         * @param fontHeight
         *   Height of the text.
         * @param fontName
         *   Name of the font.
         */
        virtual void drawTextAtModelCoords(const double modelX,
                                           const double modelY,
                                           const double modelZ,
                                           const QString& text,
                                           const TextStyle textStyle = NORMAL,
                                           const int fontHeight = 14,
                                           const AString& fontName = "") = 0;
        
    private:
        BrainOpenGLTextRenderInterface(const BrainOpenGLTextRenderInterface&);

        BrainOpenGLTextRenderInterface& operator=(const BrainOpenGLTextRenderInterface&);
        
    private:
    };
    
#ifdef __BRAIN_OPEN_G_L_TEXT_RENDER_INTERFACE_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __BRAIN_OPEN_G_L_TEXT_RENDER_INTERFACE_DECLARE__

} // namespace
#endif  //__BRAIN_OPEN_G_L_TEXT_RENDER_INTERFACE__H_
