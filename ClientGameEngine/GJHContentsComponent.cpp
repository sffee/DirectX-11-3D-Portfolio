#include "GJHContentsComponent.h"

GJHContentsComponent::GJHContentsComponent()
{

}

GJHContentsComponent::~GJHContentsComponent()
{

}

void GJHContentsComponent::Begin()
{
	ActorTrans = FindComponent<GJHActorTransform>();
}