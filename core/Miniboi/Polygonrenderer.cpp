#include "Polygonrenderer.h"

namespace Miniboi3D
{
    PolygonRenderer::PolygonRenderer(const Transform3D& camera, const ViewWindow& viewWindow)
    {
        init(camera, viewWindow, true);
    }

    PolygonRenderer::PolygonRenderer(const Transform3D& camera, const ViewWindow& viewWindow, bool clearViewEveryFrame)
    {
        init(camera, viewWindow, clearViewEveryFrame);
    }

    void PolygonRenderer::init(const Transform3D& camera, const ViewWindow& viewWindow, bool clearViewEveryFrame)
    {
        m_camera = camera;
        m_viewWindow = viewWindow;
        m_clearViewEveryFrame = clearViewEveryFrame;
        m_scanConverter = ScanConverter(viewWindow);
        m_sourcePolygon = NULL;
        m_numClipped = m_numFacing = 0;

    }

    void PolygonRenderer::startFrame()
    {
        if (m_clearViewEveryFrame)
        {

        }
    }

    bool PolygonRenderer::draw(Polygon3D* poly)
    {
        if ((*poly).isFacing(m_camera.getLocation()))
        {
            m_numFacing++;
            m_sourcePolygon = poly; // save the source poly in case data is needed later
            m_destPolygon = *poly; // point dest_poly to the polygon
            m_destPolygon.subtract(m_camera); // substract camera distance from polygon = transform in world coords
            bool visible = m_destPolygon.clip(-1.0f);
            if (visible)
            {
                m_destPolygon.project(m_viewWindow);
                visible = m_scanConverter.convert(m_destPolygon);
                if (visible)
                {
                    drawCurrentPolygon();
                    return true;
                }
            }
            else
                m_numClipped++;
        }

        return false;
    }

}
