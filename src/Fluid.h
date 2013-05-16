//
//  Fluid.h
//  Platformer
//
//  Created by Aron Granberg on 2013-05-16.
//
//

#ifndef __Platformer__Fluid__
#define __Platformer__Fluid__

#include <iostream>
#include "SFML/Graphics.hpp"
class Fluid {
	
	
	
	
public:
	Fluid ();
	
	void Init(int width, int height);
	void ReverseAdvection(float *p_in, float *p_out, float scale);
	void ReverseSignedAdvection(float *p_in, float *p_out, float scale);
	void ForwardAdvection(float *p_in, float *p_out, float scale);
	void PartialForwardAdvection(float *p_in, float *p_out, float scale, float partial);
	void BFECCForwardAdvection(float *p_in, float*p_out, float scale);
	void PressureAcceleration(float force);
	void Diffusion(float *p_in, float *p_out, float scale);
	void GlobalDiffusion(float *p_in, float *p_out, float scale);
	void DiffusionLR(float *p_in, float *p_out, float scale);
	void DiffusionUD(float *p_in, float *p_out, float scale);
	void VelocityFriction(float a, float b, float c);
	void QuadraticDecay(float *p_in, float *p_out, float a, float b, float c);
	void ClampVelocity(float max_v);
	void Render(float x, float y, float w, float h);
	void Update(float dt);
	void MouseInput();
	void Reset();
	void ZeroEdge(float *p_in);
	void PushEdge(float *p_in);
	void CopyEdge(float *p_in, float *p_out);
	void CopyField(float *p_in, float *p_out);
	void AddFields(float *p_a, float *p_b, float *p_out);
	void SubFields(float *p_a, float *p_b, float *p_out);
	void AddField(float *p_a, float f, float *p_out);
	void MulField(float *p_a, float f, float *p_out);
	void MulFields(float *p_a, float *p_b, float *p_out);
	void SetField(float *p_a, float f);
	void ZeroField(float *p_field);
	void AddValue(float *p_in, float x, float y, float v);
	float GetValue(float *p_in, float x, float y);
	void SetValue(float *p_in, float x, float y, float v);
	void EdgeVelocities();
	void ForceFrom(float *p_from, float *p_to, float f);
	void ApplyForce(float *p_to, float f);
	void EdgeForce(float *p_to, float left, float right, float top, float bot);
	
	void Paint(float* arr, int x0, int y0, int r, float str);
	float Curl(int i, int j);
	void VorticityConfinement(float scale);
	inline void Collide(float x,float y, float &x2, float &y2);
	
	int width, height;
	float deltaTime;
	
	inline int Cell (int x, int y) {
		return y*width + x;
	}
	
	float* xposition;
	float* yposition;
	float* xvelocity;
	float* yvelocity;
	
	float* xpositionMid;
	float* ypositionMid;
	float* xvelocityMid;
	float* yvelocityMid;
	
	float* xpositionOut;
	float* ypositionOut;
	float* xvelocityOut;
	float* yvelocityOut;
	
	float* pressure;
	float* heat;
	float* ink;
	
	float* pressureOut;
	float* heatOut;
	float* inkOut;
	
	// temp accumulators of data for advection
	int*		sources;
	float*		sourceFractions;
	float*		fraction;
	
	// BFECC needs its own buffer
	// can't reuse other, since they might be used by the advection step
	float*		BFECC;
	
	float*		balance;
	
	int m_init;
	int m_last_x;
	int m_last_y;
	
	
	// "Physical" constants. "Constants" begin with a capital letter
	float velocityDiffusion;
	
	int	diffusionIterations;
	
	// With no pressure diffusion, waves of pressure keep moving
	// But only if pressure is advected ahead of velocity
	float pressureDiffusion;
	
	float heatDiffusion;
	
	float inkDiffusion;
	
	// friction applied to velocity.  As a simple fraction each time step
	// so 1.0f will give you no friction
	
	// Friction is very important to the realism of the simulation
	// changes to these coeffficinets will GREATLY affect the apperence of the simulation.
	// the high factor friction (a) wich is applied to the square of the velocity,
	// acts as a smooth limit to velocity, which causes streams to break up into turbulent flow
	// The c term will make things stop, which then shows up our border artefacts
	// but a high enough (>0.01) c term is needed to stop things just dissipating too fast
	// Friction is dt*(a*v^2 + b*v + c)
	float velocityFriction_a;
	float velocityFriction_b;
	float velocityFriction_c;
	
	// Pressure accelleration.  Larger values (>10) are more realistic (like water)
	// values that are too large lead to chaotic waves
	float vorticity;
	float pressureAcc;
	float inkHeat;
	float heatForce;
	float heatFriction_a;
	float heatFriction_b;
	float heatFriction_c;
	// high ink advection allows fast moving nice swirlys
	float inkAdvection;
	// seems nice if both velocity and pressure at same value, which makes sense
	float velocityAdvection;
	float pressureAdvection;
	float heatAdvection;
	
};
#endif /* defined(__Platformer__Fluid__) */
