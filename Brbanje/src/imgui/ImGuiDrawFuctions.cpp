#include "brpch.h"
#include "ImGuiDrawFuctions.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace Brbanje
{
	void ImGuiDrawFunctions::DrawVec2Control(const std::string& label, glm::vec2& values,float speed, float resetValue, float columnWidth, const char* format)
	{
		ImGuiIO& io = ImGui::GetIO();


		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);


		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		float itemWidth = ImGui::CalcItemWidth();
		ImGui::PushMultiItemsWidths(2, itemWidth);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		//Color Testing
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;

		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, format);
		ImGui::PopItemWidth();
		ImGui::SameLine();



		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushFont(boldFont);

		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, format);
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopStyleVar();

		ImGui::PopID();
	}
}