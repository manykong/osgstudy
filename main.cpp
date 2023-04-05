#include <Windows.h>

#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgViewer/Viewer>
#include <iostream>

// in this GUIEventHandler, when key s is pressed(KEYDOWN), 
// make one node visible( set one node mask 
// is zero, and next node mask is 0xffffffff )
class MyGUIEventHander : public osgGA::GUIEventHandler
{
public:
    MyGUIEventHander(osg::Group* grp):_grp(grp)
    {
        _visibleId = 0;
        for (int i = 0; i < _grp->getNumChildren(); i++)
        {
            if (i == 0)
            {
                _grp->getChild(i)->setNodeMask(0xffffffff);
            }
            else
            {
                _grp->getChild(i)->setNodeMask(0);
            }
        }
        
    }

    virtual ~MyGUIEventHander()
    {

    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
    {
        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if (!viewer)
            return false;

        if (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN)
        {
            auto k = ea.getKey();
            if (k == osgGA::GUIEventAdapter::KEY_S)
            {
                int n = _grp->getNumChildren();
                if (n > 0)
                {
                    _grp->getChild(_visibleId)->setNodeMask(0);
                    _visibleId = (_visibleId + 1) % n;
                    _grp->getChild(_visibleId)->setNodeMask(0xffffffff);
                    
                }
                
            }

            int i = 0;
        }


        return false;
    }

private:
    osg::Group* _grp;
    int _visibleId;
};

int main(int argc, char** argv)
{
    osgViewer::Viewer viewer;

    osg::ref_ptr<osg::Node> model0 = osgDB::readNodeFile("cessna.osgt");
    if (!model0)
    {
        return 1;
    }
    model0->setDataVariance(osg::Object::DYNAMIC);

    osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("glider.osgt");
    if (!model1)
    {
        return 1;
    }
    model1->setDataVariance(osg::Object::DYNAMIC);

    osg::Group* rootnode = new osg::Group;
    rootnode->addChild(model0);
    rootnode->addChild(model1);
    viewer.setSceneData(rootnode);
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    viewer.addEventHandler(new MyGUIEventHander(rootnode));

    viewer.setUpViewInWindow(50, 50, 800, 600);
    viewer.realize();


    while (!viewer.done())
    {
        viewer.frame();
    }

    return 0;
}