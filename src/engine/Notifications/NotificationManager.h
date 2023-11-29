#pragma once

#include "../../bpch.h"

#include "../UI/ImGuiHelpers.h"

enum NotificationTextColor { RED, BLUE, GREEN, BLACK, WHITE };

enum NotificationFontScale{ SMALL, NORMAL, LARGE };

struct NotificationInfo
{
    std::string m_Text;
    ImVec4 m_TextColor;
    float m_FontScale;
    float m_Duration;
    float m_StartDuration;
    float m_FadeAfter;

    NotificationInfo(const std::string& text, const ImVec4 textColor, const float fontScale, const float duration, const float fadeAfter) : 
        m_Text(text), m_TextColor(textColor), m_FontScale(fontScale), m_Duration(duration), m_StartDuration(duration) {};
};

class NotificationManager
{
public:
    static void SendNotification(const std::string& text, const NotificationTextColor textColor, 
        const NotificationFontScale fontScale = NotificationFontScale::NORMAL, const float duration = 5)
    {
        auto n = GetInstance();
        SendNotification(text, n->m_NotificationColorLookup[textColor], fontScale, duration);
    }

    static void SendNotification(const std::string& text, const ImVec4 textColor, 
        const NotificationFontScale fontScale = NotificationFontScale::NORMAL, const float duration = 5)
    {
        auto n = GetInstance();
        NotificationInfo* info = new NotificationInfo(text, textColor, fontScale, duration, n->m_NotifFadeAfter);
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

        m_NotificationFontScaleLookup.insert(std::pair<NotificationFontScale, float>(NotificationFontScale::SMALL, .95));
        m_NotificationFontScaleLookup.insert(std::pair<NotificationFontScale, float>(NotificationFontScale::NORMAL, 1));
        m_NotificationFontScaleLookup.insert(std::pair<NotificationFontScale, float>(NotificationFontScale::LARGE, 1.05));
    };

    void UpdateMap()
    {
        for (int i = 0; i < m_ActiveNotifications.size();)
        {
            auto item = m_ActiveNotifications[i];

            if (item->m_FadeAfter > 0)
            {
                item->m_FadeAfter -= Time::DeltaTime();
                continue;
            }

            item->m_Duration -= Time::DeltaTime();

            if (item->m_Duration <= 0)
            {
                // remove from data structures
                m_ActiveNotifications.erase(m_ActiveNotifications.begin() + i);
            }
            else
            {
                i++;
            }
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
    const float m_NotifFadeAfter = 5;

    void Render()
    {
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_NotifWindowXOffset, 
            main_viewport->WorkPos.y + WINDOW_HEIGHT - m_NotifWindowHeight / 2 - m_NotifWindowYOffset), 0);
        ImGui::SetNextWindowSize(ImVec2(m_NotifWindowWidth, m_NotifWindowHeight), 0);

        ImGui::Begin("Notifications", NULL,
            ImGuiHelpers::MakeFlags(true, false, true, true, true, false, true, true, false, false));

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_NotifWindowHeight / 2 - m_NotifHeight);

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4 prevColors = style.Colors[ImGuiCol_Text];

        for (int i = 0; i < m_ActiveNotifications.size(); i++)
        {
            // Set Text Color
            auto item = m_ActiveNotifications[i];
            ImVec4 color = item->m_TextColor;
            color.w = item->m_Duration / item->m_StartDuration;
            style.Colors[ImGuiCol_Text] = color;
           
            // Set Font Scale
            ImGui::SetWindowFontScale(item->m_FontScale);

            // Show Text
            ImGui::Text(item->m_Text.c_str());

            ImGuiHelpers::HeightenCursor(m_NotifHeight * 2);
        }

        // reset
        style.Colors[ImGuiCol_Text] = prevColors;
        ImGui::SetWindowFontScale(1);

        ImGui::End();
    }
};