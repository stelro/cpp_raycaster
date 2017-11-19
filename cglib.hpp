//
// Created by stel on 8/10/2017.
//

#ifndef RAYCASTING_CGLIB_HPP
#define RAYCASTING_CGLIB_HPP

#include <functional>
#include <unordered_map>
#include <cassert>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <sstream>
#include <iostream>

namespace CgLib {

    enum class FLAGS {
        NONE = 0x00,
        DEBUG = 0x01,
        RELEASE = 0x02
    };

    //RGBA Color Object
    struct ColorRGBA {

        ColorRGBA()
                : r(0x00), g(0x00), b(0x00), a(0x00) { }
        ColorRGBA(Uint8 _r, Uint8 _g, Uint8 _b)
                : r(_r), g(_g), b(_b), a(255) { }
        ColorRGBA(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a)
                : r(_r), g(_g), b(_b), a(_a) { }

        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;

    };

    ColorRGBA operator+(const ColorRGBA& color, const ColorRGBA& color2);
    ColorRGBA operator-(const ColorRGBA& color, const ColorRGBA& color2);
    ColorRGBA operator*(const ColorRGBA& color, int a);
    ColorRGBA operator*(int a, const ColorRGBA& color);
    ColorRGBA operator/(const ColorRGBA& color, int a);
    bool operator==(const ColorRGBA& color, const ColorRGBA& color2);
    bool operator!=(const ColorRGBA& color, const ColorRGBA& color2);

    static const ColorRGBA RGB_Black    (  0,   0,   0);
    static const ColorRGBA RGB_Red      (255,   0,   0);
    static const ColorRGBA RGB_Green    (  0, 255,   0);
    static const ColorRGBA RGB_Blue     (  0,   0, 255);
    static const ColorRGBA RGB_Cyan     (  0, 255, 255);
    static const ColorRGBA RGB_Magenta  (255,   0, 255);
    static const ColorRGBA RGB_Yellow   (255, 255,   0);
    static const ColorRGBA RGB_White    (255, 255, 255);
    static const ColorRGBA RGB_Gray     (128, 128, 128);
    static const ColorRGBA RGB_Grey     (192, 192, 192);
    static const ColorRGBA RGB_Maroon   (128,   0,   0);
    static const ColorRGBA RGB_Darkgreen(  0, 128,   0);
    static const ColorRGBA RGB_Navy     (  0,   0, 128);
    static const ColorRGBA RGB_Teal     (  0, 128, 128);
    static const ColorRGBA RGB_Purple   (128,   0, 128);
    static const ColorRGBA RGB_Olive    (128, 128,   0);

    //2D Point Object
    template <class T>
    struct Point {
        Point()
                : x(0) , y(0) { }

        Point(T _x, T _y)
                : x(_x), y(_y) { }

        T x;
        T y;
    };

    template <class T>
    struct Vec2 {
        Vec2<T>() : x(static_cast<T>(0.0f))
                    , y(static_cast<T>(0.0f)) {}
        Vec2<T>(T a, T b)
                : x(a), y(b) { }
        Vec2<T>(const Vec2<T>& rhs)
                : x(rhs.x), y(rhs.y) { }
        Vec2<T>(T scalar)
                : x(scalar), y(scalar) { }

        T& operator [](unsigned int index) {

            assert(index < 2);
            if (index == 0) return x;
            if (index == 1) return y;
        }

        const T& operator [](unsigned int index) const {

            assert(index < 2);
            if (index == 0) return x;
            if (index == 1) return y;
        }

        void set(T a, T b)
        {
            this->x = static_cast<T>(a);
            this->y = static_cast<T>(b);
        }


        /* Unary arithmetic operators */

        T length() const
        {
            T _x = this->x;
            T _y = this->y;
            return static_cast<T> (sqrt(_x * _x + _y * _y));
        }

        T squareLength() const
        {
            T _x = this->x;
            T _y = this->y;
            return _x * _x + _y * _y;
        }

        Vec2<T> &operator++()
        {
            ++this->x;
            ++this->y;
            return *this;
        }

        Vec2<T> &operator--()
        {
            --this->x;
            --this->y;
            return *this;
        }

        const Vec2<T> operator++(int)
        {
            Vec2<T> result(*this);
            ++*this;
            return result;
        }

        const Vec2<T> operator--(int)
        {
            Vec2 result(*this);
            --*this;
            return result;
        }

        template <typename U>
        Vec2<T> &operator=(const Vec2<U> &rhs)
        {

            this->x = rhs.x;
            this->y = rhs.y;
            return *this;
        }

        template <typename U>
        Vec2<T> &operator+=(U scalar)
        {

            this->x += scalar;
            this->y += scalar;
            return *this;

        }

        template <typename U>
        Vec2<T> &operator+=(const Vec2<U> &rhs)
        {

            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }

        template <typename U>
        Vec2<T> &operator-=(U scalar)
        {

            this->x -= scalar;
            this->y -= scalar;
            return *this;

        }

        template <typename U>
        Vec2<T> &operator-=(const Vec2<U> &rhs)
        {

            this->x -= rhs.x;
            this->y -= rhs.y;
            return *this;

        }

        template <typename U>
        Vec2<T> &operator*=(U scalar)
        {

            this->x *= scalar;
            this->y *= scalar;
            return *this;

        }

        template <typename U>
        Vec2<T> &operator*=(const Vec2<U> &rhs)
        {

            this->x *= rhs.x;
            this->y *= rhs.y;
            return *this;

        }

        template <typename U>
        Vec2<T> &operator/=(U scalar)
        {

            this->x /= scalar;
            this->y /= scalar;
            return *this;
        }

        template <typename U>
        Vec2<T> &operator/=(const Vec2<U> &rhs)
        {

            this->x /= rhs.x;
            this->y /= rhs.y;
            return *this;
        }


        void toString() const {
            std::cout << "[ " << this->x << " | " << this->y << " ]" << '\n';
        };


        T x;
        T y;
    };

    //Vector2 Binary operators
    template <typename T>
    Vec2<T> operator+(const Vec2<T> &lhs, T scalar)
    {

        return Vec2<T>(
                lhs.x + scalar,
                lhs.y + scalar
        );
    }

    template <typename T>
    Vec2<T> operator+(T scalar, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                scalar + rhs.x,
                scalar + rhs.y
        );
    }

    template <typename T>
    Vec2<T> operator+(const Vec2<T> &lhs, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                lhs.x + rhs.x,
                lhs.y + rhs.y
        );
    }

    template <typename T>
    Vec2<T> operator-(const Vec2<T> &lhs, T scalar)
    {

        return Vec2<T>(
                lhs.x - scalar,
                lhs.y - scalar
        );
    }

    template <typename T>
    Vec2<T> operator-(T scalar, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                scalar - rhs.x,
                scalar - rhs.y
        );
    }

    template <typename T>
    Vec2<T> operator-(const Vec2<T> &lhs, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                lhs.x - rhs.x,
                lhs.y - rhs.y
        );
    }

    template <typename T>
    Vec2<T> operator*(const Vec2<T> &lhs, T scalar)
    {

        return Vec2<T>(
                lhs.x * scalar,
                lhs.y * scalar
        );
    }

    template <typename T>
    Vec2<T> operator*(T scalar, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                scalar * rhs.x,
                scalar * rhs.y
        );
    }

    template <typename T>
    Vec2<T> operator*(const Vec2<T> &lhs, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                lhs.x * rhs.x,
                lhs.y * rhs.y
        );
    }

    template <typename T>
    Vec2<T> operator/(const Vec2<T> &lhs, T scalar)
    {

        return Vec2<T>(
                lhs.x / scalar,
                lhs.y / scalar
        );
    }

    template <typename T>
    Vec2<T> operator/(T scalar, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                scalar / rhs.x,
                scalar / rhs.y
        );
    }

    template <typename T>
    Vec2<T> operator/(const Vec2<T> &lhs, const Vec2<T> &rhs)
    {

        return Vec2<T>(
                lhs.x / rhs.x,
                lhs.y / rhs.y
        );
    }

    template <typename T>
    bool operator==(const Vec2<T> &lhs, const Vec2<T> &rhs)
    {

        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }

    template <typename T>
    bool operator!=(const Vec2<T> &lhs, const Vec2<T> &rhs)
    {

        return (lhs.x != rhs.x) || (lhs.y != rhs.y);
    }

    template <typename T>
    inline T dotProduct(const Vec2<T>& lhs, const Vec2<T>& rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }


    //Rectangle Object
    struct Rect {

        Rect()
                : x(0), y(0), w(0), h(0) { }

        Rect(int _x, int _y, int _w, int _h)
                : x(_x), y(_y) , w(_w), h(_h) { }

        //Compatibility with SDL
        operator SDL_Rect() {
            SDL_Rect rect;
            rect.x = x;
            rect.y = y;
            rect.w = w;
            rect.h = h;

            return rect;
        }

        int x; //X Position
        int y; //Y Position
        int w; //Width
        int h; //height
    };

    class Timer {
    public:
        //Initializes variables
        Timer();

        //The various clock actions
        void start();
        void stop();
        void pause();
        void unpause();

        //Gets the timer's time
        Uint32 getTicks();

        //Checks the status of the timer
        bool isStarted();
        bool isPaused();

    private:
        //The clock time when the timer started
        Uint32 m_StartTicks;

        //The ticks stored when the timer was paused
        Uint32 m_PausedTicks;

        //The timer status
        bool m_Paused;
        bool m_Started;
    };

    //Texture wrapper class
    class Texture {
    public:
        //Initializes variables
        Texture(SDL_Renderer* renderer, SDL_Window* window);

        //Deallocates memory
        ~Texture();

        //Loads image at specified path
        bool loadFromFile( const std::string& path );

        #ifdef _SDL_TTF_H
        //Creates image from font string
        bool loadFromRenderedText( const std::string& textureText, SDL_Color textColor, TTF_Font* font );
        #endif

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        //Renders texture at given point
        void render( int x, int y, SDL_Rect* clip = nullptr,
                     double angle = 0.0,
                     SDL_Point* center = nullptr,
                     SDL_RendererFlip flip = SDL_FLIP_NONE
        );

        //Gets image dimensions
        int getWidth() const {
            return m_Width;
        }

        int getHeight() const {
            return m_Height;
        }

        //Pixel Manipulation
        bool lockTexture();
        bool unlockTexture();
        void* getPixels();
        int getPitch();

    private:
        int32_t getPixel(SDL_Surface* surface, int x, int y);
        //The actual hardware texture
        SDL_Texture* m_Texture;

        //Renderer
        SDL_Renderer* m_Renderer;
        SDL_Window* m_window;

        //Image dimensions
        int m_Width;
        int m_Height;

        void* m_Pixels;
        int m_Pitch;
    };

    class Cglib {
    public:
        Cglib();

        Cglib(const std::string &name, int width, int height, Uint32 fps = 60);

        ~Cglib();

        Cglib(const Cglib &rhs) = delete;

        Cglib(const Cglib &&rhs) = delete;

        Cglib &operator=(const Cglib &rhs) = delete;

        Cglib &operator=(const Cglib &&rhs) = delete;

        void beginRender();
        void endRender();
        void updateEvents();


        void pressKey(SDL_Keycode code);
        void releaseKey(SDL_Keycode code);
        void setMouseCoords(float x, float y);


        //Returns true if the key is held down
        bool isKeyDown(SDL_Keycode code);

        //returns true if the key was just pressed
        bool isKeyPressed(SDL_Keycode code);

        Point<float> getMouseCoords() const { return m_MouseCoords; }

        void setBgColor(const ColorRGBA& color);
        void setBgColor(Uint8 r, Uint8 g, Uint8 b);

        void drawRect(const Rect& rect, const ColorRGBA

        & color = ColorRGBA(0xFF, 0xFF, 0xFF, 0xFF));
        void drawRectFill(const Rect& rect, const ColorRGBA& color = ColorRGBA(0xFF, 0xFF, 0xFF, 0xFF));

        //Fast vertical line from (x,y1) to (x,y2)
        bool drawVertLine(int x, int y1, int y2, const ColorRGBA& color = ColorRGBA(0xFF, 0xFF, 0xFF, 0xFF));

        //Fast Horizontal line from (x1,y) to (x2, y)
        bool drawHorLine(int y, int x1, int x2, const ColorRGBA& color = ColorRGBA(0xFF, 0xFF, 0xFF, 0xFF));

        void drawPoint(Uint32 x, Uint32 y, Uint32 color);

        void requestToQuit();

        void loadFont(const std::string& path, int size);

        void setDebugMode(bool value) {
            m_DebugMode = value;
        }

        TTF_Font* getFont() const {
            return m_Font;
        }

        /* Inline Methods */
        bool isRuning() const
        {
            return m_IsRunning;
        }

        SDL_Event *getEvent() const
        {
            return m_Event;
        }

        SDL_Renderer *getRenderer() const
        {
            return m_Renderer;
        }

        SDL_Window *getWindow() const
        {
            return m_Window;
        }

        float getAvgFPS() const {
            return m_avgFPS;
        }

        bool isMouseMoved() const {
            return m_IsMouseMoved;
        }

        void setMouseMoved(const bool value) {
            this->m_IsMouseMoved = value;
        }

    private:

        bool initializeSDL();

        bool createScreen();

        bool wasKeyDown(SDL_Keycode code);

        SDL_Window          *m_Window;
        SDL_Renderer        *m_Renderer;
        SDL_Texture         *m_Texture;
        Uint32              **m_PixelBuffer;

        SDL_Event           *m_Event;
        TTF_Font            *m_Font;

        ColorRGBA           m_Color;

        std::string         m_Name;
        int                 m_Width;
        int                 m_Height;

        bool                m_IsRunning;

        std::unordered_map<SDL_Keycode , bool> m_KeyPressed;
        std::unordered_map<SDL_Keycode , bool> m_KeyPressedPrevious;

        Timer m_FpsTimer;
        std::stringstream m_TimeText;
        int m_CountedFrames;
        //Scene textures
        Texture *m_FPSTextTexture;

        SDL_Color m_FpsTextColor = { 168, 239, 97, 255 };

        Uint32 m_Fps;
        Uint32 m_ScreenTicksPerFrame;

        //The Frames per second cap tiemr
        Timer m_CapTimer;

        bool m_DebugMode;

        Point<float> m_MouseCoords;

        float m_avgFPS;

        bool m_IsMouseMoved;
    };
}


#endif //RAYCASTING_CGLIB_HPP
