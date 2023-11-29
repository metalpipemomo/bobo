#pragma once

#include "../../bpch.h"

class ImGuiHelpers
{
public:
    static ImGuiWindowFlags MakeFlags(bool noTitleBar, bool noScrollbar, bool noMenu, bool noMove, bool noResize, bool noCollapse,
        bool noNav, bool noBackground, bool noBringToFront, bool unsavedDocument)
    {
        ImGuiWindowFlags window_flags = 0;
        if (noTitleBar)         window_flags |= ImGuiWindowFlags_NoTitleBar;
        if (noScrollbar)        window_flags |= ImGuiWindowFlags_NoScrollbar;
        if (!noMenu)            window_flags |= ImGuiWindowFlags_MenuBar;
        if (noMove)             window_flags |= ImGuiWindowFlags_NoMove;
        if (noResize)           window_flags |= ImGuiWindowFlags_NoResize;
        if (noCollapse)         window_flags |= ImGuiWindowFlags_NoCollapse;
        if (noNav)              window_flags |= ImGuiWindowFlags_NoNav;
        if (noBackground)       window_flags |= ImGuiWindowFlags_NoBackground;
        if (noBringToFront)     window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        if (unsavedDocument)    window_flags |= ImGuiWindowFlags_UnsavedDocument;
        return window_flags;
    }

    // Helper UI function
    static void MakeCenterText(std::string text, bool centerHorizontally = true, bool centerVertically = false)
    {
        // Center Horizontally
        if (centerHorizontally)
        {
            auto windowWidth = ImGui::GetWindowSize().x;
            auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
            ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        }

        // Center Vertically
        if (centerVertically)
        {
            auto windowHeight = ImGui::GetWindowSize().y;
            auto textHeight = ImGui::CalcTextSize(text.c_str()).y;
            ImGui::SetCursorPosY((windowHeight - textHeight) * 0.5f);
        }

        ImGui::Text(text.c_str());
    }

    // Helper UI function
    static bool MakeCenterButton(std::string text)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        float size = ImGui::CalcTextSize(text.c_str()).x + style.FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        float off = (avail - size) * .5;
        if (off > 0.0f)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        return ImGui::Button(text.c_str());
    }

    static void LowerCursor(int lowerBy = 10)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lowerBy);
    }

    static void HeightenCursor(int heightenBy = 10)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - heightenBy);
    }
};