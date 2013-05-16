//
//  Fluid.cpp
//  Platformer
//
//  Created by Aron Granberg on 2013-05-16.
//
//

#include "Fluid.h"
#include<algorithm>
#include<cmath>
#include<iostream>
#include "assert.h"

using namespace std;

Fluid::Fluid () {
}

void Fluid::Init (int width, int height) {
	this->width = width;
	this->height = height;
	
	int size = width*height;
	xvelocity = new float[size];
	yvelocity = new float[size];
	xvelocityMid = new float[size];
	yvelocityMid = new float[size];
	xvelocityOut = new float[size];
	yvelocityOut = new float[size];
	pressure = new float[size];
	pressureOut = new float[size];
	ink = new float[size];
	inkOut = new float[size];
	heat = new float[size];
	heatOut = new float[size];
	sources = new int[size];
	sourceFractions = new float[size*4];
	fraction = new float[size];
	balance = new float[size];
	BFECC = new float[size];
	Reset();
	
	// add a blob for initial tests
	
	int blobx = 128;
	int bloby = 128;
	
	for (int x = 0; x<width;x++)
	{
		for (int y=0;y<height;y++)
		{
			if (x == blobx && y == bloby)
				pressure[Cell(x,y)] = 0.0f;
			else
				pressure[Cell(x,y)] = 0.0f/(sqrtf((blobx-x)*(bloby-x) + (blobx-y)*(bloby-y))*0.02f);
			
			ink[Cell(x,y)] = 0.5f;
		}
		
		//pressure[Cell(30,x)] = 1;
	}
	
	diffusionIterations = 1;
	velocityDiffusion = 1.0f;
	
	// With no pressure diffusion, waves of pressure keep moving
	// but only if pressure is advected ahead of velocity
	pressureDiffusion = 2.0f;
	
	heatDiffusion = 1.0f;
	inkDiffusion = 0.2f;
	
	// friction applied to velocity.  As a simple fraction each time step
	// so 1.0f will give you no friction
	
	// Friction is very important to the realism of the simulation
	// changes to these coeffficinets will GREATLY affect the apperence of the simulation.
	// the high factor friction (a) wich is applied to the square of the velocity,
	// acts as a smooth limit to velocity, which causes streams to break up into turbulent flow
	// The c term will make things stop, which then shows up our border artefacts
	// but a high enough (>0.01) c term is needed to stop things just dissipating too fast
	velocityFriction_a = 0.5f;
	velocityFriction_b = 0.2f;
	velocityFriction_c = 0.001f;
	
	vorticity = 2.0f;
	
	// Pressure accelleration. Larger values (> 0.5) are more realistic (like water)
	// values that are too large lead to chaotic waves
	pressureAcc = 0.2;
	
	// Upwards force applied by ink
	// heat follows ink in advection, but is a separate scalar value
	inkHeat = 0.0f;
	heatForce = -0.01f;
	
	heatFriction_a = 0.2;
	heatFriction_b = 0.2f;
	heatFriction_c = 0.01f;
	
	// High ink advection allows fast moving nice swirlys
	inkAdvection = 150.0f;
	
	// seems nice if both velocity and pressure at same value, which makes sense
	velocityAdvection = 150.0f;
	pressureAdvection = 130.0f;
	heatAdvection = 150.0f;
}

void Fluid::Reset() {
	int size = width*height;
	ZeroField(xvelocity);
	ZeroField(yvelocity);
	ZeroField(xvelocityMid);
	ZeroField(yvelocityMid);
	ZeroField(xvelocityOut);
	ZeroField(yvelocityOut);
	
	ZeroField(pressureOut);
	ZeroField(ink);
	ZeroField(inkOut);
	ZeroField(heat);
	ZeroField(heatOut);
	
	//ZeroField(pressure);
	// should be 1.0f, lower is more like gas, higher less compressible
	for (int i=0;i<size;i++) pressure[i] = 1.0f;
}

void Fluid::Update(float deltaTime) {
	this->deltaTime = deltaTime;

#if 1
	if (velocityDiffusion != 0.0f) {
		for (int i=0;i<diffusionIterations;i++) {
			Diffusion(xvelocity, xvelocityMid, velocityDiffusion/(float)diffusionIterations);
			std::swap(xvelocity,xvelocityMid);
			Diffusion(yvelocity, yvelocityMid, velocityDiffusion/(float)diffusionIterations);
			std::swap(yvelocity,yvelocityMid);
		}
	}
	

	if (pressureDiffusion != 0.0f) {
		for (int i=0;i<diffusionIterations;i++) {
			Diffusion(pressure, pressureOut, pressureDiffusion/(float)diffusionIterations);
			std::swap(pressure,pressureOut);
		}
	}
	
	if (heatDiffusion != 0.0f) {
		for (int i=0;i<diffusionIterations;i++) {
			Diffusion(heat, heatOut, heatDiffusion/(float)diffusionIterations);
			std::swap(heat,heatOut);
		}
	}
	
	if (inkDiffusion != 0.0f) {
		for (int i=0;i<diffusionIterations;i++) {
			Diffusion(ink, inkOut, inkDiffusion/(float)diffusionIterations);
			std::swap(ink, inkOut);
		}
	}
	
#endif
	
	if (inkHeat != 0.0f) {
		// TO make smoke rise under its own steam, we make the ink apply an
		// upwards force on the velocity field
		// heat froce from the ink
		ForceFrom(ink,yvelocity,inkHeat);
		//ForceFrom(mp_p0,mp_yv0,ink_heat);
		// Optional Kill edge ink, so smoke edits to the top
		//ZeroEdge(mp_ink0);
		
		// Fade ink
		//	ForceFrom(mp_ink0,mp_ink0,-0.1f);
	}
	
#if 1
	if (heatForce != 0.0f)// && !m_field_test)
	{
		// TO make smoke rise under its own steam, we make the ink apply an
		// upwards force on the velocity field
		// heat froce from the ink
		ForceFrom(heat,yvelocity,heatForce);
		//if (this == &g_fluid1)
		//ForceFrom(mp_heat0,mp_xv0,m_heat_force);
		// Kill edge ink, so smoke edits to the top
		//ZeroEdge(mp_ink0);
		
		// Cooling effect, otherwise things just explode
		//ForceFrom(mp_heat0,mp_heat0,-0.3f);
		if (heatFriction_a !=0 || heatFriction_b !=0 || heatFriction_c != 0) {
			QuadraticDecay(heat, heat, heatFriction_a, heatFriction_b, heatFriction_c);
		}
	}
#endif
	
	//ApplyForce(mp_yv0,0.01f);
	// Give a litte random force to the edges of the simulation
	// just so things preturb a little
	//EdgeForce(mp_xv0,rndf(0.01),-rndf(0.01),rndf(0.01),-rndf(0.01));
	//EdgeForce(mp_yv0,rndf(0.001),-rndf(0.001),rndf(0.001),-rndf(0.001));
	
	// Vorticity confinement is a cosmetic patch that accellerates the velocity
	// field in a direction tangential to the curve defined by the surrounding points
	if (vorticity != 0.0f)
		VorticityConfinement(vorticity);
	
	// No major diff if we swap the order of advancing pressure acc and friction forces
	if (pressureAcc != 0.0f)// && !m_field_test)
	{
		PressureAcceleration(pressureAcc);
	}
	
	if (velocityFriction_a != 0.0f || velocityFriction_b != 0.0f || velocityFriction_c != 0.0f)
	{
		VelocityFriction(velocityFriction_a, velocityFriction_b, velocityFriction_c);
	}
	
	// Clamping Veclocity prevents problems when too much energy is introduced into the system
	// it's not strictly necessary, and in fact leads to problems as it gives you a leading edge wave of
	// constant velocity, which cannot dissipate
	//	if (this == &g_fluid2)
	//	ClampVelocity(0.2f);
	
	/*float before_p = 0;
	float tot_p = 0;
	
	float v_tot = 0.0f;
	float v_max = 0.0f;
	
	for (int y=0;y<height;y++) {
		for (int x=0;x<width;x++) {
			int cell = Cell(x,y);
			
			before_p += pressure[cell];
			
			float vx = xvelocity[cell];
			float vy = yvelocity[cell];
			float v_len = sqrt(vx*vx + vy*vy);
			
			if (v_len > v_max)
				v_max = v_len;
			v_tot += v_len;
		}
	}*/
	
	// Advection step.
	// Forward advection works well in general, but does not handle the dissipation
	// of single cells of pressure (or lines of cells).
	//
	// Reverse advection does not handle self-advection of velocity without diffusion
	//
	// Both?  Beautiful!!!
	
	// Problems seems to be getting aswirly velocity field
	// and the pressure accumulating at the edges
	// and the artefacts of "ripples".  Or are they natural?
	//
	// Maybe try setting up a smoke source simulation, as that looks nice.
	
	// The advection scale is needed for when we change grid sizes
	// smaller grids mean large cells, so scale should be smaller
	float advectionScale = width / 100.f;
	
#if 1
	//SetField(inkOut,1.0f);
	//ForwardAdvection(inkOut,balance,inkAdvection * advectionScale);
	//if (this == &g_fluid2)
	//{
		// scaling the velocity by the advection balance actually
		// helps reduce the compressibility of the velocity field
		// But not really in a useful way, as it's just reducing the energy at
		// points that have high velocty
		//		MulFields(mp_balance,mp_yv0,mp_yv0);
		//		MulFields(mp_balance,mp_xv0,mp_xv0);
	//}

	
	// Advect the ink - ink is one fluid suspended in another, like smoke in air
	ForwardAdvection(ink, inkOut, inkAdvection * advectionScale);
	std::swap(ink,inkOut);
	ReverseAdvection(ink, inkOut, inkAdvection * advectionScale);
	std::swap(ink,inkOut);
	
	// Only advect the heat if it is applying a force
	if (heatForce != 0.0f)
	{
		ForwardAdvection(heat, heatOut, heatAdvection * advectionScale);
		std::swap(heat,heatOut);
		ReverseAdvection(heat, heatOut, heatAdvection * advectionScale);
		std::swap(heat,heatOut);
	}
	
#endif
	
	// Advection order makes very significant differences
	// If presure is advected first, it leads to self-maintaining waves
	// and ripple artifacts
	// So the "velocity first" seems preferable as it naturally dissipates the waves
	// By advecting the velocity first we advect the pressure using the next frames velocity
	//
	
#if 1
	// Self advect the velocity via three buffers (if reverse advecting)
	// buffers are 0,1 and 2
	// our current velocity is in 0
	// we advec t 0 to 1 using 0
	// we the  advect 1 to 2 using 0 again
	// then swap 2 into 0 so it becomes the new current velocity
	ForwardAdvection(xvelocity, xvelocityMid, velocityAdvection * advectionScale);
	ForwardAdvection(yvelocity, yvelocityMid, velocityAdvection * advectionScale);
#if 1
	// Advect from 1 into 2, then swap 0 and 2
	// We can use signed reverse advection as quantities can be negative.
	ReverseSignedAdvection(xvelocityMid, xvelocityOut, velocityAdvection * advectionScale);
	ReverseSignedAdvection(yvelocityMid, yvelocityOut, velocityAdvection * advectionScale);
	std::swap(xvelocityOut,xvelocity);
	std::swap(yvelocityOut,yvelocity);
#else
	std::swap(xvelocityMid,xvelocity);
	std::swap(yvelocityMid,yvelocity);
	//return;
#endif
	// handle velocities at the edge, confining them to within the cells.
	// Not needed with correct edge sampling and pressure, as edge pressure will turn the vel
	// But since we have several
	//if (m_pressure_acc == 0.0f || this == &g_fluid1)
	//{
	//	ZeroEdge(mp_xv0);
	//	ZeroEdge(mp_yv0);
	//	ZeroEdge(mp_p0);
	
	//}
	EdgeVelocities();
#endif
	
	
	ForwardAdvection(pressure, pressureOut, pressureAdvection * advectionScale);
	std::swap(pressureOut, pressure);
	
	ReverseAdvection(pressure, pressureOut, pressureAdvection * advectionScale);
	std::swap(pressureOut, pressure);
}

// Any velocities that are facing outwards at the edge cells, face them inwards
void Fluid::EdgeVelocities() {
	for (int x=0;x<width;x++) {
		int cell = Cell(x,0);
		if (yvelocity[cell] < 0.0f) yvelocity[cell] = -yvelocity[cell];
		cell = Cell(x,height-1);
		if (yvelocity[cell] > 0.0f) yvelocity[cell] = -yvelocity[cell];
	}
	
	for (int y=0;y<height;y++) {
		int cell = Cell(0,y);
		if (xvelocity[cell] < 0.0f) xvelocity[cell] = -xvelocity[cell];
		cell = Cell(width-1,y);
		if (xvelocity[cell] > 0.0f) xvelocity[cell] = -xvelocity[cell];
	}
}

// Signed advection is mass conserving, but allows signed quantities
// so could be used for velocity, since it's faster.
void Fluid::ReverseSignedAdvection(float *p_in, float *p_out, float scale)
{
	// negate advection scale, since it's reverse advection
	float a = -deltaTime * scale ;
	
	int size = width*height;
	
	// First copy the scalar values over, since we are adding/subtracting in values, not moving things
	memcpy((void*)p_out, (void*)p_in, size*sizeof(float));
	
	// We do all the cells,
	for (int y=0;y<height;y++) {
		for (int x=0;x<width;x++) {
			int cell = Cell(x,y);
			float vx = xvelocity[cell];
			float vy = yvelocity[cell];
			if (vx != 0.0f || vy != 0.0f)
			{
				float x2 = x + vx * a;
				float y2 = y + vy * a;
				Collide(x,y,x2,y2);
				assert ((int)x2 >= 0 && (int)x2 < width);
				assert ((int)y2 >= 0 && (int)y2 < height);
				
				int cell2 = Cell((int)x2,(int)y2);
				
				// get fractional parts
				float fx = x2-(int)x2;
				float fy = y2-(int)y2;
				
				// Get amounts from (in) source cells
				float ia = (1.0f-fy)*(1.0f-fx) * p_in[cell2];
				float ib = (1.0f-fy)*(fx)      * p_in[cell2+1];
				float ic = (fy)     *(1.0f-fx) * p_in[cell2+width];
				float id = (fy)     *(fx)      * p_in[cell2+width+1];
				
				// add to (out) dest cell
				p_out[cell] += ia + ib + ic + id ;
				// and subtract from (out) dest cells
				p_out[cell2]		-= ia;
				p_out[cell2+1]		-= ib;
				p_out[cell2+width]  -= ic;
				p_out[cell2+width+1]-= id;
				
			}
		}
	}
}

void Fluid::ReverseAdvection(float *p_in, float *p_out, float scale) {
	// negate advection scale, since it's reverse advection
	float a= - deltaTime * scale;
	
	int size = width*height;
	
	// First copy the scalar values over, since we are adding/subtracting in values, not moving things
	memcpy((void*)p_out,p_in,size*sizeof(float));
	
	//First we need to zero out the fractions
	ZeroField(fraction);
	
	//We do all the cells
	for (int y=0;y<height;y++) {
		for (int x=0;x<height;x++) {
			int cell = Cell(x,y);
			float vx = xvelocity[cell];
			float vy = yvelocity[cell];
			if (vx != 0.0f || vy != 0.0f) {
				float x2 = x + vx*a;
				float y2 = y + vy*a;
				Collide(x, y, x2, y2);
				
				assert((int)x2 >= 0 && (int)x2 < width);
				assert((int)y2 >= 0 && (int)y2 < height);
				
				int cell2 = Cell((int)x2,(int)y2);
				
				// Get fractional parts
				float fx = x2 - (int)x2;
				float fy = y2 - (int)y2;
				
				// we are taking pressure from four cells (one of which might be the target cell
				// and adding it to the target cell
				// hence we need to subtract the appropiate amounts from each cell
				//
				// Cells are like
				//
				//    A----B
				//    |    |
				//    |    |
				//    C----D
				//
				// (Should be square)
				// so we work out the bilinear fraction at each of a,b,c,d
				// NOTE: Using p1 here, to ensure consistency
				
				/*
				 
By adding the source value into the destination, we handle the problem of multiple destinations
but by subtracting it from the source we gloss over the problem of multiple sources.
Suppose multiple destinations have the same (partial) source cells, then what happens is the first dest that
is processed will get all of that source cell (or all of the fraction it needs).  Subsequent dest
cells will get a reduced fraction.  In extreme cases this will lead to holes forming based on
the update order.

Solution:  Maintain an array for dest cells, and source cells.
For dest cells, store the four source cells and the four fractions
For source cells, store the number of dest cells that source from here, and the total fraction
E.G.  Dest cells A, B, C all source from cell D (and explicit others XYZ, which we don't need to store)
So, dest cells store A->D(0.1)XYZ..., B->D(0.5)XYZ.... C->D(0.7)XYZ...
Source Cell D is updated with A, B then C
Update A:   Dests = 1, Tot = 0.1
Update B:   Dests = 2, Tot = 0.6
Update C:   Dests = 3, Tot = 1.3

How much should go to each of A, B and C? They are asking for a total of 1.3, so should they get it all, or
should they just get 0.4333 in total?
Ad Hoc answer:
if total <=1 then they get what they ask for
if total >1 then is is divided between them proportionally.
If there were two at 1.0, they would get 0.5 each
If there were two at 0.5, they would get 0.5 each
If there were two at 0.1, they would get 0.1 each
If there were one at 0.6 and one at 0.8, they would get 0.6/1.4 and 0.8/1.4  (0.429 and 0.571) each

So in our example, total is 1.3,
A gets 0.1/1.3, B gets 0.6/1.3 C gets 0.7/1.3, all totalling 1.0

SO we need additional arrays
int mp_sources[size]
int mp_source_fractions[size*4]
float mp_fraction[size]
				 
				 */
				
				// Just calculating fractions for now
				float ia = (1.0f-fy)*(1.0f-fx);
				float ib = (1.0f-fy)*(fx)     ;
				float ic = (fy)     *(1.0f-fx);
				float id = (fy)     *(fx)     ;
				// Storing the source information for this dest cell (cell)
				sources[cell] = cell2;  // that's cell1, implying cell1+1, cell1+m_w and cell1+1+m_w
				// and the fractions, unless they are all zero
				// which is quite possible if advecting something sparse
				// but since we optimize for a more likely case, this test wastes time
				//if (ia !=0.0f || ib!=0.0f || ic !=0.0f || id != 0.0f)
				{
					sourceFractions[cell*4+0] = ia;
					sourceFractions[cell*4+1] = ib;
					sourceFractions[cell*4+2] = ic;
					sourceFractions[cell*4+3] = id;
					
					// Accumullting the fractions for the four sources
					fraction[cell2]			+= ia;
					fraction[cell2+1]		+= ib;
					fraction[cell2+width]	+= ic;
					fraction[cell2+width+1]	+= id;
				}
			} else {
				// the cell has a zero velocity, advecting from itself, flag that and optimize it out later
				sources[cell] = -1;
			}
		}
	}
	
	// With nice advection, each cell has four sources and four source fractions
	
	for (int cell=0;cell < size;cell++) {
		// get the block of four source cells
		int cell2 = sources[cell];
		
		//Optmized out (see above)
		if (cell2 != -1) {
			// Get the four fractional amounts we earlier interpolated
			float ia = sourceFractions[cell*4+0]; // Using cell and not cell2 as it's an array of four values and not the grid of four
			float ib = sourceFractions[cell*4+1];
			float ic = sourceFractions[cell*4+2];
			float id = sourceFractions[cell*4+3];
			
			// get the TOTAL fraction requested from each source cell
			float fa = fraction[cell2];
			float fb = fraction[cell2+1];
			float fc = fraction[cell2+width];
			float fd = fraction[cell2+1+width];
			
			//If less than 1.0 in total, we can have all we want
			if (fa < 1.0f) fa = 1.0f;
			if (fb < 1.0f) fb = 1.0f;
			if (fc < 1.0f) fc = 1.0f;
			if (fd < 1.0f) fd = 1.0f;
			
			//scale the amount we are transferring
			ia /= fa;
			ib /= fb;
			ic /= fc;
			id /= fd;
			
			// Give the fraction of the original source, do not alter the original
			// So we are taking fractions from p_in, but not altering those values
			// as they are used again by later cells
			// if the field were mass conserving, then we could simply move the value
			// but if we try that we lose mass
			p_out[cell] += ia * p_in[cell2] + ib * p_in[cell2+1] + ic * p_in[cell2+width] + id * p_in[cell2+1+width];
			
			// The accounting happens here, the values we added to the dest in p1 are subtracted from the sources in p1
			p_out[cell2]		-= ia * p_in[cell2];
			p_out[cell2+1]		-= ib * p_in[cell2+1];
			p_out[cell2+width]	-= ic * p_in[cell2+width];
			p_out[cell2+1+width]-= id * p_in[cell2+1+width];
		}
			
	}
}
// Move scalar along the velocity field
// Forward advection moves the value at a point forward along the vector from the same point
// and dissipates it between four points as needed
void Fluid::ForwardAdvection(float *p_in, float *p_out, float scale) {
	
	// Pressure differential between points
	// to get an accelleration force.
	float a = deltaTime * scale;
	
	memcpy((void *)p_out, (void*)p_in, width*height*sizeof(float));
	
	if (scale==0.0f) return;
	
	// We advect all cells
	for (int y=0;y<height;y++) {
		for (int x=0;x<width;x++) {
			int cell = Cell(x,y);
			float vx = xvelocity[cell];
			float vy = yvelocity[cell];
			if (vx != 0.0f || vy != 0.0f) {
				float x2 = x + (vx * a);
				float y2 = y + (vy * a);
				Collide(x,y,x2,y2);
				
				int cell2 = Cell((int)x2,(int)y2);
				
				//get fractional parts
				float fx = x2 - (int)x2;
				float fy = y2 - (int)y2;
				
				// we are taking pressure from four cells (one of which might be the target cell
				// and adding it to the target cell
				// hence we need to subtract the appropiate amounts from each cell
				//
				// Cells are like
				//
				//    A----B
				//    |    |
				//    |    |
				//    C----D
				//
				// (Should be square)
				// so we work out the bilinear fraction at each of a,b,c,d
				
				float in = p_in[cell];
				
				float ia = (1.0f-fy)*(1.0f-fx) * in;
				float ib = (1.0f-fy)*(fx)      * in;
				float ic = (fy)     *(1.0f-fx) * in;
				float id = (fy)     *(fx)      * in;
				
				// Subtract them from the source cell
				p_out[cell] -= (ia+ib+ic+id);
				// Then add them to the four destination cells
				p_out[cell2]		+= ia;
				p_out[cell2+1]		+= ib;
				p_out[cell2+width]	+= ic;
				p_out[cell2+width+1]+= id;
			}
		}
	}
}

void Fluid::Collide(float x, float y, float &x2, float &y2) {
	float leftBound = width - 1.0001f;
	float botBound = height - 1.0001f;
	
	//IF NO WRAPPING
	
	//Reflection at edges
	if (x2 < 0) x2 = -x2;
	else if (x2 > leftBound) x2 = leftBound - (x2 - leftBound);
	//if (x2 > leftBound) x2 = leftBound;
	//if (x2 < 0) x2 = 0;
	
	if (y2 < 0) y2 = -y2;
	else if (y2 > botBound) y2 = botBound - (y2 - botBound);
	//if (y2 > botBound) y2 = botBound;
	//if (y2 < 0) y2 = 0;
	
	assert(y2 <= botBound);
}

// The force of pressure affects velocity
// Exchanging with a normal??
void Fluid::VelocityFriction(float a, float b, float c) {
	
	a *= deltaTime;
	b *= deltaTime;
	c *= deltaTime;
	
	for (int i=0; i < 1; i++) {
		// NOTE: We include the border cells in global friction
		for (int y=0;y<height;y++) {
			for (int x=0;x<width;x++) {
				int cell = Cell(x, y);
				
				float vx = xvelocity[cell];
				float vy = yvelocity[cell];
				
				float len2 = vx*vx + vy*vy;
				float len = std::sqrt(len2);
				//float sign = 1.0f;
				//Since when did sqrt return negative values?? skipping that code
				
				float a = a*len2 + b*len + c;
				
				if (len < 0.0f) len = 0.0f;
				a /= len + 0.0001f;
				if (a > 1.0f) a = 1.0f;
				
				//Normal
				float nvx = -vy*a + vx*(1.0f-a);//-vy*len;
				float nvy = vx*a + vy*(1.0f-a);
				
				xvelocity[cell] = nvx;
				yvelocity[cell] = nvy;
			}
		}
	}
}

// NEW - treat cells pairwise, which allows us to handle edge cells
// updates ALL cells
// \todo Doesn't actually need two buffers with this code
void Fluid::PressureAcceleration(float force) {
	
	// Pressure differential between points
	// to get an accelleration force.
	float a = deltaTime * force;
	
	// First copy the values
	/*for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {
			int cell = Cell(x,y);
			mp_xv1[cell] = mp_xv0[cell];
			mp_yv1[cell] = mp_yv0[cell];
		}
	}*/
	memcpy ((void*)xvelocityOut, (void*)xvelocity, width*height*sizeof(float));
	memcpy ((void*)yvelocityOut, (void*)yvelocity, width*height*sizeof(float));
	
	//If no fluid wrapping
	for (int y=0;y<height-1;y++) {
		for (int x=0;x<width-1;x++) {
			int cell = Cell(x,y);
			float forcex = pressure[cell] - pressure[cell+1];
			float forcey = pressure[cell] - pressure[cell+width];
			
			// forces act in same direction  on both cells
			xvelocityOut[cell]		+= a * forcex;
			xvelocityOut[cell+1]	+= a * forcex;
			
			yvelocityOut[cell]		+= a * forcey;
			yvelocityOut[cell+width]+= a * forcey;
			//In original source, more code to test here
		}
	}
	
	std::swap(xvelocityOut, xvelocity);
	std::swap(yvelocityOut, yvelocity);
}

/**
 * Calculate the curl at position (i, j) in the fluid grid.
 * Physically this represents the vortex strength at the
 * cell. Computed as follows: w = (del x U) where U is the
 * velocity vector at (i, j).
 *
 **/

float Fluid::Curl(int x, int y)
{
	// difference in XV of cells above and below
    // positive number is a counter-clockwise rotation
	float xcurl = (xvelocity[Cell(x, y + 1)] - xvelocity[Cell(x, y - 1)]) * 0.5f;
	
	// difference in YV of cells left and right
    // positive number is a counter-clockwise rotation
    float ycurl = (yvelocity[Cell(x + 1, y)] - yvelocity[Cell(x - 1, y)]) * 0.5f;
	
    return xcurl - ycurl;
}

void Fluid::VorticityConfinement(float scale) {
	
	ZeroEdge(pressureOut);
	ZeroField(xvelocityOut);
	ZeroField(yvelocityOut);
	
	float *p_abs_curl = pressureOut;
	
	for (int y=1;y < height-1; y++) {
		for (int x=1;x < width-1; x++) {
			p_abs_curl[Cell(x,y)] = std::abs(Curl(x,y));
		}
	}
	
	for (int y=2;y < height-2; y++) {
		for (int x=2;x < width-2; x++) {
			int cell = Cell(x,y);
			
			// get curl gradient across this cell, left right
            float lr_curl = (p_abs_curl[cell+1] - p_abs_curl[cell-1]) * 0.5f;
			// and up down
            float ud_curl = (p_abs_curl[cell+width] - p_abs_curl[cell+height]) * 0.5f;
			
			// Normalize the derivitive curl vector
            float length = (float)sqrt(lr_curl * lr_curl + ud_curl * ud_curl) + 0.000001f;
            lr_curl /= length;
            ud_curl /= length;
			
			// get the magnitude of the curl
            float v = Curl(x, y);
			
			// (lr,ud) would be perpendicular, so (-ud,lr) is tangential?
            xvelocityOut[cell] = -ud_curl *  v;
			yvelocityOut[cell] = lr_curl *  v;
		}
	}
	
	ForceFrom(xvelocityOut,xvelocity,scale);
	ForceFrom(yvelocityOut,yvelocity,scale);
}

// Decay a positive value towards zero via a quadratic function
// in and out can be the same array without interference
void Fluid::QuadraticDecay(float *p_in, float *p_out, float a, float b, float c) {
	a *= deltaTime;
	b *= deltaTime;
	c *= deltaTime;
	
	int size = width*height;
	for (int cell=0;cell<size;cell++)
	{
		float v = p_in[cell];
		float v2 = v*v;
		//v-= dt*(a*v2 + b*v + c);
		v -= a*v2 + b*v + c;
		
		//Clamp
		if (v < 0.0f) v = 0.0f;
		
		p_out[cell] = v;
	}
}

void Fluid::ZeroField(float *p_in) {
	int size = width*height;
	memset((void*)p_in,0,size*sizeof(float));
}

void Fluid::SetField(float *p_in, float f) {
	int size = width*height;
	std::fill(p_in, p_in+size, f);
	//for (int i=0;i<size;i++) p_in[i] = f;
}

void Fluid::ZeroEdge(float *p_in) {
	
	for (int x=0;x<width;x++) {
		p_in[x] = 0;
		p_in[width*(height-1) + x] = 0;
	}
	
	//Clear y, need not clear the corners again
	for (int y=1;y<width-1;y++) {
		p_in[y*width] = 0;
		p_in[y*width + width-1] = 0;
	}
}

// Given a field p_from, and a field p_to, then add f*p_from to p_to
// can be used to apply a heat field to velocity
void Fluid::ForceFrom(float *p_from, float *p_to, float f) {
	f *= deltaTime;
	int size = width * height;
	for (int cell = 0; cell < size; cell++)
	{
		(*p_to++) += (*p_from++)*f;
	}
}

void Fluid::Diffusion(float *p_in, float *p_out, float scale) {
	
	float a = scale * deltaTime;
	
	int cell;
	
	for (int x=1;x < width-1;x++) {
		cell = Cell(x,0);
		p_out[cell] = p_in[cell] + a * (p_in[cell-1] + p_in[cell+1] + p_in[cell+width] - 3.0f*p_in[cell]);
		
		cell = Cell(x,height-1);
		p_out[cell] = p_in[cell] + a * (p_in[cell-1] + p_in[cell+1] + p_in[cell-width] - 3.0f*p_in[cell]);
	}
	
	for (int y=1;y < height-1;y++) {
		cell = Cell(0,y);
		p_out[cell] = p_in[cell] + a * (p_in[cell-width] + p_in[cell+1] + p_in[cell+width] - 3.0f*p_in[cell]);
		cell = Cell(width-1,y);
		p_out[cell] = p_in[cell] + a * (p_in[cell-width] + p_in[cell-1] + p_in[cell+width] - 3.0f*p_in[cell]);
	}
	
	cell = Cell(0,0);
	p_out[cell] = p_in[cell] + a * (p_in[cell+1] + p_in[cell+width] - 2.0f*p_in[cell]);
	cell = Cell(width-1,0);
	p_out[cell] = p_in[cell] + a * (p_in[cell-1] + p_in[cell+width] - 2.0f*p_in[cell]);
	cell = Cell(0,height-1);
	p_out[cell] = p_in[cell] + a * (p_in[cell+1] + p_in[cell-width] - 2.0f*p_in[cell]);
	cell = Cell(width-1,height-1);
	p_out[cell] = p_in[cell] + a * (p_in[cell-1] + p_in[cell-width] - 2.0f*p_in[cell]);
	
	
	for (int y=1;y<height-1;y++) {
		for (int x=1;x<width-1;x++) {
			cell = Cell(x,y);
			p_out[cell] = p_in[cell] + a * (p_in[cell+1] + p_in[cell-1] + p_in[cell+width] + p_in[cell-width] - 4.0f*p_in[cell]);
		}
	}
}

void Fluid::Paint(float* arr, int x0, int y0, int r, float str) {
	int xmin = x0-r;
	int xmax = x0+r;
	int ymin = y0-r;
	int ymax = y0+r;
	xmin = xmin < 0 ? 0 : (xmin > width ? width : xmin);
	xmax = xmax < 0 ? 0 : (xmax > width ? width : xmax);
	ymin = ymin < 0 ? 0 : (ymin > width ? width : ymin);
	ymax = ymax < 0 ? 0 : (ymax > width ? width : ymax);
	for (int y=ymin;y<ymax;y++) {
		for (int x=xmin;x<xmax;x++) {
			float v = (r-sqrt((float)((y-y0)*(y-y0) + (x-x0)*(x-x0))));
			if (v > 0) arr[Cell(x,y)] += str * (1.0/r)*v;
		}
	}
}