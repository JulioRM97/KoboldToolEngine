#include "kte/game/components/transform_component.h"
#include "kte/game/components/camera_control_movement_component.h"
#include "kte/game/components/vr_pose_tracker_component.h"
#include "kte/game/components/light_component.h"
#include "kte/game/components/orbit_movement_component.h"

#include <string>
#include <functional>

#include <imgui.h>
#include <kte/game/component.h>

namespace kte
{

static void ShowHelpMarker(const char* desc)
{
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered())
  {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(450.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}


bool TransformLayout(const char* field_name = "None",
                     int colums = 1,
                     float* values = nullptr,
                     int count = 0,
                     ImGuiInputTextFlags flags = 0,
                     const char* id = nullptr,
                     bool border = true
);
bool TransformLayout(const char* field_name ,
                     int colums ,
                     float* values,
                     int count,
                     ImGuiInputTextFlags flags,
                     const char* id,
                     bool border)
{
  bool modified = false;
  /*
  ImGui::Columns(colums,id,border);

  ImGui::Separator();
  ImGui::Text(field_name);
  ImGui::NextColumn();

  for(int i = 0; i< count ;++i)
  {
    modified |= ImGui::InputFloat((const char*)i, values,
                                  1.0f,
                                  5.0f,
                                  2,
                                  flags);
    ImGui::NextColumn();
  }
*/
  return modified;
}
void TransformComponent::editorSettings()
{

  Component::editorSettings();

  //~=============================================================

  //~=============================================================
  bool modified = false;

  {
    {
      ImGui::Columns(3);
      ImGui::Separator();
      ImGui::NextColumn();

      ImGui::Text("Local");
      ImGui::NextColumn();

      ImGui::Text("Global");
      ImGui::NextColumn();
    }


    //Location
    {
      ImGui::Separator();

      ImGui::Text("Location");
      ImGui::NextColumn();

      float* aux_local_translation =
          (float*) glm::value_ptr(TransformComponent::local_translation());
      float* aux_world_translation =
          (float*) glm::value_ptr(TransformComponent::world_translation());

      //
      ///~======================== Local translate =========================~///
      {
        ImGui::Text("x:");
        ImGui::SameLine(0.0f, 10.0f);
        modified |= ImGui::InputFloat("##local_translate_x", &aux_local_translation[0],
                                      1.0f,
                                      5.0f,
                                      2);
      }

      {
        ImGui::Text("y:");
        ImGui::SameLine(0.0f, 10.0f);
        modified |= ImGui::InputFloat("##local_translate_y", &aux_local_translation[1],
                                      1.0f,
                                      5.0f,
                                      2);
      }

      {
        ImGui::Text("z:");
        ImGui::SameLine(0.0f, 10.0f);
        modified |= ImGui::InputFloat("##local_translate_z", &aux_local_translation[2],
                                      1.0f,
                                      5.0f,
                                      2);
      }

      ImGui::NextColumn();
      ///~======================== World translate =========================~///

      {
        {
          ImGui::Text("x:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##world_translate_x", &aux_world_translation[0],
                                        0.0f,
                                        0.0f,
                                        2, ImGuiInputTextFlags_ReadOnly);
        }

        {
          ImGui::Text("y:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##world_translate_y", &aux_world_translation[1],
                                        0.0f,
                                        0.0f,
                                        2, ImGuiInputTextFlags_ReadOnly);
        }

        {
          ImGui::Text("z:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##world_translate_z", &aux_world_translation[2],
                                        0.0f,
                                        0.0f,
                                        2, ImGuiInputTextFlags_ReadOnly);
        }
        ImGui::NextColumn();
      }
      float* aux_local_scale =
          (float*) glm::value_ptr(TransformComponent::local_scale());
      float* aux_world_scale =
          (float*) glm::value_ptr(TransformComponent::world_scale());
      //Scale
      {
        {
          ImGui::Separator();

          ImGui::Text("Scale");
          ImGui::NextColumn();
        }

        ///~======================== Local scale =========================~///
        {
          ImGui::Text("x:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##local_scale_x", &aux_local_scale[0],
                                        1.0f,
                                        5.0f,
                                        2);
        }

        {
          ImGui::Text("y:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##local_scale_y", &aux_local_scale[1],
                                        1.0f,
                                        5.0f,
                                        2);
        }

        {
          ImGui::Text("z:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##local_scale_z", &aux_local_scale[2],
                                        1.0f,
                                        5.0f,
                                        2);
        }

        ImGui::NextColumn();
        ///~======================== World scale =========================~///

        {
          {
            ImGui::Text("x:");
            ImGui::SameLine(0.0f, 10.0f);
            modified |=
                ImGui::InputFloat("##world_scale_x", &aux_world_scale[0],
                                  0.0f,
                                  0.0f,
                                  2, ImGuiInputTextFlags_ReadOnly);
          }

          {
            ImGui::Text("y:");
            ImGui::SameLine(0.0f, 10.0f);
            modified |=
                ImGui::InputFloat("##world_scale_y", &aux_world_scale[1],
                                  0.0f,
                                  0.0f,
                                  2, ImGuiInputTextFlags_ReadOnly);
          }

          {
            ImGui::Text("z:");
            ImGui::SameLine(0.0f, 10.0f);
            modified |=
                ImGui::InputFloat("##world_scale_z", &aux_world_scale[2],
                                  0.0f,
                                  0.0f,
                                  2, ImGuiInputTextFlags_ReadOnly);
          }
        }
        ImGui::NextColumn();
      }


      float* aux_local_rotation =
          (float*) glm::value_ptr(TransformComponent::local_rotation());
      float* aux_world_rotation =
          (float*) glm::value_ptr(TransformComponent::world_rotation());
      //Rotation
      {
        {
          ImGui::Separator();

          ImGui::Text("Rotation");
          ImGui::NextColumn();
        }

        ///~======================== Local Rotation ========================~///
        {
          ImGui::Text("x:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##local_rotation_x", &aux_local_rotation[0],
                                        1.0f,
                                        5.0f,
                                        2);
        }

        {
          ImGui::Text("y:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##local_rotation_y", &aux_local_rotation[1],
                                        1.0f,
                                        5.0f,
                                        2);
        }

        {
          ImGui::Text("z:");
          ImGui::SameLine(0.0f, 10.0f);
          modified |= ImGui::InputFloat("##local_rotation_z", &aux_local_rotation[2],
                                        1.0f,
                                        5.0f,
                                        2);
        }

        ImGui::NextColumn();
        ///~======================== World scale =========================~///

        {
          {
            ImGui::Text("x:");
            ImGui::SameLine(0.0f, 10.0f);
            modified |=
                ImGui::InputFloat("##world_rotation_x", &aux_world_rotation[0],
                                  0.0f,
                                  0.0f,
                                  2, ImGuiInputTextFlags_ReadOnly);
          }

          {
            ImGui::Text("y:");
            ImGui::SameLine(0.0f, 10.0f);
            modified |=
                ImGui::InputFloat("##world_rotation_y", &aux_world_rotation[1],
                                  0.0f,
                                  0.0f,
                                  2, ImGuiInputTextFlags_ReadOnly);
          }

          {
            ImGui::Text("z:");
            ImGui::SameLine(0.0f, 10.0f);
            modified |=
                ImGui::InputFloat("##world_rotation_z", &aux_world_rotation[2],
                                  0.0f,
                                  0.0f,
                                  2, ImGuiInputTextFlags_ReadOnly);
          }
        }
        ImGui::NextColumn();
      }

      if (modified)
      {
        set_local_translation(glm::make_vec3((const float*) (aux_local_translation)));
        set_local_scale(glm::make_vec3((const float*)(aux_local_scale)));
        set_local_rotation(glm::make_quat((const float*)(aux_world_rotation)));

      }
    }
    ImGui::Columns(1);
    ImGui::Separator();


  }

}

void CameraControlMovementComponent::editorSettings()
{
  Component::editorSettings();


  ImGui::Separator();
  ImGui::Text("Camera Speed Movement:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderFloat("##camera_speed",
                     &cam_speed_,
                     0.0f,
                     1000.0f);
  ImGui::Text("Camera Speed Rotation:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderFloat("##camera_speed_rotation",
                     &rotate_speed_,
                     0.0f,
                     50.0f);
  ImGui::Separator();

}

void VrPoseTrackerComponent::editorSettings()
{
  Component::editorSettings();

  ImGui::Separator();
  ImGui::Text("Camera Is VR: true"); ImGui::SameLine(0.f, 10.f);
  ImGui::Separator();

}

void LightComponent::editorSettings()
{
  if(ImGui::Button("Directional Light"))
  {
    set_type(LightType::k_Directional);
  }
  ImGui::SameLine(0.f, 10.f);

  if(ImGui::Button("Spot Light"))
  {

  }
  ImGui::SameLine(0.f, 10.f);

  if(ImGui::Button("Point Light"))
  {

  }
  ImGui::SameLine(0.f, 10.f);

  ImGui::Separator();

  /*
  ImGui::SliderFloat3("Light Direction",
                      &data_->direction.x,
                      -1.0f, 1.0f, "%.3f", 0.05f);
  ImGui::Text("Intensity:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderFloat("##light_intensity",
                     &data_->intensity,
                     0.0f,
                     1000.0f);

  ImGui::Text("Constant Attenuation:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderFloat("##light_constant_attenuation",
                     &data_->constant_attenuation,
                     1.0f,
                     2.0f);

  ImGui::Text("Linear Attenuation:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderFloat("##light_linear_attenuation",
                     &data_->linear_attenuation,
                     0.0f,
                     1.0f);

  ImGui::Text("Quadratic Attenuation:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderFloat("##light_quadratic_attenuation",
                     &data_->quadratic_attenuation,
                     0.0f,
                     1.0f);

  ImGui::Text("Falloff Exponent:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderInt("##light_falloff_exponent",
                     &data_->falloff_exponent,
                     0,
                     10);

  ImGui::Text("Specular Strenght:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderFloat("##light_specular_strenght",
                     &data_->specular_strength,
                     1.0f,
                     10.0f);

  ImGui::Text("Specular Density:"); ImGui::SameLine(0.f, 10.f);
  ImGui::SliderInt("##light_specular_density",
                   &data_->specular_density,
                   0,
                   1024);

  ImGui::ColorPicker3("Light Color", &data_->color.x);
  */

  ImGui::Separator();
}

void OrbitMovementComponent::editorSettings()
{
}

}

