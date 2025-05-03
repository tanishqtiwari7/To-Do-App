#include "../include/gui.h"
#include <algorithm>
#include <cmath>

GUI::GUI(const std::string &title, int width, int height) 
    : window(nullptr), renderer(nullptr), windowTitle(title), screenWidth(width), screenHeight(height) {}

GUI::~GUI() {
    cleanup();
}

bool GUI::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return false;
    
    window = SDL_CreateWindow(windowTitle.c_str(), 
                             SDL_WINDOWPOS_CENTERED, 
                             SDL_WINDOWPOS_CENTERED, 
                             screenWidth, screenHeight, 
                             SDL_WINDOW_SHOWN);
    if (!window)
        return false;

    // --- Set window icon ---
    SDL_Surface* iconSurface = SDL_LoadBMP("../assets/images/logo.bmp");
    if (!iconSurface) {
        iconSurface = SDL_LoadBMP("assets/images/logo.bmp"); // Try fallback path if running from project root
    }
    if (iconSurface) {
        SDL_SetWindowIcon(window, iconSurface);
        SDL_FreeSurface(iconSurface);
    } else {
        SDL_Log("Warning: Could not load window icon: %s", SDL_GetError());
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    
    // Set default background color
    SDL_SetRenderDrawColor(renderer, 
                          COLOR_BACKGROUND.r, 
                          COLOR_BACKGROUND.g, 
                          COLOR_BACKGROUND.b, 
                          COLOR_BACKGROUND.a);
    return true;
}

void GUI::cleanup() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void GUI::clear() {
    SDL_SetRenderDrawColor(renderer, 
                          COLOR_BACKGROUND.r, 
                          COLOR_BACKGROUND.g, 
                          COLOR_BACKGROUND.b, 
                          COLOR_BACKGROUND.a);
    SDL_RenderClear(renderer);
}

void GUI::present() {
    SDL_RenderPresent(renderer);
}

void GUI::renderText(const std::string &text, int x, int y, SDL_Color color, int size, bool bold) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDLTest_DrawString(renderer, x, y, text.c_str());
}

void GUI::renderTextCentered(const std::string &text, int x, int y, int w, int h, SDL_Color color, int size, bool bold) {
    // Calculate text dimensions based on fixed font size
    int textWidth = text.length() * FONT_CHAR_WIDTH;
    int textHeight = FONT_CHAR_HEIGHT; // SDL_test_font is 8 pixels high
    
    // Ensure text fits, otherwise truncate (basic)
    std::string textToRender = text; // Renamed variable
    if (textWidth > w - PADDING) { // Add some padding
        int maxChars = (w - PADDING) / FONT_CHAR_WIDTH;
        if (maxChars > 3) {
             textToRender = text.substr(0, maxChars - 3) + "..."; // Use renamed variable
             textWidth = textToRender.length() * FONT_CHAR_WIDTH;
        } else {
            textToRender = ""; // Too small to render meaningfully
            textWidth = 0;
        }
    }

    int centerX = x + (w - textWidth) / 2;
    int centerY = y + (h - textHeight) / 2;
    
    // Adjust for potential negative coordinates if text is wider than container
    if (centerX < x) centerX = x + PADDING/2;
    if (centerY < y) centerY = y + PADDING/2;

    // Call the member function correctly
    this->renderText(textToRender, centerX, centerY, color, size, bold); 
}

void GUI::renderRect(int x, int y, int w, int h, SDL_Color fillColor, int borderSize, SDL_Color borderColor, bool shadow) {
    if (shadow) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 70);
        SDL_Rect shadowRect = {x + 3, y + 3, w, h};
        SDL_RenderFillRect(renderer, &shadowRect);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
    
    SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
    
    if (borderSize > 0) {
        SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        SDL_Rect border = {x, y, w, h};
        for (int i = 0; i < borderSize; ++i) {
             SDL_RenderDrawRect(renderer, &border);
             border.x++;
             border.y++;
             border.w -= 2;
             border.h -= 2;
        }
    }
}

void GUI::renderPanel(int x, int y, int w, int h, SDL_Color color, bool shadow, int borderSize) {
    renderRect(x, y, w, h, color, borderSize, COLOR_PANEL, shadow);
}

void GUI::renderButton(const std::string &label, int x, int y, int w, int h, bool hovered, bool enabled) {
    SDL_Color buttonColor;
    SDL_Color textColor;

    if (enabled) {
        buttonColor = hovered ? COLOR_TEXT : COLOR_PRIMARY;
        textColor = COLOR_WHITE;
    } else {
        buttonColor = COLOR_DISABLED;
        textColor = COLOR_BACKGROUND;
    }
    
    renderRect(x, y, w, h, buttonColor, 0, {0,0,0,0}, !hovered && enabled);
    renderTextCentered(label, x, y, w, h, textColor, FONT_SIZE, true);
}

void GUI::renderSecondaryButton(const std::string &label, int x, int y, int w, int h, bool hovered, bool enabled) {
    SDL_Color buttonColor;
    SDL_Color textColor;

    if (enabled) {
        buttonColor = hovered ? COLOR_PRIMARY : COLOR_PANEL;
        textColor = hovered ? COLOR_WHITE : COLOR_TEXT;
    } else {
        buttonColor = COLOR_DISABLED;
        textColor = COLOR_BACKGROUND;
    }

    renderRect(x, y, w, h, buttonColor, BORDER_SIZE, COLOR_TEXT, !hovered && enabled);
    renderTextCentered(label, x, y, w, h, textColor, FONT_SIZE, false);
}

void GUI::renderDangerButton(const std::string &label, int x, int y, int w, int h, bool hovered, bool enabled) {
    SDL_Color buttonColor;
    SDL_Color textColor;

    if (enabled) {
        buttonColor = hovered ? COLOR_TEXT : COLOR_ACCENT;
        textColor = COLOR_WHITE;
    } else {
        buttonColor = COLOR_DISABLED;
        textColor = COLOR_BACKGROUND;
    }
    
    renderRect(x, y, w, h, buttonColor, 0, {0,0,0,0}, !hovered && enabled);
    renderTextCentered(label, x, y, w, h, textColor, FONT_SIZE, true);
}

void GUI::renderInputField(const std::string &text, int x, int y, int w, int h, bool active, const std::string &placeholder) {
    SDL_Color borderColor = active ? COLOR_PRIMARY : COLOR_PANEL;
    
    renderRect(x, y, w, h, COLOR_WHITE, BORDER_SIZE, borderColor, false);
    
    int textPadding = PADDING / 2;
    int textY = y + (h - FONT_CHAR_HEIGHT) / 2;
    
    if (!text.empty()) {
        renderText(text, x + textPadding, textY, COLOR_TEXT, FONT_SIZE, false);
    } 
    else if (!placeholder.empty()) {
        SDL_Color placeholderColor = {COLOR_TEXT.r, COLOR_TEXT.g, COLOR_TEXT.b, 150};
        renderText(placeholder, x + textPadding, textY, placeholderColor, FONT_SIZE, false);
    }
    
    if (active && (SDL_GetTicks() / 500) % 2 == 0) {
        int cursorX = x + textPadding + text.length() * FONT_CHAR_WIDTH;
        int cursorY1 = y + PADDING / 2;
        int cursorY2 = y + h - PADDING / 2;
        
        SDL_SetRenderDrawColor(renderer, COLOR_PRIMARY.r, COLOR_PRIMARY.g, COLOR_PRIMARY.b, 255);
        SDL_RenderDrawLine(renderer, cursorX, cursorY1, cursorX, cursorY2);
    }
}

void GUI::renderCheckbox(int x, int y, int size, bool checked, bool hovered) {
    SDL_Color borderColor = hovered ? COLOR_PRIMARY : COLOR_PANEL;
    int border = 2;
    renderRect(x, y, size, size, COLOR_WHITE, border, borderColor, false);
    
    if (checked) {
        SDL_SetRenderDrawColor(renderer, COLOR_PRIMARY.r, COLOR_PRIMARY.g, COLOR_PRIMARY.b, COLOR_PRIMARY.a);
        int innerPad = border + 2;
        SDL_RenderDrawLine(renderer, x + innerPad, y + innerPad, x + size - innerPad - 1, y + size - innerPad - 1); 
        SDL_RenderDrawLine(renderer, x + innerPad, y + size - innerPad - 1, x + size - innerPad - 1, y + innerPad);
    }
}

void GUI::renderProgressBar(int x, int y, int w, int h, float progress) {
    progress = std::clamp(progress, 0.0f, 1.0f);
    
    renderRect(x, y, w, h, COLOR_PANEL, 1, COLOR_TEXT, false);
    
    int fillWidth = static_cast<int>((w - 2) * progress);
    if (fillWidth > 0) {
        renderRect(x + 1, y + 1, fillWidth, h - 2, COLOR_PRIMARY, 0, {0,0,0,0}, false);
    }
}

void GUI::renderTaskItem(const std::string &title, bool completed, int x, int y, int w) {
    int taskHeight = FONT_SIZE + PADDING;
    
    SDL_Color bgColor = completed ? COLOR_PANEL : COLOR_WHITE;
    SDL_Color textColor = completed ? COLOR_DISABLED : COLOR_TEXT;
    SDL_Color borderColor = completed ? COLOR_DISABLED : COLOR_PANEL;

    renderRect(x, y, w, taskHeight, bgColor, 1, borderColor, false);
    
    int checkboxSize = FONT_SIZE;
    int checkboxPadding = PADDING / 2;
    int checkboxX = x + checkboxPadding;
    int checkboxY = y + (taskHeight - checkboxSize) / 2;
    
    renderCheckbox(checkboxX, checkboxY, checkboxSize, completed, false);
    
    int textX = checkboxX + checkboxSize + checkboxPadding;
    int textY = y + (taskHeight - FONT_CHAR_HEIGHT) / 2;
    int textW = w - (textX - x) - PADDING;

    std::string renderTitle = title;
    int textWidth = title.length() * FONT_CHAR_WIDTH;
    if (textWidth > textW) {
        int maxChars = textW / FONT_CHAR_WIDTH;
         if (maxChars > 3) {
             renderTitle = title.substr(0, maxChars - 3) + "...";
        } else {
            renderTitle = ""; 
        }
    }
    
    renderText(renderTitle, textX, textY, textColor, FONT_SIZE, false);
    
    if (completed) {
        int strikeWidth = renderTitle.length() * FONT_CHAR_WIDTH;
        SDL_SetRenderDrawColor(renderer, textColor.r, textColor.g, textColor.b, 180);
        SDL_RenderDrawLine(renderer, 
                          textX, textY + FONT_CHAR_HEIGHT / 2, 
                          textX + strikeWidth, textY + FONT_CHAR_HEIGHT / 2);
        SDL_RenderDrawLine(renderer, 
                          textX, textY + FONT_CHAR_HEIGHT / 2 + 1, 
                          textX + strikeWidth, textY + FONT_CHAR_HEIGHT / 2 + 1);
    }
}