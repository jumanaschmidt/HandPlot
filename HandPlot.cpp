// This is a graph which updates on each step. To test for a bug, but also to show how
// a graph can be completely redrawn each time, if required.
#include <morph/Visual.h>
#include <morph/GraphVisual.h>
#include <morph/vVector.h>
#include <morph/MathConst.h>

int main()
{
	morph::Visual v(1024, 768, "Continuous redrawing of GraphVisual");

	auto gv = new morph::GraphVisual<double> (v.shaderprog, v.tshaderprog, {0,0,0});

	morph::vVector<double> x;
	morph::vVector<double> y;
	gv->setsize(2,2);
	v.setSceneTransXY(-1, -1);
	x.linspace (-morph::mathconst<double>::pi, morph::mathconst<double>::pi, 100);
	y.linspace (-morph::mathconst<double>::pi, morph::mathconst<double>::pi, 100);

	double dx = 0.;		
	gv->setdata (x, y);
	gv->finalize();

	v.addVisualModel (gv);

	while (v.readyToFinish == false) {
		dx += 0.01667;
		glfwWaitEventsTimeout (0.01667); // 16.67 ms ~ 60 Hz
		
		
		double p = sin(dx*3.0);
		
		
		
		for(int i=0; i<y.size()-1; i++){
			y[i] = y[i+1];
		}
		y[y.size()-1] = p;
		gv->update (x, y, 0);
		v.render();
	}

	return 0;
}
