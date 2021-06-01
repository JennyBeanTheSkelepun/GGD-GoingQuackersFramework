#ifndef _PICKUP_H_
#define _PICKUP_H_ 
#pragma once

#include "Component.h"
#include "../../Data Structures/Vectors.h"
#include "../GameObject.h"
#include "./Physics/Rigidbody.h"

//typedef nlohmann::json json;


class Pickup : public Component
{
public:

    Pickup(GameObject* owner);
    ~Pickup();

    //- Basic Loops -//
    void OnDestroy() override;
    void Update() override;

    //- ImGui UpdateLoop -//
    void ImGUIUpdate() override;

    //- Scene Save and Load -//
    json* SceneSave() override;
    void SceneLoad(json* componentJSON) override;

private:

    GameObject* parentObj;
    Rigidbody* rb;
};
#endif