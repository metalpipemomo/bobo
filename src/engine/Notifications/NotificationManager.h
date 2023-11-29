#pragma once

#include "../../bpch.h"

#include "../UI/ImGuiHelpers.h"
#include "../Coroutine/Waits/WaitForSeconds.h"

enum NotificationTextColor { RED, BLUE, GREEN, BLACK, WHITE };

// Fonts look really blurry when scaled so just ignoring for the moment
enum NotificationFontScale { SMALL, NORMAL, LARGE };

struct NotificationInfo
{
    std::string m_Text;
    ImVec4 m_TextColor;
    float m_FontScale;
    float m_Duration;
    float m_StartDuration;

    NotificationInfo(const std::string& text, const ImVec4 textColor, const float fontScale, const float duration) :
        m_Text(text),
        m_TextColor(textColor),
        m_FontScale(fontScale),
        m_Duration(duration),
        m_StartDuration(duration) {};
};

class NotificationManager
{
public:
    bool m_ReverseOrder = true;

    static void SendNotification(const std::string& text, const NotificationTextColor textColor, const float duration = 5)
    {
        auto n = GetInstance();
        SendNotification(text, n->m_NotificationColorLookup[textColor], duration);
    }

    static void SendNotification(const std::string& text, const ImVec4 textColor, const float duration = 5)
    {
        auto n = GetInstance();
        NotificationInfo* info = new NotificationInfo(text, textColor, 1, duration);
        n->m_ActiveNotifications.push_back(info);
    }

    static void Update()
    {
        auto n = GetInstance();
        n->UpdateMap();
        n->Render();
    }
private:
    static NotificationManager* GetInstance()
    {
        static NotificationManager* instance = new NotificationManager();
        return instance;
    }

    NotificationManager() 
    {
        m_NotificationColorLookup.insert(
            std::pair<NotificationTextColor, ImVec4>(NotificationTextColor::BLUE, ImVec4(0, 0, 1, 1)));
        m_NotificationColorLookup.insert(
            std::pair<NotificationTextColor, ImVec4>(NotificationTextColor::RED, ImVec4(1, 0, 0, 1)));
        m_NotificationColorLookup.insert(
            std::pair<NotificationTextColor, ImVec4>(NotificationTextColor::GREEN, ImVec4(0, 1, 0, 1)));
        m_NotificationColorLookup.insert(
            std::pair<NotificationTextColor, ImVec4>(NotificationTextColor::BLACK, ImVec4(0, 0, 0, 1)));
        m_NotificationColorLookup.insert(
            std::pair<NotificationTextColor, ImVec4>(NotificationTextColor::WHITE, ImVec4(1, 1, 1, 1)));

        m_NotificationFontScaleLookup.insert(std::pair<NotificationFontScale, float>(NotificationFontScale::SMALL, .75));
        m_NotificationFontScaleLookup.insert(std::pair<NotificationFontScale, float>(NotificationFontScale::NORMAL, 1));
        m_NotificationFontScaleLookup.insert(std::pair<NotificationFontScale, float>(NotificationFontScale::LARGE, 1.25));
    };

    void UpdateMap()
    {
        for (int i = 0; i < m_ActiveNotifications.size();)
        {
            auto item = m_ActiveNotifications[i];
            item->m_Duration -= Time::DeltaTime() * m_NotificationFadeRate;
            if (item->m_Duration <= 0)
                m_ActiveNotifications.erase(m_ActiveNotifications.begin() + i);
            else
                i++;
        }
    }

    std::vector<NotificationInfo*> m_ActiveNotifications;
    std::unordered_map<NotificationTextColor, ImVec4> m_NotificationColorLookup;
    std::unordered_map<NotificationFontScale, float> m_NotificationFontScaleLookup;
    const int m_NotifWindowWidth = 500;
    const int m_NotifWindowHeight = WINDOW_HEIGHT;
    const int m_NotifHeight = 30;
    const int m_NotifWindowXOffset = 10;
    const int m_NotifWindowYOffset = 10;
    const float m_NotificationFadeRate = .5;

    void Render()
    {
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_NotifWindowXOffset, 
            main_viewport->WorkPos.y + WINDOW_HEIGHT / 2 - m_NotifWindowHeight / 2 - m_NotifWindowYOffset), 0);
        ImGui::SetNextWindowSize(ImVec2(m_NotifWindowWidth, m_NotifWindowHeight), 0);

        ImGui::Begin("Notifications", NULL,
            ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, true, false, false));

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_NotifWindowHeight - m_NotifHeight);

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4 prevColors = style.Colors[ImGuiCol_Text];

        if (!m_ReverseOrder)
        {
            for (int i = 0; i < m_ActiveNotifications.size(); i++)
            {
                RenderNotification(m_ActiveNotifications[i], style);
            }
        }
        else if (m_ActiveNotifications.size() > 0)
        {
            for (int i = m_ActiveNotifications.size() - 1; i >= 0; i--)
            {
                RenderNotification(m_ActiveNotifications[i], style);
            }
        }

        // reset
        style.Colors[ImGuiCol_Text] = prevColors;
        ImGui::SetWindowFontScale(1);

        ImGui::End();
    }

    void RenderNotification(NotificationInfo* info, ImGuiStyle& style)
    {
        // Set font scale
        float oldFontSize = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= info->m_FontScale;
        ImGui::PushFont(ImGui::GetFont());

        // Set Text Color
        ImVec4 color = info->m_TextColor;
        color.w = info->m_Duration / info->m_StartDuration;
        style.Colors[ImGuiCol_Text] = color;

        // Show Text
        ImGui::Text(info->m_Text.c_str());

        ImGuiHelpers::HeightenCursor(m_NotifHeight * 2);

        ImGui::GetFont()->Scale = oldFontSize;
        ImGui::PopFont();
    }
};