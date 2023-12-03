#pragma once

#include "../../bpch.h"

#include <queue>

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

    NotificationInfo(const std::string& text, const ImVec4 textColor, const float fontScale, const float duration) :
        m_Text(text),
        m_TextColor(textColor),
        m_FontScale(fontScale),
        m_Duration(duration) {}
};

struct DefaultNotificationInfo : NotificationInfo
{
    float m_StartDuration;

    DefaultNotificationInfo(const std::string& text, const ImVec4 textColor, const float fontScale, const float duration) :
       NotificationInfo(text, textColor, fontScale, duration),
       m_StartDuration(duration) {};
};

struct BannerNotificationInfo : NotificationInfo
{
    float m_Alpha;
    float m_YOffset;

    BannerNotificationInfo(const std::string& text, const ImVec4 textColor, const float fontScale, const float duration,
        const float startAlpha, const float yOffset) :
        NotificationInfo(text, textColor, fontScale, duration), m_Alpha(startAlpha), m_YOffset(yOffset) {};

    virtual bool In() = 0;
    virtual bool Out() = 0;
};

struct AlphaBannerNotificationInfo : BannerNotificationInfo
{
    float m_FadeRate;
    float m_MaxAlpha;

    AlphaBannerNotificationInfo(const std::string& text, const ImVec4 textColor, const float fontScale, const float duration,
        const float fadeRate, const float alphaTarget) :
        BannerNotificationInfo(text, textColor, fontScale, duration, 0, 0), m_FadeRate(fadeRate), m_MaxAlpha(alphaTarget) {};

    bool In() 
    {
        m_Alpha += Time::DeltaTime() * m_FadeRate;
        if (m_Alpha >= m_MaxAlpha)
        {
            m_Alpha = m_MaxAlpha;
            return true;
        }
        return false;
    }

    bool Out()
    {
        m_Alpha -= Time::DeltaTime() * m_FadeRate;
        if (m_Alpha <= 0)
        {
            m_Alpha = 0;
            return true;
        }
        return false;
    }
};

struct SlideBannerNotificationInfo : BannerNotificationInfo
{
    float m_SlideSpeed;

    SlideBannerNotificationInfo(const std::string& text, const ImVec4 textColor, const float fontScale, const float duration,
        const float slideSpeed) :
        BannerNotificationInfo(text, textColor, fontScale, duration, .75, -WINDOW_HEIGHT / 2), m_SlideSpeed(slideSpeed) {};

    bool In()
    {
        m_YOffset += Time::DeltaTime() * m_SlideSpeed;
        if (m_YOffset >= 0)
        {
            m_YOffset = 0;
            return true;
        }
        return false;
    }

    bool Out()
    {
        m_YOffset += Time::DeltaTime() * m_SlideSpeed;
        if (m_YOffset >= WINDOW_HEIGHT / 2)
        {
            m_YOffset = WINDOW_HEIGHT / 2;
            return true;
        }
        return false;
    }
};

class NotificationManager
{
public:
    bool m_ReverseOrder = true;

    /* Default */
    static void SendDefaultNotification(const std::string& text, 
        const NotificationTextColor textColor, const float duration = 3)
    {
        auto n = GetInstance();
        SendDefaultNotification(text, n->GetNotificationColor(textColor), duration);
    }

    static void SendDefaultNotification(const std::string& text, const ImVec4 textColor, const float duration = 3)
    {
        auto n = GetInstance();
        DefaultNotificationInfo* info = new DefaultNotificationInfo(text, textColor, 1, duration);
        n->m_ActiveNotifications.push_back(info);
    }

    /* Banner */
    static void SendAlphaBannerNotification(const std::string& text, const NotificationTextColor textColor, const float duration = 1, 
        const float speed = 5, const float targetAlpha = .75)
    {
        auto n = GetInstance();
        SendAlphaBannerNotification(text, n->GetNotificationColor(textColor), duration, speed, targetAlpha);
    }

    static void SendAlphaBannerNotification(const std::string& text, const ImVec4 textColor, const float duration = 1,
        const float speed = 5, const float targetAlpha = .75)
    {
        auto n = GetInstance();
        BannerNotificationInfo* info = new AlphaBannerNotificationInfo(text, textColor, 1, duration, speed, targetAlpha);
        n->m_PendingBannerNotifications.push(info);
    }

    static void SendSlidingBannerNotification(const std::string& text, const NotificationTextColor textColor, const float duration = 1,
        const float speed = 750)
    {
        auto n = GetInstance();
        SendSlidingBannerNotification(text, n->GetNotificationColor(textColor), duration, speed);
    }

    static void SendSlidingBannerNotification(const std::string& text, const ImVec4 textColor, const float duration = 1,
        const float speed = 750)
    {
        auto n = GetInstance();
        BannerNotificationInfo* info = new SlideBannerNotificationInfo(text, textColor, 1, duration, speed);
        n->m_PendingBannerNotifications.push(info);
    }

    static void Update()
    {
        auto n = GetInstance();
        n->UpdateMap();
        n->UpdateBanner();
        n->Render();
    }

    static void ClearPendingBannerNotifications()
    {
        auto n = GetInstance();
        n->m_PendingBannerNotifications = {};
    }

private:
    static NotificationManager* GetInstance()
    {
        static NotificationManager* instance = new NotificationManager();
        return instance;
    }

    NotificationManager() {};

    // Banner Notifications
    std::queue<BannerNotificationInfo*> m_PendingBannerNotifications;
    BannerNotificationInfo* m_CurrentBannerNotif = nullptr;
    unsigned int m_BannerNotifStage;
    unsigned int m_BannerWindowHeight = 75;

    // Default Notifications
    std::vector<DefaultNotificationInfo*> m_ActiveNotifications;
    const int m_NotifWindowWidth = 500;
    const int m_NotifWindowHeight = WINDOW_HEIGHT;
    const int m_NotifHeight = 30;
    const int m_NotifWindowXOffset = 10;
    const int m_NotifWindowYOffset = 10;
    const float m_NotificationFadeRate = .5;

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

    void UpdateBanner()
    {
        if (m_CurrentBannerNotif == nullptr)
        {
            if (m_PendingBannerNotifications.size() > 0)
            {
                // Set new notif
                m_CurrentBannerNotif = m_PendingBannerNotifications.front();
                m_PendingBannerNotifications.pop();
                m_BannerNotifStage = 0;
            }
        }
        else 
        {
            switch (m_BannerNotifStage)
            {
                case 0:
                    if (m_CurrentBannerNotif->In()) m_BannerNotifStage++;
                    break;
                case 1:
                    m_CurrentBannerNotif->m_Duration -= Time::DeltaTime();
                    if (m_CurrentBannerNotif->m_Duration <= 0) m_BannerNotifStage++;
                    break;
                case 2:
                    if (m_CurrentBannerNotif->Out()) m_BannerNotifStage++;
                    break;
                case 3:
                    m_CurrentBannerNotif = nullptr;
                    break;
            }
        }
    }

    void Render()
    {
        /* Notifications Window */
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + m_NotifWindowXOffset, 
            main_viewport->WorkPos.y + WINDOW_HEIGHT / 2 - m_NotifWindowHeight / 2 - m_NotifWindowYOffset), 0);
        ImGui::SetNextWindowSize(ImVec2(m_NotifWindowWidth, m_NotifWindowHeight), 0);

        ImGui::Begin("Notifications", NULL,
            ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, true, false, false));

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_NotifWindowHeight - m_NotifHeight);

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4 prevTextColors = style.Colors[ImGuiCol_Text];
        ImVec4 prevWindowBgColor = style.Colors[ImGuiCol_WindowBg];
        ImVec4 prevBorderColor = style.Colors[ImGuiCol_Border];

        if (!m_ReverseOrder)
        {
            for (int i = 0; i < m_ActiveNotifications.size(); i++)
            {
                RenderDefaultNotification(m_ActiveNotifications[i], style);
            }
        }
        else if (m_ActiveNotifications.size() > 0)
        {
            for (int i = m_ActiveNotifications.size() - 1; i >= 0; i--)
            {
                RenderDefaultNotification(m_ActiveNotifications[i], style);
            }
        }

        // Reset
        style.Colors[ImGuiCol_Text] = prevTextColors;
        ImGui::SetWindowFontScale(1);

        ImGui::End();

        /* Banner Notification */
        if (m_CurrentBannerNotif != nullptr)
        {
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x - 25, 
                main_viewport->WorkPos.y + WINDOW_HEIGHT / 2 - m_BannerWindowHeight / 2 + m_CurrentBannerNotif->m_YOffset), 0);
            ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH + 50, m_BannerWindowHeight), 0);

            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05, 0.05, 0.05, m_CurrentBannerNotif->m_Alpha);
            style.Colors[ImGuiCol_Border] = ImVec4(
                m_CurrentBannerNotif->m_TextColor.x,
                m_CurrentBannerNotif->m_TextColor.y,
                m_CurrentBannerNotif->m_TextColor.z, m_CurrentBannerNotif->m_Alpha);

            ImGui::Begin("Banner", NULL, ImGuiHelpers::MakeFlags(true, true, true, true, true, true, true, false, false, false));

            RenderBannerNotification(m_CurrentBannerNotif, style);

            // Reset
            style.Colors[ImGuiCol_Text] = prevTextColors;
            style.Colors[ImGuiCol_WindowBg] = prevWindowBgColor;
            style.Colors[ImGuiCol_Border] = prevBorderColor;
            ImGui::SetWindowFontScale(1);

            ImGui::End();
        }
    }

    float SetFontSize(NotificationInfo* info)
    {
        // Set font scale
        float oldFontSize = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale *= info->m_FontScale;
        ImGui::PushFont(ImGui::GetFont());
        return oldFontSize;
    }

    void ResetFontSize(float oldFontSize)
    {
        ImGui::GetFont()->Scale = oldFontSize;
        ImGui::PopFont();
    }

    void RenderDefaultNotification(DefaultNotificationInfo* info, ImGuiStyle& style)
    {
        // Set Text Color
        float oldFontSize = SetFontSize(info);

        ImVec4 color = info->m_TextColor;
        color.w = info->m_Duration / info->m_StartDuration;
        style.Colors[ImGuiCol_Text] = color;
        
        // Show Text
        ImGui::Text(info->m_Text.c_str());

        ImGuiHelpers::HeightenCursor(m_NotifHeight * 2);

        ResetFontSize(oldFontSize);
    }

    void RenderBannerNotification(BannerNotificationInfo* info, ImGuiStyle& style)
    {
        // Set Text Color
        float oldFontSize = SetFontSize(info);

        ImVec4 color = info->m_TextColor;
        color.w = m_CurrentBannerNotif->m_Alpha;
        style.Colors[ImGuiCol_Text] = color;

        // Show Text
        ImGuiHelpers::MakeCenterText(info->m_Text, true, true);

        ImGuiHelpers::HeightenCursor(m_NotifHeight * 2);

        ResetFontSize(oldFontSize);
    }

    ImVec4 GetNotificationColor(NotificationTextColor color)
    {
        switch (color)
        {
        case NotificationTextColor::BLUE:
            return ImVec4(0, 0, 1, 1);
        case NotificationTextColor::RED:
            return ImVec4(1, 0, 0, 1);
        case NotificationTextColor::GREEN:
            return ImVec4(0, 1, 0, 1);
        case NotificationTextColor::BLACK:
            return ImVec4(0, 0, 0, 1);
        case NotificationTextColor::WHITE:
            return ImVec4(1, 1, 1, 1);
        default:
            return ImVec4(0, 0, 0, 1);
        }
    }

    int GetNotificationScale(NotificationFontScale scale)
    {
        switch (scale)
        {
        case NotificationFontScale::SMALL:
            return .75;
        case NotificationFontScale::NORMAL:
            return 1;
        case NotificationFontScale::LARGE:
            return 1.25;
        default:
            return 1;
        }
    }
};