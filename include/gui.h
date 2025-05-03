#pragma once

#include "SDL.h"
#include "SDL_test_font.h"
#include <string>

// Screen Dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Modern color palette - Reassigned usage
const SDL_Color COLOR_BACKGROUND = {0xEE, 0xF5, 0xDB, 0xFF};  // Cream - Main background
const SDL_Color COLOR_PANEL = {0xB8, 0xD8, 0xD8, 0xFF};       // Light blue-green - Sidebar/Header
const SDL_Color COLOR_PRIMARY = {0x7A, 0x9E, 0x9F, 0xFF};     // Teal - Primary buttons, accents, progress
const SDL_Color COLOR_TEXT = {0x4F, 0x63, 0x67, 0xFF};        // Dark gray-blue - Main text
const SDL_Color COLOR_ACCENT = {0xFE, 0x5F, 0x55, 0xFF};      // Coral red - Danger buttons, highlights
const SDL_Color COLOR_WHITE = {0xFF, 0xFF, 0xFF, 0xFF};       // White for text on dark buttons
const SDL_Color COLOR_DISABLED = {0x99, 0x99, 0x99, 0xFF};    // Gray for disabled elements

// Adjusted font size and sizing constants
const int FONT_SIZE = 16; // Slightly larger base font for readability in smaller elements
const int FONT_CHAR_WIDTH = 8; // SDL_test_font uses 8x8 characters
const int FONT_CHAR_HEIGHT = 8;

const int BUTTON_HEIGHT = 40; // Smaller buttons
const int INPUT_HEIGHT = 40;  // Smaller input fields
const int PADDING = 15;       // Reduced padding
const int BORDER_SIZE = 2;    // Slightly smaller border

class GUI {
public:
    GUI(const std::string &title = "To-Do App", int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT);
    ~GUI();

    // Core system methods
    bool initialize();
    void cleanup();
    void clear();
    void present();
    
    // Getters
    SDL_Renderer *getRenderer() const { return renderer; }
    SDL_Window *getWindow() const { return window; }
    
    // UI rendering functions - updated signatures/implementations might be needed
    void renderText(const std::string &text, int x, int y, SDL_Color color = COLOR_TEXT, int size = FONT_SIZE, bool bold = false);
    void renderTextCentered(const std::string &text, int x, int y, int w, int h, SDL_Color color = COLOR_TEXT, int size = FONT_SIZE, bool bold = false);
    void renderPanel(int x, int y, int w, int h, SDL_Color color = COLOR_PANEL, bool shadow = false, int borderSize = BORDER_SIZE);
    void renderButton(const std::string &label, int x, int y, int w, int h, bool hovered = false, bool enabled = true);
    void renderSecondaryButton(const std::string &label, int x, int y, int w, int h, bool hovered = false, bool enabled = true);
    void renderDangerButton(const std::string &label, int x, int y, int w, int h, bool hovered = false, bool enabled = true);
    void renderInputField(const std::string &text, int x, int y, int w, int h, bool active, const std::string &placeholder = "");
    void renderCheckbox(int x, int y, int size, bool checked, bool hovered = false);
    void renderProgressBar(int x, int y, int w, int h, float progress);
    void renderTaskItem(const std::string &title, bool completed, int x, int y, int w);
    
    // Utility function
    bool isMouseOver(int x, int y, int w, int h, int mouseX, int mouseY) {
        return mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h;
    }
    
private:
    // Helper for rendering rectangles with borders
    void renderRect(int x, int y, int w, int h, SDL_Color fillColor, int borderSize = 0, SDL_Color borderColor = {0,0,0,0}, bool shadow = false);
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::string windowTitle;
    int screenWidth;
    int screenHeight;
};