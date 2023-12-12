#pragma once

#include "../../bpch.h"

#include "../UI/ImGuiHelpers.h"

enum AnchorPos 
{
    TOP_LEFT,
    TOP_CENTER,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER_CENTER,
    CENTER_RIGHT,
    LOW_LEFT,
    LOW_CENTER,
    LOW_RIGHT
};

struct PerPopupInformation
{
    const std::vector<const char*> m_Text;
    const ImVec2 m_WindowSize;

    PerPopupInformation(const char* text, const ImVec2 windowSize) : m_Text({ text }), m_WindowSize(windowSize) {};
    PerPopupInformation(const std::vector<const char*> text, const ImVec2 windowSize) : m_Text(text), m_WindowSize(windowSize) {};
};

struct PopupInformation
{
    const char* m_Title;
    std::vector<PerPopupInformation> m_Pages;
    unsigned int m_PageIndex;
    const ImVec2 m_OffsetFromAnchor;
    const bool m_AllowMove;
    const bool m_AllowEarlyClose;
    ImVec2 m_AnchorPosition;

    PopupInformation(const char* title, std::vector<PerPopupInformation> pages,
        const AnchorPos anchorPos, const ImVec2 offsetFromAnchor, const bool allowMove, const bool allowEarlyClose)
        : m_Title(title), m_Pages(pages), m_OffsetFromAnchor(offsetFromAnchor), m_AllowMove(allowMove), m_AllowEarlyClose(allowEarlyClose)
    {
        m_PageIndex = 0;
        SetAnchorPos(anchorPos);
    };

    ImVec2 GetWindowSize()
    {
        return m_Pages[m_PageIndex].m_WindowSize;
    }

    const std::vector<const char*> GetText()
    {
        return m_Pages[m_PageIndex].m_Text;
    }

private:
    void SetAnchorPos(AnchorPos anchor)
    {
        switch (anchor)
        {
            case AnchorPos::TOP_LEFT:
                m_AnchorPosition = ImVec2(0, 0);
                break;
            case AnchorPos::TOP_CENTER:
                m_AnchorPosition = ImVec2(WINDOW_WIDTH / 2 - GetWindowSize().x / 2, 0);
                break;
            case AnchorPos::TOP_RIGHT:
                m_AnchorPosition = ImVec2(WINDOW_WIDTH - GetWindowSize().x, 0);
                break;
            case AnchorPos::CENTER_LEFT:
                m_AnchorPosition = ImVec2(0, WINDOW_HEIGHT / 2 - GetWindowSize().y / 2);
                break;
            case AnchorPos::CENTER_CENTER:
                m_AnchorPosition = ImVec2(WINDOW_WIDTH / 2 - GetWindowSize().x / 2, WINDOW_HEIGHT / 2 - GetWindowSize().y / 2);
                break;
            case AnchorPos::CENTER_RIGHT:
                m_AnchorPosition = ImVec2(WINDOW_WIDTH - GetWindowSize().x, WINDOW_HEIGHT / 2 - GetWindowSize().y / 2);
                break;
            case AnchorPos::LOW_LEFT:
                m_AnchorPosition = ImVec2(0, WINDOW_HEIGHT - GetWindowSize().y);
                break;
            case AnchorPos::LOW_CENTER:
                m_AnchorPosition = ImVec2(WINDOW_WIDTH / 2 - GetWindowSize().x / 2, WINDOW_HEIGHT - GetWindowSize().y);
                break;
            case AnchorPos::LOW_RIGHT:
                m_AnchorPosition = ImVec2(WINDOW_WIDTH - GetWindowSize().x, WINDOW_HEIGHT - GetWindowSize().y);
                break;
            default:
                break;
        }
    }
};

class PopupManager
{
public:
    static void MakePopup(const char* title, const char* text, const ImVec2 windowSize,
        const AnchorPos anchorPos, const ImVec2 offsetFromAnchor, bool allowMove = true)
    {
        MakePopup(title, 
            { PerPopupInformation({ text }, windowSize) }, anchorPos, offsetFromAnchor, allowMove);
    }

    static void MakePopup(const char* title, const std::vector<const char*> text, const ImVec2 windowSize, 
        const AnchorPos anchorPos, const ImVec2 offsetFromAnchor, bool allowMove = true)
    {
        MakePopup(title, { PerPopupInformation(text, windowSize) },anchorPos, offsetFromAnchor, allowMove);
    }

    static void MakePopup(const char* title, std::vector<PerPopupInformation> pages,
        const AnchorPos anchorPos, const ImVec2 offsetFromAnchor, bool allowMove = true, bool allowEarlyClose = false)
    {
        auto pm = GetInstance();
        pm->m_ActivePopups.push_back(new PopupInformation(title, pages, anchorPos, offsetFromAnchor, allowMove, allowEarlyClose));
    }

    static void Update()
    {
        auto pm = GetInstance();
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        bool open = true;
        for (unsigned int i = 0; i < pm->m_ActivePopups.size(); )
        {
            auto& item = pm->m_ActivePopups[i];
            ImGui::SetNextWindowPos(ImVec2(
                main_viewport->WorkPos.x + item->m_AnchorPosition.x + item->m_OffsetFromAnchor.x, 
                main_viewport->WorkPos.y + item->m_AnchorPosition.y + item->m_OffsetFromAnchor.y), 
                item->m_AllowMove ? ImGuiCond_Once : ImGuiCond_Always);
            ImGui::SetNextWindowSize(item->GetWindowSize(), 0);
           
            bool hasNextButton = item->m_PageIndex + 1 < item->m_Pages.size();
            bool hasPrevButton = item->m_PageIndex > 0;
            bool canClose = (item->m_Pages.size() == 1 || item->m_PageIndex == item->m_Pages.size() - 1) || item->m_AllowEarlyClose;

            ImGui::Begin(item->m_Title, canClose ? &open : NULL, 
                ImGuiHelpers::MakeFlags(false, false, true, false, true, false, true, false, false, true));

            ImGuiHelpers::LowerCursor();

            for (auto& it : item->GetText())
            {
                ImGuiHelpers::MakeCenterText(it);
            }

            // Controls
            // Prev & Next
            if (hasPrevButton || hasNextButton)
                ImGuiHelpers::LowerCursor();
            
            auto windowWidth = ImGui::GetWindowSize().x;
            if (hasPrevButton && !hasNextButton)
            {
                ImGui::SetCursorPosX((windowWidth - 75) * 0.5f);
                if (ImGuiHelpers::MakeButton("Previous", ImVec2(75, 20)))
                    item->m_PageIndex--;
            }
            else if (!hasPrevButton && hasNextButton)
            {
                // To Center
                ImGui::SetCursorPosX((windowWidth - 75) * 0.5f);

                if (ImGuiHelpers::MakeButton("Next", ImVec2(75, 20)))
                    item->m_PageIndex++;
            }
            else if (hasPrevButton && hasNextButton)
            {
                // To Center
                ImGui::SetCursorPosX((windowWidth - 150) * 0.5f);

                if (ImGuiHelpers::MakeButton("Previous", ImVec2(75, 20)))
                    item->m_PageIndex--;
                ImGui::SameLine();
                if (ImGuiHelpers::MakeButton("Next", ImVec2(75, 20)))
                    item->m_PageIndex++;
            }
            
            if (open)
                i++;
            else
            {
                pm->m_ActivePopups.erase(pm->m_ActivePopups.begin() + i);
                ImGuiHelpers::PlayUIClick();
            }
            open = true;

            ImGui::End();
        }
    }
private:
    static PopupManager* GetInstance()
    {
        static PopupManager* instance = new PopupManager();
        return instance;
    }

    PopupManager() {};

    std::vector<PopupInformation*> m_ActivePopups;
};