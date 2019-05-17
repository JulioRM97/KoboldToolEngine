#include "kte/editor/editor_manager.h"

#include "kte/game/game.h"
#include "kte/game/game_manager.h"
#include "kte/game/scene.h"
#include "kte/graphics/window.h"

namespace kte
{

////////////////////////////////////////////
kte::EditorManager::EditorManager()
    : DefaultUpdatable()
{

}

////////////////////////////////////////////
kte::EditorManager::~EditorManager()
{

}

///////////////////////////////////////
void kte::EditorManager::draw()
{
  auto& manager = kte::GameManager::Instance();

  //ImGui::PushStyleColor(ImGuiCol_WindowBg,ImVec4(0.0f,0.0f,0.0,0.0f));
  //ImGuiStyle *style = &ImGui::GetStyle();
 // ImGui::ShowStyleEditor(style);
  ImGui::Begin("Debug",&opened_,
               ImGuiWindowFlags_AlwaysVerticalScrollbar |
               ImGuiWindowFlags_HorizontalScrollbar );

  if (ImGui::CollapsingHeader("Tree"))
  {
    auto& scene_nodes =
        manager.current_game_->scenes_[manager.current_game_->current_scene_id_]->nodes_;
    node(scene_nodes);
  }
  ImGui::End();
}

/////////////////////////////////////////////
void EditorManager::node(std::deque<Node*> nodes)
{
  //TODO getNames
  if(ImGui::TreeNode("Nodes"))
  {
    int i = 0;
    for(auto& node_aux : nodes )
    {

      if (ImGui::TreeNode((void*) (intptr_t) i, "node %i", i))
      {
        if(!node_aux->children_.empty())
        {
          node(node_aux->children_);
        }
        if(!node_aux->components_.empty())
        {
          components(node_aux->components());
        }
        ImGui::TreePop();
      }
      i++;
    }
    ImGui::TreePop();
  }

}

/////////////////////////////////////////////
void EditorManager::components(std::deque<Component*> comp)
{
  //TODO getNames
  int i = 0;
  if(ImGui::TreeNode("Components"))
  {
    for (auto* comp_aux : comp)
    {
      if(comp_aux == nullptr)
        continue;

      if (ImGui::TreeNode((void*) (intptr_t) i, comp_aux->name(), i))
      {
        comp_aux->editorSettings();
        ImGui::TreePop();
      }
      i++;
    }
    ImGui::TreePop();
  }
}

////////////////////////////////////////////
void kte::EditorManager::update()
{
  /*auto& manager = GameManager::Instance();
    auto list = manager.display_list_buffer_.createList();
    list->add<kte::graphics::DrawEditorCommand>();
    manager.display_list_buffer_.submit(list);*/
}

} // namespace kte

