#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	Vdesired = targetPos - actorPos;


	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	Vdesired = actorPos - targetPos;



	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos =  m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	std::cout << targetPos << std::endl;
	std::cout << actorPos << std::endl;
	std::cout << std::endl;

	Vdesired = (targetPos - actorPos) * actor->KArrival;

	// TODO: add your code here to compute Vdesired
	/*
	int maxDistance = 1000;
	if (Distance(targetPos, actorPos) < 300.0) {
		Vdesired = vec3(0,0,0);
	}
	else {
		Vdesired = (targetPos - actorPos);// *(1.0 * Distance(targetPos, actorPos) / maxDistance);
	}*/




	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 e = targetPos - actorPos;
	Vdesired = actor->KDeparture * -e / (e.Length() * e.Length());





	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel( BehaviorController* actor)
{

	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

	//TODO: add your code here
	vec3 Varrival(0, 0, 0);
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target
	Vdesired = (m_pTarget->getLocalTranslation() - m_actorPos) * (actor->KAvoid);
	Varrival = Vdesired;



	vec3 Vavoid(0, 0, 0);
	//TODO: add your code here to compute Vavoid 



	// Step 2. compute Lb
	//TODO: add your code here
	float lb = m_actorVel.Length() * actor->TAvoid;


	// Step 3. find closest obstacle 
	//TODO: add your code here
	vec3 bestDistance = vec3(numeric_limits<float>::infinity(), 0.0f, numeric_limits<float>::infinity());
	float bestRadius = 0;
	for (int obstacle = 0; obstacle < mObstacles->size(); obstacle++) {
		if ((((*mObstacles)[obstacle]).m_Center.getLocalTranslation() - m_actorPos).Length() < bestDistance.Length()) {
			bestDistance = (((*mObstacles)[obstacle]).m_Center.getLocalTranslation()).Length();
			bestRadius = ((*mObstacles)[obstacle]).m_Radius;
		}
	}

	vec3 angle = actor->getOrientation();

	mat3 rotation;
	rotation.FromAxisAngle(vec3(0, 1, 0), angle[1] * M_PI / 180.0f);	

	vec3 l = rotation.Inverse() * bestDistance;

	float rb = actor->gAgentRadius;
	float ro = bestRadius;

	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TODO: add your code here
	if (abs(l[0]) <= m_actorVel.Length() * BehaviorController::TAvoid + (ro + rb) && abs(l[2]) <= (rb + ro)) {

		vec3 cap = vec3(-l[0], 0, 0);
		cap.Normalize();

		vec3 Vavoid(0, 0, 0);
		Vavoid = cap * (actor->KAvoid * ((rb + ro) - l[0]) / (rb + ro));

		Vdesired = Vavoid + Varrival;
	}


	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TODO: add your code here




	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// compute Vdesired = Vwander

	// Step. 1 find a random direction
	//TODO: add your code here
	float randX = rand() % 100 > 50 ? -1 : 1;
	float randZ = rand() % 100 > 50 ? -1 : 1;
	vec3 n = vec3((rand() % 100) * randX, 0, (rand() % 100) * randZ);
	n = n.Normalize();
	


	// Step2. scale it with a noise factor
	//TODO: add your code here
	vec3 rNoise = actor->KNoise * n;


	// Step3. change the current Vwander  to point to a random direction
	//TODO: add your code here
	m_Wander = actor->KWander * (m_Wander + rNoise).Normalize();


	
	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TODO: add your code here
	Vdesired = m_actorVel + m_Wander;



	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target
	 
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	//TODO: add your code here
	if (actor == leader) {
		Vdesired = (targetPos - actorPos) * actor->KArrival;
	}
	else {
		vec3 totalVelocity = vec3(0, 0, 0);
		int n = 0;
		for (auto& a : agentList) {
			float distance = (actorPos - a.getBehaviorController()->getPosition()).Length();

			if(distance < actor->gKNeighborhood) {
				totalVelocity += a.getBehaviorController()->getVelocity();
				n++;
			}
		}

		Vdesired = totalVelocity / n;
	}



	// Step 2. if not first agent compute Valign as usual
	//TODO: add your code here
	
	
	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TODO: add your code here to compute Vdesired 
	vec3 totalVelocity = vec3(0, 0, 0);
	for (auto& a : agentList) {
		vec3 distance = (actorPos - a.getBehaviorController()->getPosition());
		float distanceLength = distance.Length();

		if (distanceLength < actor->gKNeighborhood && distanceLength > 0) {
			totalVelocity += (distance / (distanceLength * distanceLength));
		}
		
	}

	Vdesired = totalVelocity * actor->KSeparation;




	if (Vdesired.Length() < 5.0)
		Vdesired = 0.0;
	
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TODO: add your code here 
	vec3 numerator = vec3(0, 0, 0);
	float denominator = 0;
	for (auto& a : agentList) {
		vec3 distance = (actorPos - a.getBehaviorController()->getPosition());
		float distanceLength = distance.Length();

		if (distanceLength < actor->gKNeighborhood && distanceLength > 0) {
			numerator += a.getBehaviorController()->getPosition();
			denominator += 1;
		}
	}

	vec3 centerOfMass = numerator / denominator;

	Vdesired = actor->KCohesion * (centerOfMass - actorPos);





	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// compute Vdesired = Vflocking
	// TODO: add your code here 

	Separation separation = Separation(m_pTarget, m_AgentList);
	Alignment alignment = Alignment(m_pTarget, m_AgentList);
	Cohesion cohesion = Cohesion(m_AgentList);

	Vdesired = (0.1 * separation.calcDesiredVel(actor)) + 
		(0.7 * alignment.calcDesiredVel(actor)) + 
		(0.2 * cohesion.calcDesiredVel(actor));

	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// TODO: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 4.0;  float CArrival = 2.0;

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	mat3 Rmat = leader->getGuide().getLocalRotation();  // is rotattion matrix of lead agent


	Arrival arrival = Arrival(m_pTarget);
	if (actor == leader) {
		Vdesired = arrival.calcDesiredVel(actor);
	}
	else {
		AJoint joint = AJoint();
		joint.setLocalTranslation(leader->getPosition() + (-100 * leader->getVelocity().Normalize()));

		arrival = Arrival(&joint);
		Separation separation = Separation(&joint, m_AgentList);

		Vdesired = (CSeparation * separation.calcDesiredVel(actor))
			+ (CArrival * arrival.calcDesiredVel(actor));
	}

	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////

