#ifndef __BRAIN_OPEN_G_L_F_P_VOLUME_OBLIQUE_DRAWING_H__
#define __BRAIN_OPEN_G_L_F_P_VOLUME_OBLIQUE_DRAWING_H__

/*LICENSE_START*/
/*
 * Copyright 2013 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/


#include "BrainOpenGLFixedPipeline.h"
#include "CaretObject.h"



namespace caret {

    class BrainOpenGLFixedPipeline;
    class BrowserTabContent;
    class Matrix4x4;
    
    class BrainOpenGLFPVolumeObliqueDrawing : public CaretObject {
        
    public:
        BrainOpenGLFPVolumeObliqueDrawing();
        
        virtual ~BrainOpenGLFPVolumeObliqueDrawing();
        
        void draw(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                  BrowserTabContent* browserTabContent,
                  std::vector<BrainOpenGLFixedPipeline::VolumeDrawInfo>& volumeDrawInfo,
                  const int viewport[4]);
        
        void drawAxis(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                      const VolumeSliceViewPlaneEnum::Enum slicePlane,
                      const Matrix4x4& rotationMatrix);
        
        void drawSlice(Brain* brain,
                       BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                  BrowserTabContent* browserTabContent,
                  std::vector<BrainOpenGLFixedPipeline::VolumeDrawInfo>& volumeDrawInfo,
                       const VolumeSliceViewPlaneEnum::Enum slicePlane,
                  const bool isSliceView);
        
        void drawSlicesForAllView(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                                  BrowserTabContent* browserTabContent,
                                  std::vector<BrainOpenGLFixedPipeline::VolumeDrawInfo>& volumeDrawInfo,
                                  const int viewport[4]);
        
        void drawSliceForSliceView(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                       BrowserTabContent* browserTabContent,
                       std::vector<BrainOpenGLFixedPipeline::VolumeDrawInfo>& volumeDrawInfo,
                       const VolumeSliceViewPlaneEnum::Enum slicePlane,
                       const int viewport[4]);
        
        void drawSliceForSliceViewOld(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                                   BrowserTabContent* browserTabContent,
                                   std::vector<BrainOpenGLFixedPipeline::VolumeDrawInfo>& volumeDrawInfo,
                                   const VolumeSliceViewPlaneEnum::Enum slicePlane,
                                   const int viewport[4]);
        
        void drawSliceOld(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                       BrowserTabContent* browserTabContent,
                       std::vector<BrainOpenGLFixedPipeline::VolumeDrawInfo>& volumeDrawInfo,
                       const VolumeSliceViewPlaneEnum::Enum slicePlane,
                       const int viewport[4]);
        
        void drawQuads(const std::vector<float>& coordinates,
                       const std::vector<float>& normals,
                       const std::vector<uint8_t>& rgbaColors);
        
        void drawLines(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                       const std::vector<float>& coordinates,
                       const std::vector<uint8_t>& rgbaColors,
                       const float thickness);
        
        void drawSurfaces(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                          BrowserTabContent* browserTabContent,
                          const int viewport[4]);
        
        void drawFibers(BrainOpenGLFixedPipeline* fixedPipelineDrawing,
                        const VolumeSliceViewPlaneEnum::Enum slicePlane,
                        const Matrix4x4& rotationMatrix);
    
    private:
        BrainOpenGLFPVolumeObliqueDrawing(const BrainOpenGLFPVolumeObliqueDrawing&);

        BrainOpenGLFPVolumeObliqueDrawing& operator=(const BrainOpenGLFPVolumeObliqueDrawing&);
        
        bool getVoxelCoordinateBoundsAndSpacing(const std::vector<BrainOpenGLFixedPipeline::VolumeDrawInfo>& volumeDrawInfo,
                                                float boundsOut[6],
                                                float spacingOut[3]);
    public:

        // ADD_NEW_METHODS_HERE

        virtual AString toString() const;
        
    private:
        // ADD_NEW_MEMBERS_HERE

    };
    
#ifdef __BRAIN_OPEN_G_L_F_P_VOLUME_OBLIQUE_DRAWING_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __BRAIN_OPEN_G_L_F_P_VOLUME_OBLIQUE_DRAWING_DECLARE__

} // namespace
#endif  //__BRAIN_OPEN_G_L_F_P_VOLUME_OBLIQUE_DRAWING_H__
