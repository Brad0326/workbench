#ifndef __CUBIC_SPLINE_H__
#define __CUBIC_SPLINE_H__

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

namespace caret {

    class CubicSpline
    {
        float m_weights[4];
        CubicSpline();
    public:
        ///takes as input the fraction in [0, 1] along the middle (used) range of the spline, low and high edge set whether it doesn't have p[0] or p[3] to use, respectively
        static CubicSpline hermite(float frac, bool lowEdge, bool highEdge);
        
        ///NOTE: data should be deconvolved before using this spline
        static CubicSpline bspline(float frac, bool lowEdge, bool highEdge);

        //splines will be reused, so this part should be fast for the majority case (testing for if it is an edge case would slow it down for the majority case)
        ///evaluate the spline with these samples
        inline float evaluate(const float p0, const float p1, const float p2, const float p3) const
        {
            return p0 * m_weights[0] + p1 * m_weights[1] + p2 * m_weights[2] + p3 * m_weights[3];
        }
        
        ///convenience function for edge evaluating without a dummy argument
        inline float evalLowEdge(const float p1, const float p2, const float p3)
        {
            return p1 * m_weights[1] + p2 * m_weights[2] + p3 * m_weights[3];
        }
        
        ///convenience function for edge evaluating without a dummy argument
        inline float evalHighEdge(const float p0, const float p1, const float p2)
        {
            return p0 * m_weights[0] + p1 * m_weights[1] + p2 * m_weights[2];
        }
        
        ///convenience function for edge evaluating without dummy arguments
        inline float evalBothEdge(const float p1, const float p2)
        {
            return p1 * m_weights[1] + p2 * m_weights[2];
        }
    };

}

#endif //__CUBIC_SPLINE_H__
