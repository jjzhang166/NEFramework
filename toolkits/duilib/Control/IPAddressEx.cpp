#include "StdAfx.h"
#include <Shlwapi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shlwapi.lib")

namespace ui
{
	IMPLEMENT_DUICONTROL(IPAddressEx)

    IPAddressEx::IPAddressEx()
    {
        active_section_ = 0;
        SetReadOnly(true);

        first_ = 0;
        second_ = 0;
        third_ = 0;
        fourth_ = 0;

        UpdateText();
    }

    std::wstring IPAddressEx::GetClass() const
    {
        return DUI_CTR_IPADDRESS;
    }

    LPVOID IPAddressEx::GetInterface(std::wstring pstrName)
    {
        if( _tcscmp(pstrName.c_str(), DUI_CTR_IPADDRESS) == 0 )
        {
            return static_cast<IPAddressEx*>(this);
        }

        return Edit::GetInterface(pstrName);
    }

    UINT IPAddressEx::GetControlFlags() const
    {
        if( !IsEnabled() )
        {
            return Control::GetControlFlags();
        }

        return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
    }

    void IPAddressEx::GetNumInput(TCHAR chKey)
    {
        if (chKey == 0x30 || chKey == VK_NUMPAD0) {num_char_ = '0';}
        else if (chKey == 0x31 || chKey == VK_NUMPAD1) {num_char_ = '1';}
        else if (chKey == 0x32 || chKey == VK_NUMPAD2) {num_char_ = '2';}
        else if (chKey == 0x33 || chKey == VK_NUMPAD3) {num_char_ = '3';}
        else if (chKey == 0x34 || chKey == VK_NUMPAD4) {num_char_ = '4';}
        else if (chKey == 0x35 || chKey == VK_NUMPAD5) {num_char_ = '5';}
        else if (chKey == 0x36 || chKey == VK_NUMPAD6) {num_char_ = '6';}
        else if (chKey == 0x37 || chKey == VK_NUMPAD7) {num_char_ = '7';}
        else if (chKey == 0x38 || chKey == VK_NUMPAD8) {num_char_ = '8';}
        else if (chKey == 0x39 || chKey == VK_NUMPAD9) {num_char_ = '9';}

        num_string_ += num_char_;
        CharToInt();
        if ((num_string_.length() == 3) && (active_section_ < 4))
        {
            active_section_++;
            num_string_.clear();
        }
    }

    void IPAddressEx::CharToInt()
    {
        WCHAR wszNum[MAX_PATH] = {0};
        lstrcpynW(wszNum, num_string_.c_str(), MAX_PATH);

        int nSection = _wtoi(wszNum);
        if (nSection <= 0)
        {
            nSection = 0;
        }
        else if (nSection > 255)
        {
            nSection = 255;
        }

        switch (active_section_)
        {
        case 1:
            first_ = nSection;
            break;
        case 2:
            second_ = nSection;
            break;
        case 3:
            third_ = nSection;
            break;
        case 4:
            fourth_ = nSection;
            break;
        default:
            break;
        }
        UpdateText();
    }

    void IPAddressEx::DoEvent(EventArgs& event)
    {
        if( event.type_ == kEventKillFocus && IsEnabled() ) 
        {
            active_section_ = 0;
            Invalidate();
        }
        if( event.type_ == kEventMouseButtonDown || event.type_ == kEventMouseDoubleClick || event.type_ == kEventMouseRightButtonDown)
        {
            if( !IsEnabled() )
            {
                return;
            }
            num_string_.clear();

            POINT p = event.mouse_point_;
            RECT r = GetPos();
            // 判断焦点范围确定哪一段被选中
            int nFocus = (r.right - r.left) / 4;
            if(p.x - r.left <= nFocus)
            {
                active_section_ = 1;
            }
            else if((p.x - r.left > nFocus) && (p.x - r.left <= nFocus * 2))
            {
                active_section_ = 2;
            }
            else if ((p.x - r.left > nFocus * 2) && (p.x - r.left <= nFocus * 3))
            {
                active_section_ = 3;
            }
            else
            {
                active_section_ = 4;
            }

            UpdateText();
        }
        else if( event.type_ == kMouseScrollWheel )
        {
            if( !IsEnabled() )
            {
                return;
            }

            if( event.w_param_ )
            {
                DecNum();
            }
            else
            {
                IncNum();
            }
        }
        else if( event.type_ == kEventKeyDown )
        {
            if( !IsEnabled() )
            {
                return;
            }
            // 删除
            if ((event.key_ == VK_DELETE) || (event.key_ == VK_BACK))
            {
                switch (active_section_)
                {
                    case 1:
                        first_ = 0;
                        break;
                    case 2:
                        second_ = 0;
                        break;
                    case 3:
                        third_ = 0;
                        break;
                    case 4:
                        fourth_ = 0;
                        break;
                    default:
                        break;
                }

                num_string_.clear();
                UpdateText();
            }

            // 获取输入字符
            if ((active_section_ == 1) && (event.key_ >= 0x30) && (event.key_ <= 0x39) ||
                (active_section_ == 1) && (event.key_ >= VK_NUMPAD0) && (event.key_ <= VK_NUMPAD9))
            {
                GetNumInput(event.key_);
            }
            else if ((active_section_ == 2) && (event.key_ >= 0x30) && (event.key_ <= 0x39) ||
                (active_section_ == 2) && (event.key_ >= VK_NUMPAD0) && (event.key_ <= VK_NUMPAD9))
            {
                GetNumInput(event.key_);
            }
            else if ((active_section_ == 3) && (event.key_ >= 0x30) && (event.key_ <= 0x39) ||
                (active_section_ == 3) && (event.key_ >= VK_NUMPAD0) && (event.key_ <= VK_NUMPAD9))
            {
                GetNumInput(event.key_);
            }
            else if ((active_section_ == 4) && (event.key_ >= 0x30) && (event.key_ <= 0x39) ||
                (active_section_ == 4) && (event.key_ >= VK_NUMPAD0) && (event.key_ <= VK_NUMPAD9))
            {
                GetNumInput(event.key_);
            }

            if( event.key_ == VK_UP )
            {
                IncNum();
            }
            else if( event.key_ == VK_DOWN )
            {
                DecNum();
            }
            else if( event.key_ == VK_LEFT )
            {
                if( active_section_ > 1 )
                {
                    if (!num_string_.empty())
                    {
                        CharToInt();
                        num_string_.clear();
                    }
                    active_section_--;
                    Invalidate();
                }
            }
            else if( event.key_ == VK_RIGHT )
            {
                if( active_section_ < 4 )
                {
                    if (!num_string_.empty())
                    {
                        CharToInt();
                        num_string_.clear();
                    }
                    active_section_++;
                    Invalidate();
                }
            }
            else if ((event.key_ == VK_OEM_PERIOD) || (event.key_ == VK_DECIMAL))
            {
                if( active_section_ < 4 )
                {
                    if (!num_string_.empty())
                    {
                        CharToInt();
                        num_string_.clear();
                    }
                    active_section_++;
                    Invalidate();
                }
            }
        }

        Label::DoEvent(event);
    }

    void IPAddressEx::PaintText(HDC hDC)
    {
        if( text_color_ == 0 ) text_color_ = GlobalManager::GetDefaultFontColor();
        if( disabled_text_color_ == 0 ) disabled_text_color_ = GlobalManager::GetDefaultDisabledColor();

        if( text_.empty() ) return;

        RECT rc = rect_;
        rc.left += text_padding_rect_.left;
        rc.right -= text_padding_rect_.right;
        rc.top += text_padding_rect_.top;
        rc.bottom -= text_padding_rect_.bottom;

        DWORD dwTextColor = IsEnabled() ? text_color_ : disabled_text_color_;
        HFONT hOldFont = (HFONT)::SelectObject(hDC, GlobalManager::GetFont(font_));

        char szFirst[8] = {0};
        char szSecond[8] = {0};
        char szThird[8] = {0};
        char szFourth[8] = {0};
        char szDivide[8] = {"."};

        wsprintfA(szFirst, "%d", first_);
        wsprintfA(szSecond, "%d", second_);
        wsprintfA(szThird, "%d", third_);
        wsprintfA(szFourth, "%d", fourth_);

        SIZE First;
        SIZE Second;
        SIZE Third;
        SIZE Fourth;
        SIZE divideSize;
        GetTextExtentPointA(hDC, szFirst, 3, &First);
        GetTextExtentPointA(hDC, szSecond, 3, &Second);
        GetTextExtentPointA(hDC, szThird, 3, &Third);
        GetTextExtentPointA(hDC, szFourth, 3, &Fourth);
        GetTextExtentPointA(hDC, szFourth, 1, &divideSize);

        ::SetBkMode(hDC, TRANSPARENT);
        ::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

        //Start Test Draw point (".")
        RECT rcPoint = rc;
        RECT rcIP = rc;
        int nIPAddrWidth = rcPoint.right - rcPoint.left;
        int nPointPos = nIPAddrWidth / 4;
        for (int i = 0; i < 3; i++)
        {
            rcPoint.left += nPointPos;
            ::DrawTextA(hDC, szDivide, 1, &rcPoint, DT_SINGLELINE | text_style_ | DT_NOPREFIX);
        }
        //End

        if (first_ == 0 &&
            second_ == 0 &&
            third_ == 0 &&
            fourth_ == 0 && 
            active_section_ == 0
            )
        {
            return;
        }

        int nIPPos = nPointPos / 2;
        if( 1 == active_section_ && IsEnabled() )
        {
            ::SetBkMode(hDC, OPAQUE);
            ::SetBkColor(hDC, RGB(51, 153, 255));
            ::SetTextColor(hDC, RGB(255, 255, 255));
        }
        rcIP.left = rc.left + nIPPos;
        ::DrawTextA(hDC, szFirst, 3, &rcIP, DT_SINGLELINE | text_style_ | DT_NOPREFIX);
        rc.left += nPointPos;
        ::SetBkMode(hDC, TRANSPARENT);
        ::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

        if( 2 == active_section_ && IsEnabled() )
        {
            ::SetBkMode(hDC, OPAQUE);
            ::SetBkColor(hDC, RGB(51, 153, 255));
            ::SetTextColor(hDC, RGB(255, 255, 255));
        }
        rcIP.left = rc.left + nIPPos;
        ::DrawTextA(hDC, szSecond, 3, &rcIP, DT_SINGLELINE | text_style_ | DT_NOPREFIX);
        rc.left += nPointPos;
        ::SetBkMode(hDC, TRANSPARENT);
        ::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

        if( 3 == active_section_ && IsEnabled() )
        {
            ::SetBkMode(hDC, OPAQUE);
            ::SetBkColor(hDC, RGB(51, 153, 255));
            ::SetTextColor(hDC, RGB(255, 255, 255));
        }
        rcIP.left = rc.left + nIPPos;
        ::DrawTextA(hDC, szThird, 3, &rcIP, DT_SINGLELINE | text_style_ | DT_NOPREFIX);
        rc.left += nPointPos;
        ::SetBkMode(hDC, TRANSPARENT);
        ::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

        if( 4 == active_section_ && IsEnabled() )
        {
            ::SetBkMode(hDC, OPAQUE);
            ::SetBkColor(hDC, RGB(51, 153, 255));
            ::SetTextColor(hDC, RGB(255, 255, 255));
        }
        rcIP.left = rc.left + nIPPos;
        ::DrawTextA(hDC, szFourth, 3, &rcIP, DT_SINGLELINE | text_style_ | DT_NOPREFIX);
        ::SetBkMode(hDC, TRANSPARENT);
        ::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));

        ::SelectObject(hDC, hOldFont);
    }

    void IPAddressEx::SetIP(LPCWSTR lpIP)
    {
        static int nPos = 0;
        wstring curStr;
        while (*lpIP)
        {
            if (StrChrW(L".", *lpIP))
            {
                if (curStr.size())
                {
                    switch (nPos)
                    {
                    case 0:
                        first_ = _wtoi(curStr.c_str());
                        nPos++;
                        break;
                    case 1:
                        second_ = _wtoi(curStr.c_str());
                        nPos++;
                        break;
                    case 2:
                        third_ = _wtoi(curStr.c_str());
                        nPos++;
                        break;
                    default:
                        break;
                    }
                    curStr = L"";
                }
            }
            else
            {
                curStr += (WCHAR)(*lpIP);
            }
            lpIP++;
        }
        if (curStr.size())
        {
            fourth_ = _wtoi(curStr.c_str());
            nPos = 0;
        }

        UpdateText();
    }

    std::wstring IPAddressEx::GetIP()
    {
        std::wstring strIP;
        strIP = nbase::StringPrintf(L"%d.%d.%d.%d", first_, second_, third_, fourth_);
        return strIP;
    }

    void IPAddressEx::UpdateText()
    {
        TCHAR szIP[MAX_PATH] = {0};
        _stprintf(szIP, _T("%d.%d.%d.%d"), first_, second_, third_, fourth_);
        SetText(szIP);
    }

    void IPAddressEx::IncNum()
    {
        if( active_section_ == 1 )
        {
            if (first_ < 255)
            {
                first_++;
            }
        }
        else if(active_section_ == 2)
        {
            if(second_ < 255)
            {
                second_++;
            }
        }
        else if(active_section_ == 3)
        {
            if(third_ < 255)
            {
                third_++;
            }
        }
        else if(active_section_ == 4)
        {
            if(fourth_ < 255)
            {
                fourth_++;
            }
        }

        UpdateText();
    }

    void IPAddressEx::DecNum()
    {
        if(active_section_ == 1)
        {
            if (first_ > 0)
            {
                first_--;
            }
            
        }
        else if(active_section_ == 2)
        {
            if(second_ > 0)
            {
                second_--;
            }
        }
        else if(active_section_ == 3)
        {
            if(third_ > 0)
            {
                third_--;
            }
        }
        else if(active_section_ == 4)
        {
            if(fourth_ > 0)
            {
                fourth_--;
            }
        }

        UpdateText();
    }
}