#include "Pickup.h"
#include <DirectXMath.h>
#include "../GameObject.h"
#include <math.h>

//- Constructors -//
Pickup::Pickup(GameObject* owner) : Component(owner, ComponentTypes::PICKUP, "Pickup")
{
    parentObj = this->GetOwner();

    if (parentObj->GetComponent<Rigidbody>() == NULL)
    {
        rb = parentObj->AddComponent<Rigidbody>();
    }
    else
    {
        rb = parentObj->GetComponent<Rigidbody>();
    }
}

//- Deconstructors -//
Pickup::~Pickup()
{

}

void Pickup::OnDestroy()
{
    this->~Pickup();
}

//- Update Function -//
void Pickup::Update()
{
    if (rb->GetCollidedObjects().size() > 0)
    {
        Debug::getInstance()->Log("picking up scrap");
    }
}

//- ImGui -//
void Pickup::ImGUIUpdate()
{

}

//- Scene Save / Load -//
void Pickup::SceneLoad(json* componentJSON)
{
    /*ElementVar1 = (*componentJSON)["ElementName1"];
        ElementVar2 = (*componentJSON)["ElementName2"];*/
}

json* Pickup::SceneSave()
{
    //json* returnObj = new json({ ​​​​​​​​​​​​​​​​​​​​​​​
        ////        /*{​​​​​​​​​​​​​​​​​​​​​​​"ElementName1", ElementVar1}​​​​​​​​​​​​​​​​​​​​​​​,
        ////        {​​​​​​​​​​​​​​​​​​​​​​​"ElementName2", ElementVar2}​​​​​​​​​​​​​​​​​​​​​​​*/
        //        }​​​​​​​​​​​​​​​​​​​​​​​);

        return new json();
}