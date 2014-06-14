#ifndef polygonrenderer_h
#define polygonrenderer_h

#include "Vector3D.h"
#include "Transform3D.h"
#include "Polygon3D.h"
#include "Scanconverter.h"
#include "ViewWindow.h"


namespace Miniboi3D
{
    class PolygonRenderer
    {
    public:
        PolygonRenderer() {}
        virtual ~PolygonRenderer() {}

        PolygonRenderer(const Transform3D& camera, const ViewWindow& viewWindow);
        PolygonRenderer(const Transform3D& camera, const ViewWindow& viewWindow, bool clearViewEveryFrame);
        Transform3D& getCamera()  { return m_camera; }
        void startFrame();
        void endFrame() {};
        bool draw(Polygon3D* poly);
        void resetCounters() { m_numClipped = m_numFacing = 0; }

        int m_numFacing;
        int m_numClipped;

    protected:
        ScanConverter m_scanConverter;
        Transform3D m_camera;
        ViewWindow m_viewWindow;
        bool m_clearViewEveryFrame;
        Polygon3D* m_sourcePolygon;     // a pointer because behavior is polymorphic
        Polygon3D m_destPolygon;


        void init(const Transform3D& camera, const ViewWindow& viewWindow, bool clearViewEveryFrame);

        // This must be implemented by a subclass - it does the actual drawing
        virtual void drawCurrentPolygon() = 0;

    private:

    };

}
#endif // polygonrenderer_h
