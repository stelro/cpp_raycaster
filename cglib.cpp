//
// Created by stel on 8/10/2017.
//

#include "cglib.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <cassert>

namespace CgLib {

    Cglib::Cglib()
            : m_Window(nullptr)
              , m_Renderer(nullptr)
              , m_Event(nullptr)
              , m_Font(nullptr)
              , m_Color(0x00, 0x00, 0x00, 0xFF)
              , m_Name("Untitled")
              , m_Width(1024)
              , m_Height(768)
              , m_IsRunning(true)
              , m_CountedFrames(0)
              , m_Fps(60)
              , m_ScreenTicksPerFrame( 1000 / m_Fps )
              , m_DebugMode(false)
              , m_IsMouseMoved(false)
    {

        if (initializeSDL()) {
            assert(createScreen());
        }

        // Create PixelBuffer ( it is actually 2D Uint32 array )

        m_PixelBuffer = new Uint32*[m_Height];
        for (auto i = 0; i < m_Height; ++i) {
            m_PixelBuffer[i] = new Uint32[m_Width];
        }

        for (auto i = 0; i < m_Height; i++) {
            for (auto j = 0; j < m_Width; j++) {
                m_PixelBuffer[i][j] = 0x00;
            }
        }

        m_FPSTextTexture = new Texture(m_Renderer, m_Window);
        m_FPSTextTexture->setBlendMode(SDL_BLENDMODE_BLEND);
        m_FpsTimer.start();

    }

    Cglib::Cglib(const std::string &name, int width, int height, Uint32 fps)
            : m_Window(nullptr)
              , m_Renderer(nullptr)
              , m_Event(nullptr)
              , m_Font(nullptr)
              , m_Color(0x00, 0x00, 0x00, 0xFF)
              , m_Name(name)
              , m_Width(width)
              , m_Height(height)
              , m_IsRunning(true)
              , m_CountedFrames(0)
              , m_Fps(fps)
              , m_ScreenTicksPerFrame( 1000 / m_Fps )
              , m_DebugMode(false)
              , m_IsMouseMoved(false)
    {
        if (initializeSDL()) {
            assert(createScreen());
        }

        // Create PixelBuffer ( it is actually 2D Uint32 array )

        m_PixelBuffer = new Uint32*[m_Height];
        for (auto i = 0; i < m_Height; ++i) {
            m_PixelBuffer[i] = new Uint32[m_Width];
        }

        for (auto i = 0; i < m_Height; i++) {
            for (auto j = 0; j < m_Width; j++) {
                m_PixelBuffer[i][j] = 0x00;
            }
        }

        m_FPSTextTexture = new Texture(m_Renderer, m_Window);
        m_FPSTextTexture->setBlendMode(SDL_BLENDMODE_BLEND);
        m_FpsTimer.start();
    }

    Cglib::~Cglib()
    {
        TTF_CloseFont(m_Font);
        m_Font = nullptr;

        SDL_DestroyRenderer(m_Renderer);
        SDL_DestroyWindow(m_Window);
        m_Renderer = nullptr;
        m_Window = nullptr;

        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    bool Cglib::initializeSDL()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
            return true;
        }

        return false;
    }

    bool Cglib::createScreen()
    {
        //Set Texture filtering to Linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            std::cout << "Warning: Linear Texture filtering is not enabled!" << '\n';
        }

        //Create SDL Window
        m_Window = SDL_CreateWindow(m_Name.c_str(),
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    m_Width,
                                    m_Height,
                                    SDL_WINDOW_ALLOW_HIGHDPI
        );

        if (m_Window == nullptr) {
            std::cerr << "SDL Window could not be created! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

        if (m_Renderer == nullptr) {
            std::cerr << "SDL Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
            return false;
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
        SDL_RenderSetLogicalSize(m_Renderer, m_Width, m_Height);

        m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, m_Width, m_Height);

        if (m_Texture == nullptr) {
            std::cerr << "SDL Texture faild to initialize. SDL ERror: " << SDL_GetError() << '\n';
            return false;
        }

        //Initialize Renderer Color
        SDL_SetRenderDrawColor(m_Renderer, 0x00, 0x00, 0x00, 0x00);

        int imgFlags = IMG_INIT_PNG;

        if (!(IMG_Init(imgFlags) & imgFlags)) {
            std::cerr << "SDL_Image could not be initialized! SDL_Image Error: " << IMG_GetError() << '\n';
            return false;
        }

        m_Event = new SDL_Event();

        if (m_Event == nullptr) {
            std::cerr << "SDL_Event coult not be initialzied! " << SDL_GetError() << '\n';
            return false;
        }

        //Initialize Font
        if ( TTF_Init() == -1 ) {
            std::cerr << "SDL_TTF could not initialized! SDL_TTF Error: " << TTF_GetError() << '\n';
            return false;
        }

        return true;

    }

    void Cglib::beginRender()
    {
        m_CapTimer.start();

        //Calculate and correct fps
        m_avgFPS = m_CountedFrames / ( m_FpsTimer.getTicks() / 1000.f );
        if( m_avgFPS > 2000000 )
        {
            m_avgFPS = 0;
        }

        //Set text to be rendered
        m_TimeText.str( "" );
        m_TimeText << "@FPS: " << m_avgFPS;

        //Render text
        if( !m_FPSTextTexture->loadFromRenderedText( m_TimeText.str().c_str(),  m_FpsTextColor , m_Font ) )
        {
            printf( "Unable to render FPS texture!\n" );
        }


//        SDL_SetRenderDrawColor(m_Renderer,
//                               m_Color.r,
//                               m_Color.g,
//                               m_Color.b,
//                               m_Color.a
//        );

        SDL_UpdateTexture(m_Texture, nullptr, m_PixelBuffer, m_Width * 4);
        //Clear Screen
        SDL_RenderClear(m_Renderer);
        SDL_RenderCopy(m_Renderer, m_Texture, nullptr, nullptr);

        if ( m_DebugMode ) {

            //Render textures
            m_FPSTextTexture->render(
                    ( m_Width - m_FPSTextTexture->getWidth() - 20 ),
                    ( m_FPSTextTexture->getHeight() - 20 )
            );
        }

    }

    void Cglib::endRender()
    {
        //Update Screen
        SDL_RenderPresent(m_Renderer);
        ++m_CountedFrames;

        int frameTicks = m_CapTimer.getTicks();
        if( frameTicks < m_ScreenTicksPerFrame )
        {
            //Wait remaining time
            SDL_Delay( m_ScreenTicksPerFrame - frameTicks );
        }
    }

    void Cglib::updateEvents()
    {
        // Loop through _keyMap using a for each loop, and copy it over to _previousKeyMap
        for (auto& it : m_KeyPressed) {
            m_KeyPressedPrevious[it.first] = it.second;
        }


        while (SDL_PollEvent(m_Event) != 0) {

            switch (m_Event->type) {
                case SDL_QUIT:
                    this->requestToQuit();
                    break;
                case SDL_MOUSEMOTION:
                    setMouseCoords(m_Event->motion.x, m_Event->motion.y);
                    break;
                case SDL_KEYDOWN:
                    pressKey(m_Event->key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    releaseKey(m_Event->key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    pressKey(m_Event->button.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    releaseKey(m_Event->button.button);
                    break;
            }

        }
    }

    void Cglib::setBgColor(const ColorRGBA &color)
    {
        m_Color = color;
    }

    void Cglib::setBgColor(Uint8 r, Uint8 g, Uint8 b)
    {
        m_Color.r = r;
        m_Color.g = g;
        m_Color.b = b;
        m_Color.a = 0xFF;
    }

    void Cglib::drawRect(const Rect &rect, const ColorRGBA &color)
    {
        SDL_SetRenderDrawColor(m_Renderer,color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(m_Renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
        SDL_RenderDrawLine(m_Renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
        SDL_RenderDrawLine(m_Renderer, rect.x + rect.w, rect.y + rect.h, rect.x, rect.y + rect.h);
        SDL_RenderDrawLine(m_Renderer, rect.x, rect.y + rect.h, rect.x, rect.y);
    }

    void Cglib::drawRectFill(const Rect &rect, const ColorRGBA &color)
    {
        SDL_Rect recttmp;
        recttmp.x = rect.x;
        recttmp.y = rect.y;
        recttmp.w = rect.w;
        recttmp.h = rect.h;

        SDL_SetRenderDrawColor(m_Renderer,color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(m_Renderer, &recttmp);
        SDL_RenderDrawLine(m_Renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
        SDL_RenderDrawLine(m_Renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
        SDL_RenderDrawLine(m_Renderer, rect.x + rect.w, rect.y + rect.h, rect.x, rect.y + rect.h);
        SDL_RenderDrawLine(m_Renderer, rect.x, rect.y + rect.h, rect.x, rect.y);
    }

    void Cglib::requestToQuit()
    {
        m_IsRunning = false;
    }

    void Cglib::loadFont(const std::string &path, int size)
    {
       m_Font = TTF_OpenFont(path.c_str(), size);
        if ( m_Font == nullptr ) {
            std::cerr<< " Faild to load font: " << path << '\n';
        }
    }

    bool Cglib::drawVertLine(int x, int y1, int y2, const ColorRGBA &color)
    {

        SDL_SetRenderDrawColor(m_Renderer,color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(m_Renderer, x, y1, x, y2);
        return true;
    }

    bool Cglib::drawHorLine(int y, int x1, int x2, const ColorRGBA &color)
    {
        SDL_SetRenderDrawColor(m_Renderer,color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(m_Renderer, x1, y, x2, y);
        return true;
    }

    bool Cglib::isKeyPressed(SDL_Keycode code)
    {
        // Check if it is pressed this frame, and wasn't pressed last frame
        if (isKeyDown(code) == true && wasKeyDown(code) == false) {
            return true;
        }
        return false;
    }

    void Cglib::pressKey(SDL_Keycode code)
    {
        // Here we are treating m_KeyPressed as an associative array.
        // if keyID doesn't already exist in m_KeyPressed, it will get added
        m_KeyPressed[code] = true;
    }

    bool Cglib::wasKeyDown(SDL_Keycode code)
    {
        // We dont want to use the associative array approach here
        // because we don't want to create a key if it doesnt exist.
        // So we do it manually
        auto it = m_KeyPressedPrevious.find(code);
        if (it != m_KeyPressedPrevious.end()) {
            // Found the key
            return it->second;
        } else {
            // Didn't find the key
            return false;
        }
    }

    void Cglib::releaseKey(SDL_Keycode code)
    {
        m_KeyPressed[code] = false;
    }

    void Cglib::setMouseCoords(float x, float y)
    {
        m_MouseCoords.x = x;
        m_MouseCoords.y = y;
        this->setMouseMoved(true);
    }

    bool Cglib::isKeyDown(SDL_Keycode code)
    {
        // We dont want to use the associative array approach here
        // because we don't want to create a key if it doesnt exist.
        // So we do it manually
        auto it = m_KeyPressed.find(code);
        if (it != m_KeyPressed.end()) {
            // Found the key
            return it->second;
        } else {
            // Didn't find the key
            return false;
        }
    }

    void Cglib::drawPoint(Uint32 x, Uint32 y, Uint32 color)
    {
        if ( x > m_Width | y > m_Height)
            return;
        m_PixelBuffer[y][x] = color;
    }

    Timer::Timer()
        : m_StartTicks(0)
        , m_PausedTicks(0)
        , m_Paused(false)
        , m_Started(false)
    { }

    void Timer::start()
    {
        //Start the timer
        m_Started = true;

        //Unpause the timer
        m_Paused = false;

        //Get the current clock time
        m_StartTicks = SDL_GetTicks();
        m_PausedTicks = 0;
    }

    void Timer::stop()
    {
        //Stop the timer
        m_Started = false;

        //Unpause the timer
        m_Paused = false;

        //Clear tick variables
        m_StartTicks = 0;
        m_PausedTicks = 0;
    }

    void Timer::pause()
    {
        //If the timer is running and isn't already paused
        if( m_Started && !m_Paused )
        {
            //Pause the timer
            m_Paused = true;

            //Calculate the paused ticks
            m_PausedTicks = SDL_GetTicks() - m_StartTicks;
            m_StartTicks = 0;
        }
    }

    void Timer::unpause()
    {
        //If the timer is running and paused
        if( m_Started && m_Paused )
        {
            //Unpause the timer
            m_Paused = false;

            //Reset the starting ticks
            m_StartTicks = SDL_GetTicks() - m_PausedTicks;

            //Reset the paused ticks
            m_PausedTicks = 0;
        }
    }

    Uint32 Timer::getTicks()
    {
        //The actual timer time
        Uint32 time = 0;

        //If the timer is running
        if( m_Started )
        {
            //If the timer is paused
            if( m_Paused )
            {
                //Return the number of ticks when the timer was paused
                time = m_PausedTicks;
            }
            else
            {
                //Return the current time minus the start time
                time = SDL_GetTicks() - m_StartTicks;
            }
        }

        return time;
    }

    bool Timer::isStarted()
    {
        //Timer is running and paused or unpaused
        return m_Started;
    }

    bool Timer::isPaused()
    {
        //Timer is running and paused
        return m_Paused && m_Started;
    }

    Texture::Texture(SDL_Renderer* renderer, SDL_Window* window)
        : m_Texture(nullptr)
        , m_Renderer(renderer)
        , m_window(window)
        , m_Width(0)
        , m_Height(0) { }

    Texture::~Texture()
    {
        this->free();
    }

    bool Texture::loadFromFile(const std::string& path)
    {
        //Get rid of preexisting texture
        free();

        //The final texture
        SDL_Texture* newTexture = nullptr;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if( loadedSurface == nullptr )
        {
            printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        }
        else
        {
            //Convert surface to display format
            SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_GetWindowPixelFormat( m_window ), NULL );
            if( formattedSurface == nullptr )
            {
                printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Create blank streamable texture
                newTexture = SDL_CreateTexture( m_Renderer, SDL_GetWindowPixelFormat( m_window ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
                if( newTexture == nullptr )
                {
                    printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
                }
                else
                {
                    //Lock texture for manipulation
                    SDL_LockTexture( newTexture, nullptr, &m_Pixels, &m_Pitch );

                    //Copy loaded/formatted surface pixels
                    memcpy( m_Pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );

                    //Unlock texture to update
                    SDL_UnlockTexture( newTexture );
                    m_Pixels = nullptr;

                    //Get image dimensions
                    m_Width = formattedSurface->w;
                    m_Height = formattedSurface->h;
                }

                //Get rid of old formatted surface
                SDL_FreeSurface( formattedSurface );
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
        }

        //Return success
        m_Texture = newTexture;
        return m_Texture != nullptr;
    }

    #ifdef _SDL_TTF_H
    bool Texture::loadFromRenderedText(const std::string& textureText, SDL_Color textColor, TTF_Font* font)
    {
        //Get rid of preexisting texture
        free();


        //Render text surface
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
        if( textSurface != nullptr )
        {
            //Create texture from surface pixels
            m_Texture = SDL_CreateTextureFromSurface( m_Renderer, textSurface );
            if( m_Texture == nullptr )
            {
                printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
            }
            else
            {
                //Get image dimensions
                m_Width = textSurface->w;
                m_Height = textSurface->h;
            }

            //Get rid of old surface
            SDL_FreeSurface( textSurface );
        }
        else
        {
            printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        }


        //Return success
        return m_Texture != nullptr;
    }
    #endif

    void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
    {
        //Modulate texture rgb
        SDL_SetTextureColorMod( m_Texture, red, green, blue );
    }

    void Texture::setBlendMode(SDL_BlendMode blending)
    {
        //Set blending function
        SDL_SetTextureBlendMode( m_Texture, blending );
    }

    void Texture::setAlpha(Uint8 alpha)
    {
        //Modulate texture alpha
        SDL_SetTextureAlphaMod( m_Texture, alpha );
    }

    void Texture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { x, y, m_Width, m_Height };

        //Set clip rendering dimensions
        if( clip != nullptr )
        {
            renderQuad.w = clip->w;
            renderQuad.h = clip->h;
        }

        //Render to screen
        SDL_RenderCopyEx( m_Renderer, m_Texture, clip, &renderQuad, angle, center, flip );
    }

    void Texture::free()
    {
        //Free texture if it exists
        if( m_Texture != nullptr )
        {
            SDL_DestroyTexture( m_Texture );
            m_Texture = nullptr;
            m_Width = 0;
            m_Height = 0;
        }
    }

    bool Texture::lockTexture()
    {
        bool success = true;

        //Texture is already locked
        if( m_Pixels != nullptr )
        {
            printf( "Texture is already locked!\n" );
            success = false;
        }
            //Lock texture
        else
        {
            if( SDL_LockTexture( m_Texture, NULL, &m_Pixels, &m_Pitch ) != 0 )
            {
                printf( "Unable to lock texture! %s\n", SDL_GetError() );
                success = false;
            }
        }

        return success;
    }

    bool Texture::unlockTexture()
    {
        bool success = true;

        //Texture is not locked
        if( m_Pixels == NULL )
        {
            printf( "Texture is not locked!\n" );
            success = false;
        }
            //Unlock texture
        else
        {
            SDL_UnlockTexture( m_Texture );
            m_Pixels = NULL;
            m_Pitch = 0;
        }

        return success;
    }

    void *Texture::getPixels()
    {
        return m_Pixels;
    }

    int Texture::getPitch()
    {
        return m_Pitch;
    }

    int32_t Texture::getPixel(SDL_Surface *surface, int x, int y)
    {
        int bpp = surface->format->BytesPerPixel;
        int8_t *p = (int8_t *)surface->pixels + y * surface->pitch + x * bpp;

        switch(bpp) {
            case 1:
                return *p;
            case 2:
                return *(int16_t *)p;
            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                    return p[0] << 16 | p[1] << 8 | p[2];
                else
                    return p[0] | p[1] << 8 | p[2] << 16;
            case 4:
                return *(int32_t *)p;
            default:
                return 0;
        }
    }


    //Add two colors
    ColorRGBA operator+(const ColorRGBA& color, const ColorRGBA& color2)
    {
        ColorRGBA c;
        c.r = color.r + color2.r;
        c.g = color.g + color2.g;
        c.b = color.b + color2.b;
        return c;
    }

    //Subtract two colors
    ColorRGBA operator-(const ColorRGBA& color, const ColorRGBA& color2)
    {
        ColorRGBA c;
        c.r = color.r - color2.r;
        c.g = color.g - color2.g;
        c.b = color.b - color2.b;
        return c;
    }

    //Multiplies a color with an integer
    ColorRGBA operator*(const ColorRGBA& color, int a)
    {
        ColorRGBA c;
        c.r = color.r * a;
        c.g = color.g * a;
        c.b = color.b * a;
        return c;
    }

    //Multiplies a color with an integer
    ColorRGBA operator*(int a, const ColorRGBA& color)
    {
        ColorRGBA c;
        c.r = color.r * a;
        c.g = color.g * a;
        c.b = color.b * a;
        return c;
    }

    //Divides a color through an integer
    ColorRGBA operator/(const ColorRGBA& color, int a)
    {
        if(a == 0) return color;
        ColorRGBA c;
        c.r = color.r / a;
        c.g = color.g / a;
        c.b = color.b / a;
        return c;
    }

    //Are both colors equal?
    bool operator==(const ColorRGBA& color, const ColorRGBA& color2)
    {
        return(color.r == color2.r && color.g == color2.g && color.b == color2.b);
    }

    //Are both colors not equal?
    bool operator!=(const ColorRGBA& color, const ColorRGBA& color2)
    {
        return(!(color.r == color2.r && color.g == color2.g && color.b == color2.b));
    }
}


