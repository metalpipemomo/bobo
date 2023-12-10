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

    static bool MakeButton(std::string text, ImVec2 size = ImVec2(0, 0))
    {
        bool b = ImGui::Button(text.c_str(), size);
        if (b)
            PlayUIClick();
        return b;
    }

    // Helper UI function
    static bool MakeCenterButton(std::string text, bool centerHorizontally = true, bool centerVertically = false)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        if (centerHorizontally)
        {
            float size = ImGui::CalcTextSize(text.c_str()).x + style.FramePadding.x * 2.0f;
            float avail = ImGui::GetContentRegionAvail().x;
            float off = (avail - size) * .5;
            if (off > 0.0f)
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
        }

        if (centerVertically)
        {
            float size = ImGui::CalcTextSize(text.c_str()).y + style.FramePadding.y * 2.0f;
            float avail = ImGui::GetContentRegionAvail().y;
            float off = (avail - size) * .5;
            if (off > 0.0f)
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off);
        }

        return MakeButton(text);
    }

    static void LowerCursor(int lowerBy = 10)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lowerBy);
    }

    static void HeightenCursor(int heightenBy = 10)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - heightenBy);
    }

    static void MakeVerticalList(std::vector<std::string> texts, float gap, std::string title = "")
    {
        if (!title.empty())
        {
            MakeCenterText(title);
            LowerCursor(gap);
        }

        for (auto& it : texts)
        {
            MakeCenterText(it);
            LowerCursor(gap);
        }
    }

    static void PlayUIClick()
    {
        auto s = Audio::GetSoundInfo("Click");
        s->m_Pitch = RandomFloat(.9, 1.1);
        s->SetVolume(RandomFloat(.8, 1));
        Audio::PlaySound("Click");
    }

    static float RandomFloat(float a, float b)
    {
        return ((b - a) * ((float)rand() / RAND_MAX)) + a;
    }
};