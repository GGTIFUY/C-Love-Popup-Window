#include <windows.h>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
#include <ctime>
#include <mutex>

using namespace std;

// 全局互斥锁，用于保护窗口位置
mutex posMutex;

// 窗口位置记录
vector<pair<int, int>> usedPositions;

// 祝福语列表
vector<wstring> tips = {
    L"✨ 梦里梦外，皆是你",
    L"💓 心跳为你，加速不已",
    L"🌟 想摘你眼中的星星",
    L"🌸 陪你走过，四季如歌",
    L"😊 你的笑容，最珍贵",
    L"🌙 数星星，直到天明",
    L"🌅 看遍日出，也看日落",
    L"💝 每个表情，都让我心动",
    L"🌈 陪你淋雨，也看彩虹",
    L"🎵 你的声音，最动听",
    L"🏮 看遍世间繁华",
    L"💃 每个动作，都让我着迷",
    L"🍃 走过春夏秋冬",
    L"🌹 你的世界，最美丽",
    L"💫 想做你的全世界",
    L"📸 定格每个美好瞬间",
    L"🚶 陪你走过每个角落",
    L"🌊 沉醉在你的温柔里",
    L"🛡️ 守护这份美好",
    L"🌟 所有愿望都会成真",
    L"⏰ 在时光里相爱",
    L"🍂 在季节里相守",
    L"🎨 描绘此刻的美好",
    L"📝 写下最美的诗篇",
    L"🤝 分享每个感动瞬间",
    L"🌸 珍惜这份缘分",
    L"💝 守护这份温柔",
    L"🍯 感受这份甜蜜",
    L"🎊 品味这份幸福",
    L"🌺 欣赏这份美好",
    L"☀️ 拥抱这份温暖",
    L"🌙 享受这份浪漫",
    L"💕 体会这份深情",
    L"🌸 感受这份温柔",
    L"🍬 品尝这份甜蜜",
    L"🌟 收获这份幸福",
    L"🌹 欣赏这份美丽",
    L"☀️ 感受这份温暖",
    L"🍯 品尝这份甜蜜",
    L"🎵 聆听这份幸福",
    L"🌸 感受这份美好",
    L"☀️ 拥抱这份温暖",
    L"🍬 品尝这份甜蜜",
    L"🎊 分享这份幸福",
    L"🌺 感受这份美好"
};

// 背景色列表
vector<COLORREF> bg_colors = {
    RGB(255, 182, 193),  // 浅粉色
    RGB(255, 218, 185),  // 桃色
    RGB(255, 228, 225),  // 薄雾玫瑰色
    RGB(255, 240, 245),  // 薰衣草色
    RGB(255, 250, 250),  // 雪花色
    RGB(240, 248, 255),  // 爱丽丝蓝
    RGB(240, 255, 255),  // 浅天蓝
    RGB(224, 255, 255),  // 浅青色
    RGB(255, 250, 205),  // 柠檬绸色
    RGB(255, 255, 224),  // 浅黄色
    RGB(250, 250, 210),  // 浅金色
    RGB(255, 239, 213),  // 木瓜色
    RGB(255, 228, 181),  // 桃色
    RGB(255, 218, 185),  // 桃色
    RGB(255, 192, 203),  // 粉色
    RGB(255, 182, 193),  // 浅粉色
    RGB(255, 160, 122),  // 浅鲑鱼色
    RGB(255, 160, 122),  // 浅鲑鱼色
    RGB(255, 140, 90),   // 浅砖色
    RGB(255, 140, 0),    // 深橙色
    RGB(255, 165, 0),    // 橙色
    RGB(255, 215, 0),    // 金色
    RGB(255, 255, 224),  // 浅黄色
    RGB(255, 255, 240),  // 象牙色
    RGB(255, 255, 255),  // 白色
    RGB(240, 255, 240),  // 蜜瓜色
    RGB(240, 255, 255),  // 浅天蓝
    RGB(224, 255, 255),  // 浅青色
    RGB(192, 255, 255),  // 浅青色
    RGB(175, 238, 238),  // 浅青色
    RGB(152, 251, 152),  // 浅绿色
    RGB(144, 238, 144),  // 浅绿色
    RGB(127, 255, 212),  // 浅绿色
    RGB(124, 252, 0),    // 草坪绿
    RGB(0, 255, 127),    // 春绿色
    RGB(0, 250, 154),    // 春绿色
    RGB(0, 255, 127),    // 春绿色
    RGB(173, 216, 230),  // 浅蓝色
    RGB(135, 206, 235),  // 天蓝色
    RGB(135, 206, 250),  // 浅天蓝
    RGB(176, 224, 230),  // 粉蓝色
    RGB(175, 238, 238),  // 浅青色
    RGB(255, 250, 250)   // 雪花色
};

// 文字颜色列表
vector<COLORREF> text_colors = {
    RGB(139, 0, 0),      // 深红色
    RGB(128, 0, 0),      // 栗色
    RGB(255, 255, 255),  // 白色
    RGB(0, 0, 0),        // 黑色
    RGB(105, 105, 105),  // 暗灰色
    RGB(128, 128, 128),  // 灰色
    RGB(169, 169, 169),  // 暗灰色
    RGB(192, 192, 192),  // 银色
    RGB(211, 211, 211),  // 浅灰色
    RGB(220, 220, 220),  // 浅灰色
    RGB(245, 245, 245),  // 白色烟色
    RGB(255, 250, 250),  // 雪花色
    RGB(0, 0, 255),      // 蓝色
    RGB(0, 0, 205),      // 中蓝色
    RGB(0, 0, 139),      // 深蓝色
    RGB(0, 0, 128),      // 海军蓝
    RGB(0, 0, 0),        // 黑色
    RGB(75, 0, 130),     // 靛蓝色
    RGB(72, 61, 139),    // 暗石板蓝
    RGB(106, 90, 205),   // 石板蓝
    RGB(123, 104, 238),  // 中石板蓝
    RGB(132, 112, 255),  // 浅石板蓝
    RGB(147, 112, 219),  // 中紫色
    RGB(153, 50, 204),   // 暗兰花紫
    RGB(138, 43, 226),   // 蓝紫色
    RGB(148, 0, 211),    // 暗兰花紫
    RGB(160, 32, 240),   // 紫色
    RGB(186, 85, 211),   // 中兰花紫
    RGB(218, 112, 214),  // 兰花紫
    RGB(221, 160, 221),  // 梅红色
    RGB(238, 130, 238),  // 紫罗兰色
    RGB(255, 0, 255),    // 洋红色
    RGB(255, 20, 147),   // 深粉色
    RGB(255, 105, 180),  // 热粉色
    RGB(255, 182, 193),  // 浅粉色
    RGB(255, 192, 203),  // 粉色
    RGB(250, 128, 114),  // 鲑鱼色
    RGB(255, 160, 122),  // 浅鲑鱼色
    RGB(255, 140, 90),   // 浅砖色
    RGB(255, 165, 0),    // 橙色
    RGB(255, 215, 0),    // 金色
    RGB(184, 134, 11),   // 深金色
    RGB(218, 165, 32),   // 金麒麟色
    RGB(238, 232, 170),  // 浅黄色
    RGB(255, 255, 224),  // 浅黄色
    RGB(255, 255, 240),  // 象牙色
    RGB(240, 255, 240)   // 蜜瓜色
};

struct WindowData {
    wstring text;
    COLORREF bgColor;
    COLORREF textColor;
    int x, y;
    bool isHovered = false;
};

// 检查位置是否有效
bool isPositionValid(int x, int y, int windowWidth, int windowHeight) {
    lock_guard<mutex> lock(posMutex);
    for (const auto& pos : usedPositions) {
        if (abs(x - pos.first) < windowWidth && abs(y - pos.second) < windowHeight) {
            return false;
        }
    }
    usedPositions.push_back({ x, y });
    return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static WindowData* data = nullptr;

    switch (msg) {
    case WM_CREATE: {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        data = reinterpret_cast<WindowData*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
        SetTimer(hwnd, 1, 6000, NULL);
        break;
    }

    case WM_MOUSEMOVE: {
        data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (data && !data->isHovered) {
            data->isHovered = true;
            // 放大效果
            RECT rect;
            GetWindowRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            SetWindowPos(hwnd, NULL,
                rect.left - 10, rect.top - 10,
                width + 20, height + 20,
                SWP_NOZORDER);
        }
        break;
    }

    case WM_MOUSELEAVE: {
        data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (data) {
            data->isHovered = false;
            // 恢复原始大小
            RECT rect;
            GetWindowRect(hwnd, &rect);
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            SetWindowPos(hwnd, NULL,
                rect.left + 10, rect.top + 10,
                width - 20, height - 20,
                SWP_NOZORDER);
        }
        break;
    }

    case WM_TIMER: {
        if (wParam == 1) {
            // 添加渐隐效果
            for (int alpha = 255; alpha >= 0; alpha -= 15) {
                SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
                SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
                Sleep(30);
            }
            DestroyWindow(hwnd);
        }
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);

        data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (data) {
            // 绘制背景
            HBRUSH hBrush = CreateSolidBrush(data->bgColor);
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);

            // 绘制文字
            SetTextColor(hdc, data->textColor);
            SetBkMode(hdc, TRANSPARENT);
            HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");
            SelectObject(hdc, hFont);

            // 计算文字矩形以实现垂直居中
            RECT textRect = rect;
            DrawTextW(hdc, data->text.c_str(), -1, &textRect,
                DT_CENTER | DT_WORDBREAK | DT_CALCRECT);

            // 计算垂直居中的位置
            int offsetY = (rect.bottom - textRect.bottom) / 2;
            if (offsetY < 0) offsetY = 0;
            textRect.top += offsetY;
            textRect.bottom += offsetY;

            // 绘制居中的文字
            DrawTextW(hdc, data->text.c_str(), -1, &textRect,
                DT_CENTER | DT_WORDBREAK);

            DeleteObject(hFont);
        }

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY: {
        data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (data) {
            // 清理位置记录
            lock_guard<mutex> lock(posMutex);
            usedPositions.erase(
                remove(usedPositions.begin(), usedPositions.end(),
                    make_pair(data->x, data->y)),
                usedPositions.end()
            );
            delete data;
        }
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void show_warn_tip() {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"WarnTipWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);

    // 获取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 设置窗口大小
    const int windowWidth = 300;
    const int windowHeight = 100;

    // 使用更好的随机数生成
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disX(0, screenWidth - windowWidth);
    uniform_int_distribution<> disY(0, screenHeight - windowHeight);
    uniform_int_distribution<> disText(0, tips.size() - 1);
    uniform_int_distribution<> disBg(0, bg_colors.size() - 1);
    uniform_int_distribution<> disTextCol(0, text_colors.size() - 1);

    // 创建窗口数据
    WindowData* data = new WindowData;
    data->text = tips[disText(gen)];
    data->bgColor = bg_colors[disBg(gen)];
    data->textColor = text_colors[disTextCol(gen)];

    // 获取有效位置
    int attempts = 0;
    do {
        data->x = disX(gen);
        data->y = disY(gen);
        attempts++;
    } while (!isPositionValid(data->x, data->y, windowWidth, windowHeight) && attempts < 100);

    HWND hwnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        wc.lpszClassName, L"温馨祝福",
        WS_POPUP | WS_VISIBLE,
        data->x, data->y,
        windowWidth, windowHeight,
        NULL, NULL, wc.hInstance, data
    );

    // 添加渐显效果
    for (int alpha = 0; alpha <= 255; alpha += 15) {
        SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
        Sleep(30);
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int main() {
    HWND hwnd = GetForegroundWindow();
    ShowWindow(hwnd, SW_MINIMIZE);
    // 启动后自动最小化
    const int window_count = 150;
    vector<thread> threads;

    // 创建窗口
    for (int i = 0; i < window_count; ++i) {
        threads.emplace_back(show_warn_tip);
        this_thread::sleep_for(chrono::milliseconds(20));
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
