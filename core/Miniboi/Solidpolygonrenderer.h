#ifndef solidpolygonrenderer_h
#define solidpolygonrenderer_h

#include "Polygonrenderer.h"

namespace Miniboi3D
{
    class SolidPolygonRenderer : public PolygonRenderer
    {
    public:
        SolidPolygonRenderer() {}
        SolidPolygonRenderer(const Transform3D& camera, const ViewWindow& viewWindow) { init(camera, viewWindow, true); }
        SolidPolygonRenderer(const Transform3D& camera, const ViewWindow& viewWindow, bool clearViewEveryFrame)
            { init(camera, viewWindow, clearViewEveryFrame); }



    protected:
        void drawCurrentPolygon();

    private:

    };

}

#endif // solidpolygonrenderer_h
