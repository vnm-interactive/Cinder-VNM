
/*
 Copyright (c) 2013 Nick Porcino, All rights reserved.
 License is MIT: http://opensource.org/licenses/MIT

 LabCameraImgui has no external dependencies besides LabCamera and Dear ImGui.
 Include LabCamera.cpp, and LabCameraImgui.cpp in your project.
*/

#ifndef LABCAMERAIMGUI_H
#define LABCAMERAIMGUI_H

#include "LabCamera.h"

enum LabCameraNavigatorPanelInteraction
{
    LCNav_None = 0,
    LCNav_ModeChange, LCNav_RollUpdated,
    LCNav_TumbleInitiated, LCNav_TumbleContinued, LCNav_TumbleEnded
};

enum LCNav_PanelMode
{
    LCNav_Mode_PanTilt, LCNav_Mode_Arcball
};

class LCNav_PanelState
{
protected:
    LCNav_PanelState() = default;
    virtual ~LCNav_PanelState() = default;

public:
    float nav_radius = 6;
    lab::camera::PanTiltController pan_tilt;
    LabCameraNavigatorPanelInteraction state = LCNav_None;
    lab::camera::InteractionMode camera_interaction_mode = lab::camera::InteractionMode::TurnTableOrbit;
};

/*
 * The navigator panel takes a viewport, and a camera to mutate.
 * The viewport should be the viewport size in device coordinates.
 */

LabCameraNavigatorPanelInteraction
run_navigator_panel(LCNav_PanelState* navigator_panel, lab::camera::Camera& camera, float dt);

LCNav_PanelState* create_navigator_panel();
void release_navigator_panel(LCNav_PanelState*);


void camera_minimap(int w, int h, const lab::camera::rigid_transform* cam, const lab::camera::v3f lookat);



#endif
