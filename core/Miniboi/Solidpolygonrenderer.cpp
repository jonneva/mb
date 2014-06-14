#include "Solidpolygonrenderer.h"
#include "Solidpolygon3D.h"
#include "Miniboi.h"

namespace Miniboi3D
{
    /*
    Draws the current polygon. At this point, the current
    polygon is transformed, clipped, projected,
    scan-converted, and visible.
    */
    void SolidPolygonRenderer::drawCurrentPolygon()
    {
        // draw the scans
        int y = m_scanConverter.getTopBoundary();
        while (y <= m_scanConverter.getBottomBoundary())
        {
            ScanConverter::Scan scan = m_scanConverter[y];

            if (scan.isValid())
            {
                // TODO: sort out this horrible cast
                //line_fast(scan.left, y, scan.right, y, (*(SolidPolygon3D*)m_sourcePolygon).getColor());
                //line_horiz(scan.left, scan.right, y, (*(SolidPolygon3D*)m_sourcePolygon).getColor());
                MB.draw_row(y,scan.left, scan.right, (*(SolidPolygon3D*)m_sourcePolygon).getFillColor());
            }
            y++;
        }

    }
}
