#include "stdafx.h"
#include "Entity.h"

#define GAME_ENGINE (GameEngine::GetSingleton())

Entity::Entity()
{
}

Entity::~Entity()
{
	delete m_ActPtr;
	m_ActPtr = nullptr;
}

// ==== PUBLIC METHODS ====
void Entity::Tick(double deltaTime)
{
	UpdateVariables(deltaTime);
	ApplyImpulse(deltaTime);
}

void Entity::Paint()
{
	CreateWorldMatrix();
}

void Entity::RemoveContactListener()
{
	m_ActPtr->RemoveContactListener(this);
}

int GetHealth()
{
	return 0;
}

// ==== PROTECTED METHODS ====
void Entity::Init()
{

}

void Entity::UpdateVariables(double deltaTime)
{
	m_AccuTime += deltaTime;
	m_Vel = m_ActPtr->GetLinearVelocity();
	m_DesiredVel = DOUBLE2(0.0, m_ActPtr->GetLinearVelocity().y);
	m_Mass = m_ActPtr->GetMass();
}

void Entity::CreateWorldMatrix()
{
	DOUBLE2 actHeroPos = m_ActPtr->GetPosition();
	double actHeroAngle = m_ActPtr->GetAngle();

	MATRIX3X2 matPivot, matTransform, matTranslate, matAngle, matScale;
	matPivot.SetAsTranslate(DOUBLE2(-TILE_SIZE / 2, -TILE_SIZE / 2));
	matAngle.SetAsRotate(actHeroAngle);

	switch (m_Direction)
	{
	case Entity::Direction::LEFT:
		matScale.SetAsScale(-1, 1);
		break;
	case Entity::Direction::RIGHT:
		matScale.SetAsScale(1, 1);
		break;
	default:
		break;
	}

	matTranslate.SetAsTranslate(actHeroPos);
	matTransform = matPivot*matScale*matAngle*matTranslate;
	GAME_ENGINE->SetWorldMatrix(matTransform);
}

void Entity::Idle()
{
	m_ActionState = ActionState::IDLE;
	Anim();
	m_DesiredVel.x = m_Vel.x * 0.85;
}

void Entity::MoveLeft()
{
	if (m_OnFloor)m_ActionState = ActionState::WALK;
	Anim();
	m_Direction = Direction::LEFT;
	m_DesiredVel.x = -m_BaseSpeed;
}

void Entity::MoveRight()
{
	if (m_OnFloor)m_ActionState = ActionState::WALK;
	Anim();
	m_Direction = Direction::RIGHT;
	m_DesiredVel.x = m_BaseSpeed;
}

void Entity::ApplyImpulse(double deltaTime)
{
	DOUBLE2 deltaVel = m_DesiredVel - m_Vel;
	DOUBLE2 j = m_Mass * deltaVel / PhysicsActor::SCALE;
	m_ActPtr->ApplyLinearImpulse(j);
}

void Entity::ResetCurrentFrame()
{
	if (m_BoolCurrentFrame)
	{
		m_CurrentFrame = 0;
		m_BoolCurrentFrame = false;
	}
}