/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 * # CategoryEvents
 *
 * Event queue management.
 */

#ifndef SDL_events_h_
#define SDL_events_h_

#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pen.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_touch.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_camera.h>

#include <SDL3/SDL_begin_code.h>
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* General keyboard/mouse/pen state definitions */

/**
 * A value that signifies a button is no longer pressed.
 *
 * \since This macro is available since SDL 3.0.0.
 *
 * \sa SDL_PRESSED
 */
#define SDL_RELEASED    0

/**
 * A value that signifies a button has been pressed down.
 *
 * \since This macro is available since SDL 3.0.0.
 *
 * \sa SDL_RELEASED
 */
#define SDL_PRESSED     1


/**
 * The types of events that can be delivered.
 *
 * \since This enum is available since SDL 3.0.0.
 */
typedef enum SDL_EventType
{
    SDL_EVENT_FIRST     = 0,     /**< Unused (do not remove) */

    /* Application events */
    SDL_EVENT_QUIT           = 0x100, /**< User-requested quit */

    /* These application events have special meaning on iOS, see README-ios.md for details */
    SDL_EVENT_TERMINATING,        /**< The application is being terminated by the OS
                                     Called on iOS in applicationWillTerminate()
                                     Called on Android in onDestroy()
                                */
    SDL_EVENT_LOW_MEMORY,          /**< The application is low on memory, free memory if possible.
                                     Called on iOS in applicationDidReceiveMemoryWarning()
                                     Called on Android in onLowMemory()
                                */
    SDL_EVENT_WILL_ENTER_BACKGROUND, /**< The application is about to enter the background
                                     Called on iOS in applicationWillResignActive()
                                     Called on Android in onPause()
                                */
    SDL_EVENT_DID_ENTER_BACKGROUND, /**< The application did enter the background and may not get CPU for some time
                                     Called on iOS in applicationDidEnterBackground()
                                     Called on Android in onPause()
                                */
    SDL_EVENT_WILL_ENTER_FOREGROUND, /**< The application is about to enter the foreground
                                     Called on iOS in applicationWillEnterForeground()
                                     Called on Android in onResume()
                                */
    SDL_EVENT_DID_ENTER_FOREGROUND, /**< The application is now interactive
                                     Called on iOS in applicationDidBecomeActive()
                                     Called on Android in onResume()
                                */

    SDL_EVENT_LOCALE_CHANGED,  /**< The user's locale preferences have changed. */

    SDL_EVENT_SYSTEM_THEME_CHANGED, /**< The system theme changed */

    /* Display events */
    /* 0x150 was SDL_DISPLAYEVENT, reserve the number for sdl2-compat */
    SDL_EVENT_DISPLAY_ORIENTATION = 0x151,   /**< Display orientation has changed to data1 */
    SDL_EVENT_DISPLAY_ADDED,                 /**< Display has been added to the system */
    SDL_EVENT_DISPLAY_REMOVED,               /**< Display has been removed from the system */
    SDL_EVENT_DISPLAY_MOVED,                 /**< Display has changed position */
    SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED,  /**< Display has changed desktop mode */
    SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED,  /**< Display has changed current mode */
    SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED, /**< Display has changed content scale */
    SDL_EVENT_DISPLAY_FIRST = SDL_EVENT_DISPLAY_ORIENTATION,
    SDL_EVENT_DISPLAY_LAST = SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED,

    /* Window events */
    /* 0x200 was SDL_WINDOWEVENT, reserve the number for sdl2-compat */
    /* 0x201 was SDL_EVENT_SYSWM, reserve the number for sdl2-compat */
    SDL_EVENT_WINDOW_SHOWN = 0x202,     /**< Window has been shown */
    SDL_EVENT_WINDOW_HIDDEN,            /**< Window has been hidden */
    SDL_EVENT_WINDOW_EXPOSED,           /**< Window has been exposed and should be redrawn, and can be redrawn directly from event watchers for this event */
    SDL_EVENT_WINDOW_MOVED,             /**< Window has been moved to data1, data2 */
    SDL_EVENT_WINDOW_RESIZED,           /**< Window has been resized to data1xdata2 */
    SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,/**< The pixel size of the window has changed to data1xdata2 */
    SDL_EVENT_WINDOW_MINIMIZED,         /**< Window has been minimized */
    SDL_EVENT_WINDOW_MAXIMIZED,         /**< Window has been maximized */
    SDL_EVENT_WINDOW_RESTORED,          /**< Window has been restored to normal size and position */
    SDL_EVENT_WINDOW_MOUSE_ENTER,       /**< Window has gained mouse focus */
    SDL_EVENT_WINDOW_MOUSE_LEAVE,       /**< Window has lost mouse focus */
    SDL_EVENT_WINDOW_FOCUS_GAINED,      /**< Window has gained keyboard focus */
    SDL_EVENT_WINDOW_FOCUS_LOST,        /**< Window has lost keyboard focus */
    SDL_EVENT_WINDOW_CLOSE_REQUESTED,   /**< The window manager requests that the window be closed */
    SDL_EVENT_WINDOW_HIT_TEST,          /**< Window had a hit test that wasn't SDL_HITTEST_NORMAL */
    SDL_EVENT_WINDOW_ICCPROF_CHANGED,   /**< The ICC profile of the window's display has changed */
    SDL_EVENT_WINDOW_DISPLAY_CHANGED,   /**< Window has been moved to display data1 */
    SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED, /**< Window display scale has been changed */
    SDL_EVENT_WINDOW_OCCLUDED,          /**< The window has been occluded */
    SDL_EVENT_WINDOW_ENTER_FULLSCREEN,  /**< The window has entered fullscreen mode */
    SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,  /**< The window has left fullscreen mode */
    SDL_EVENT_WINDOW_DESTROYED,         /**< The window with the associated ID is being or has been destroyed. If this message is being handled
                                             in an event watcher, the window handle is still valid and can still be used to retrieve any userdata
                                             associated with the window. Otherwise, the handle has already been destroyed and all resources
                                             associated with it are invalid */
    SDL_EVENT_WINDOW_PEN_ENTER,         /**< Window has gained focus of the pressure-sensitive pen with ID "data1" */
    SDL_EVENT_WINDOW_PEN_LEAVE,         /**< Window has lost focus of the pressure-sensitive pen with ID "data1" */
    SDL_EVENT_WINDOW_HDR_STATE_CHANGED, /**< Window HDR properties have changed */
    SDL_EVENT_WINDOW_FIRST = SDL_EVENT_WINDOW_SHOWN,
    SDL_EVENT_WINDOW_LAST = SDL_EVENT_WINDOW_HDR_STATE_CHANGED,

    /* Keyboard events */
    SDL_EVENT_KEY_DOWN        = 0x300, /**< Key pressed */
    SDL_EVENT_KEY_UP,                  /**< Key released */
    SDL_EVENT_TEXT_EDITING,            /**< Keyboard text editing (composition) */
    SDL_EVENT_TEXT_INPUT,              /**< Keyboard text input */
    SDL_EVENT_KEYMAP_CHANGED,          /**< Keymap changed due to a system event such as an
                                            input language or keyboard layout change. */
    SDL_EVENT_KEYBOARD_ADDED,          /**< A new keyboard has been inserted into the system */
    SDL_EVENT_KEYBOARD_REMOVED,        /**< A keyboard has been removed */
    SDL_EVENT_TEXT_EDITING_CANDIDATES, /**< Keyboard text editing candidates */

    /* Mouse events */
    SDL_EVENT_MOUSE_MOTION    = 0x400, /**< Mouse moved */
    SDL_EVENT_MOUSE_BUTTON_DOWN,       /**< Mouse button pressed */
    SDL_EVENT_MOUSE_BUTTON_UP,         /**< Mouse button released */
    SDL_EVENT_MOUSE_WHEEL,             /**< Mouse wheel motion */
    SDL_EVENT_MOUSE_ADDED,             /**< A new mouse has been inserted into the system */
    SDL_EVENT_MOUSE_REMOVED,           /**< A mouse has been removed */

    /* Joystick events */
    SDL_EVENT_JOYSTICK_AXIS_MOTION  = 0x600, /**< Joystick axis motion */
    SDL_EVENT_JOYSTICK_BALL_MOTION,          /**< Joystick trackball motion */
    SDL_EVENT_JOYSTICK_HAT_MOTION,           /**< Joystick hat position change */
    SDL_EVENT_JOYSTICK_BUTTON_DOWN,          /**< Joystick button pressed */
    SDL_EVENT_JOYSTICK_BUTTON_UP,            /**< Joystick button released */
    SDL_EVENT_JOYSTICK_ADDED,                /**< A new joystick has been inserted into the system */
    SDL_EVENT_JOYSTICK_REMOVED,              /**< An opened joystick has been removed */
    SDL_EVENT_JOYSTICK_BATTERY_UPDATED,      /**< Joystick battery level change */
    SDL_EVENT_JOYSTICK_UPDATE_COMPLETE,      /**< Joystick update is complete */

    /* Gamepad events */
    SDL_EVENT_GAMEPAD_AXIS_MOTION  = 0x650, /**< Gamepad axis motion */
    SDL_EVENT_GAMEPAD_BUTTON_DOWN,          /**< Gamepad button pressed */
    SDL_EVENT_GAMEPAD_BUTTON_UP,            /**< Gamepad button released */
    SDL_EVENT_GAMEPAD_ADDED,                /**< A new gamepad has been inserted into the system */
    SDL_EVENT_GAMEPAD_REMOVED,              /**< A gamepad has been removed */
    SDL_EVENT_GAMEPAD_REMAPPED,             /**< The gamepad mapping was updated */
    SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN,        /**< Gamepad touchpad was touched */
    SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION,      /**< Gamepad touchpad finger was moved */
    SDL_EVENT_GAMEPAD_TOUCHPAD_UP,          /**< Gamepad touchpad finger was lifted */
    SDL_EVENT_GAMEPAD_SENSOR_UPDATE,        /**< Gamepad sensor was updated */
    SDL_EVENT_GAMEPAD_UPDATE_COMPLETE,      /**< Gamepad update is complete */
    SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED,  /**< Gamepad Steam handle has changed */

    /* Touch events */
    SDL_EVENT_FINGER_DOWN      = 0x700,
    SDL_EVENT_FINGER_UP,
    SDL_EVENT_FINGER_MOTION,

    /* 0x800, 0x801, and 0x802 were the Gesture events from SDL2. Do not reuse these values! sdl2-compat needs them! */

    /* Clipboard events */
    SDL_EVENT_CLIPBOARD_UPDATE = 0x900, /**< The clipboard or primary selection changed */

    /* Drag and drop events */
    SDL_EVENT_DROP_FILE        = 0x1000, /**< The system requests a file open */
    SDL_EVENT_DROP_TEXT,                 /**< text/plain drag-and-drop event */
    SDL_EVENT_DROP_BEGIN,                /**< A new set of drops is beginning (NULL filename) */
    SDL_EVENT_DROP_COMPLETE,             /**< Current set of drops is now complete (NULL filename) */
    SDL_EVENT_DROP_POSITION,             /**< Position while moving over the window */

    /* Audio hotplug events */
    SDL_EVENT_AUDIO_DEVICE_ADDED = 0x1100,  /**< A new audio device is available */
    SDL_EVENT_AUDIO_DEVICE_REMOVED,         /**< An audio device has been removed. */
    SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED,  /**< An audio device's format has been changed by the system. */

    /* Sensor events */
    SDL_EVENT_SENSOR_UPDATE = 0x1200,     /**< A sensor was updated */

    /* Pressure-sensitive pen events */
    SDL_EVENT_PEN_DOWN      = 0x1300,     /**< Pressure-sensitive pen touched drawing surface */
    SDL_EVENT_PEN_UP,                     /**< Pressure-sensitive pen stopped touching drawing surface */
    SDL_EVENT_PEN_MOTION,                 /**< Pressure-sensitive pen moved, or angle/pressure changed */
    SDL_EVENT_PEN_BUTTON_DOWN,            /**< Pressure-sensitive pen button pressed */
    SDL_EVENT_PEN_BUTTON_UP,              /**< Pressure-sensitive pen button released */

    /* Camera hotplug events */
    SDL_EVENT_CAMERA_DEVICE_ADDED = 0x1400,  /**< A new camera device is available */
    SDL_EVENT_CAMERA_DEVICE_REMOVED,         /**< A camera device has been removed. */
    SDL_EVENT_CAMERA_DEVICE_APPROVED,        /**< A camera device has been approved for use by the user. */
    SDL_EVENT_CAMERA_DEVICE_DENIED,          /**< A camera device has been denied for use by the user. */

    /* Render events */
    SDL_EVENT_RENDER_TARGETS_RESET = 0x2000, /**< The render targets have been reset and their contents need to be updated */
    SDL_EVENT_RENDER_DEVICE_RESET, /**< The device has been reset and all textures need to be recreated */

    /* Internal events */
    SDL_EVENT_POLL_SENTINEL = 0x7F00, /**< Signals the end of an event poll cycle */

    /** Events SDL_EVENT_USER through SDL_EVENT_LAST are for your use,
     *  and should be allocated with SDL_RegisterEvents()
     */
    SDL_EVENT_USER    = 0x8000,

    /**
     *  This last event is only for bounding internal arrays
     */
    SDL_EVENT_LAST    = 0xFFFF,

    /* This just makes sure the enum is the size of Uint32 */
    SDL_EVENT_ENUM_PADDING = 0x7FFFFFFF

} SDL_EventType;

/**
 * Fields shared by every event
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_CommonEvent
{
    Uint32 type;        /**< Event type, shared with all events, Uint32 to cover user events which are not in the SDL_EventType enumeration */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
} SDL_CommonEvent;

/**
 * Display state change event data (event.display.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_DisplayEvent
{
    SDL_EventType type; /**< SDL_DISPLAYEVENT_* */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_DisplayID displayID;/**< The associated display */
    Sint32 data1;       /**< event dependent data */
    Sint32 data2;       /**< event dependent data */
} SDL_DisplayEvent;

/**
 * Window state change event data (event.window.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_WindowEvent
{
    SDL_EventType type; /**< SDL_EVENT_WINDOW_* */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID; /**< The associated window */
    Sint32 data1;       /**< event dependent data */
    Sint32 data2;       /**< event dependent data */
} SDL_WindowEvent;

/**
 * Keyboard device event structure (event.kdevice.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_KeyboardDeviceEvent
{
    SDL_EventType type; /**< SDL_EVENT_KEYBOARD_ADDED or SDL_EVENT_KEYBOARD_REMOVED */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_KeyboardID which;   /**< The keyboard instance id */
} SDL_KeyboardDeviceEvent;

/**
 * Keyboard button event structure (event.key.*)
 *
 * The `key` is the base SDL_Keycode generated by pressing the `scancode`
 * using the current keyboard layout, applying any options specified in
 * SDL_HINT_KEYCODE_OPTIONS. You can get the SDL_Keycode corresponding to the
 * event scancode and modifiers directly from the keyboard layout, bypassing
 * SDL_HINT_KEYCODE_OPTIONS, by calling SDL_GetKeyFromScancode().
 *
 * \since This struct is available since SDL 3.0.0.
 *
 * \sa SDL_GetKeyFromScancode
 * \sa SDL_HINT_KEYCODE_OPTIONS
 */
typedef struct SDL_KeyboardEvent
{
    SDL_EventType type;     /**< SDL_EVENT_KEY_DOWN or SDL_EVENT_KEY_UP */
    Uint32 reserved;
    Uint64 timestamp;       /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID;  /**< The window with keyboard focus, if any */
    SDL_KeyboardID which;   /**< The keyboard instance id, or 0 if unknown or virtual */
    SDL_Scancode scancode;  /**< SDL physical key code */
    SDL_Keycode key;        /**< SDL virtual key code */
    SDL_Keymod mod;         /**< current key modifiers */
    Uint16 raw;             /**< The platform dependent scancode for this event */
    Uint8 state;            /**< SDL_PRESSED or SDL_RELEASED */
    Uint8 repeat;           /**< Non-zero if this is a key repeat */
} SDL_KeyboardEvent;

/**
 * Keyboard text editing event structure (event.edit.*)
 *
 * The start cursor is the position, in UTF-8 characters, where new typing
 * will be inserted into the editing text. The length is the number of UTF-8
 * characters that will be replaced by new typing.
 *
 * The text string is temporary memory which will be automatically freed
 * later, and can be claimed with SDL_ClaimTemporaryMemory().
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_TextEditingEvent
{
    SDL_EventType type;         /**< SDL_EVENT_TEXT_EDITING */
    Uint32 reserved;
    Uint64 timestamp;           /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID;      /**< The window with keyboard focus, if any */
    const char *text;           /**< The editing text */
    Sint32 start;               /**< The start cursor of selected editing text, or -1 if not set */
    Sint32 length;              /**< The length of selected editing text, or -1 if not set */
} SDL_TextEditingEvent;

/**
 * Keyboard IME candidates event structure (event.edit_candidates.*)
 *
 * The candidates are temporary memory which will be automatically freed
 * later, and can be claimed with SDL_ClaimTemporaryMemory().
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_TextEditingCandidatesEvent
{
    SDL_EventType type;         /**< SDL_EVENT_TEXT_EDITING_CANDIDATES */
    Uint32 reserved;
    Uint64 timestamp;           /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID;      /**< The window with keyboard focus, if any */
    const char * const *candidates;    /**< The list of candidates, or NULL if there are no candidates available */
    Sint32 num_candidates;      /**< The number of strings in `candidates` */
    Sint32 selected_candidate;  /**< The index of the selected candidate, or -1 if no candidate is selected */
    SDL_bool horizontal;          /**< SDL_TRUE if the list is horizontal, SDL_FALSE if it's vertical */
} SDL_TextEditingCandidatesEvent;

/**
 * Keyboard text input event structure (event.text.*)
 *
 * The text string is temporary memory which will be automatically freed
 * later, and can be claimed with SDL_ClaimTemporaryMemory().
 *
 * This event will never be delivered unless text input is enabled by calling
 * SDL_StartTextInput(). Text input is disabled by default!
 *
 * \since This struct is available since SDL 3.0.0.
 *
 * \sa SDL_StartTextInput
 * \sa SDL_StopTextInput
 */
typedef struct SDL_TextInputEvent
{
    SDL_EventType type; /**< SDL_EVENT_TEXT_INPUT */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID; /**< The window with keyboard focus, if any */
    const char *text;   /**< The input text, UTF-8 encoded */
} SDL_TextInputEvent;

/**
 * Mouse device event structure (event.mdevice.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_MouseDeviceEvent
{
    SDL_EventType type; /**< SDL_EVENT_MOUSE_ADDED or SDL_EVENT_MOUSE_REMOVED */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_MouseID which;  /**< The mouse instance id */
} SDL_MouseDeviceEvent;

/**
 * Mouse motion event structure (event.motion.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_MouseMotionEvent
{
    SDL_EventType type; /**< SDL_EVENT_MOUSE_MOTION */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID; /**< The window with mouse focus, if any */
    SDL_MouseID which;  /**< The mouse instance id, SDL_TOUCH_MOUSEID, or SDL_PEN_MOUSEID */
    SDL_MouseButtonFlags state;       /**< The current button state */
    float x;            /**< X coordinate, relative to window */
    float y;            /**< Y coordinate, relative to window */
    float xrel;         /**< The relative motion in the X direction */
    float yrel;         /**< The relative motion in the Y direction */
} SDL_MouseMotionEvent;

/**
 * Mouse button event structure (event.button.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_MouseButtonEvent
{
    SDL_EventType type; /**< SDL_EVENT_MOUSE_BUTTON_DOWN or SDL_EVENT_MOUSE_BUTTON_UP */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID; /**< The window with mouse focus, if any */
    SDL_MouseID which;  /**< The mouse instance id, SDL_TOUCH_MOUSEID, or SDL_PEN_MOUSEID */
    Uint8 button;       /**< The mouse button index */
    Uint8 state;        /**< SDL_PRESSED or SDL_RELEASED */
    Uint8 clicks;       /**< 1 for single-click, 2 for double-click, etc. */
    Uint8 padding;
    float x;            /**< X coordinate, relative to window */
    float y;            /**< Y coordinate, relative to window */
} SDL_MouseButtonEvent;

/**
 * Mouse wheel event structure (event.wheel.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_MouseWheelEvent
{
    SDL_EventType type; /**< SDL_EVENT_MOUSE_WHEEL */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID; /**< The window with mouse focus, if any */
    SDL_MouseID which;  /**< The mouse instance id, SDL_TOUCH_MOUSEID, or SDL_PEN_MOUSEID */
    float x;            /**< The amount scrolled horizontally, positive to the right and negative to the left */
    float y;            /**< The amount scrolled vertically, positive away from the user and negative toward the user */
    SDL_MouseWheelDirection direction; /**< Set to one of the SDL_MOUSEWHEEL_* defines. When FLIPPED the values in X and Y will be opposite. Multiply by -1 to change them back */
    float mouse_x;      /**< X coordinate, relative to window */
    float mouse_y;      /**< Y coordinate, relative to window */
} SDL_MouseWheelEvent;

/**
 * Joystick axis motion event structure (event.jaxis.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_JoyAxisEvent
{
    SDL_EventType type; /**< SDL_EVENT_JOYSTICK_AXIS_MOTION */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 axis;         /**< The joystick axis index */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;       /**< The axis value (range: -32768 to 32767) */
    Uint16 padding4;
} SDL_JoyAxisEvent;

/**
 * Joystick trackball motion event structure (event.jball.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_JoyBallEvent
{
    SDL_EventType type; /**< SDL_EVENT_JOYSTICK_BALL_MOTION */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 ball;         /**< The joystick trackball index */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 xrel;        /**< The relative motion in the X direction */
    Sint16 yrel;        /**< The relative motion in the Y direction */
} SDL_JoyBallEvent;

/**
 * Joystick hat position change event structure (event.jhat.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_JoyHatEvent
{
    SDL_EventType type; /**< SDL_EVENT_JOYSTICK_HAT_MOTION */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 hat;          /**< The joystick hat index */
    Uint8 value;        /**< The hat position value.
                         *   \sa SDL_HAT_LEFTUP SDL_HAT_UP SDL_HAT_RIGHTUP
                         *   \sa SDL_HAT_LEFT SDL_HAT_CENTERED SDL_HAT_RIGHT
                         *   \sa SDL_HAT_LEFTDOWN SDL_HAT_DOWN SDL_HAT_RIGHTDOWN
                         *
                         *   Note that zero means the POV is centered.
                         */
    Uint8 padding1;
    Uint8 padding2;
} SDL_JoyHatEvent;

/**
 * Joystick button event structure (event.jbutton.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_JoyButtonEvent
{
    SDL_EventType type; /**< SDL_EVENT_JOYSTICK_BUTTON_DOWN or SDL_EVENT_JOYSTICK_BUTTON_UP */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 button;       /**< The joystick button index */
    Uint8 state;        /**< SDL_PRESSED or SDL_RELEASED */
    Uint8 padding1;
    Uint8 padding2;
} SDL_JoyButtonEvent;

/**
 * Joystick device event structure (event.jdevice.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_JoyDeviceEvent
{
    SDL_EventType type; /**< SDL_EVENT_JOYSTICK_ADDED or SDL_EVENT_JOYSTICK_REMOVED or SDL_EVENT_JOYSTICK_UPDATE_COMPLETE */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which;       /**< The joystick instance id */
} SDL_JoyDeviceEvent;

/**
 * Joysick battery level change event structure (event.jbattery.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_JoyBatteryEvent
{
    SDL_EventType type; /**< SDL_EVENT_JOYSTICK_BATTERY_UPDATED */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    SDL_PowerState state; /**< The joystick battery state */
    int percent;          /**< The joystick battery percent charge remaining */
} SDL_JoyBatteryEvent;

/**
 * Gamepad axis motion event structure (event.gaxis.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_GamepadAxisEvent
{
    SDL_EventType type; /**< SDL_EVENT_GAMEPAD_AXIS_MOTION */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 axis;         /**< The gamepad axis (SDL_GamepadAxis) */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
    Sint16 value;       /**< The axis value (range: -32768 to 32767) */
    Uint16 padding4;
} SDL_GamepadAxisEvent;


/**
 * Gamepad button event structure (event.gbutton.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_GamepadButtonEvent
{
    SDL_EventType type; /**< SDL_EVENT_GAMEPAD_BUTTON_DOWN or SDL_EVENT_GAMEPAD_BUTTON_UP */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Uint8 button;       /**< The gamepad button (SDL_GamepadButton) */
    Uint8 state;        /**< SDL_PRESSED or SDL_RELEASED */
    Uint8 padding1;
    Uint8 padding2;
} SDL_GamepadButtonEvent;


/**
 * Gamepad device event structure (event.gdevice.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_GamepadDeviceEvent
{
    SDL_EventType type; /**< SDL_EVENT_GAMEPAD_ADDED, SDL_EVENT_GAMEPAD_REMOVED, or SDL_EVENT_GAMEPAD_REMAPPED, SDL_EVENT_GAMEPAD_UPDATE_COMPLETE or SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which;       /**< The joystick instance id */
} SDL_GamepadDeviceEvent;

/**
 * Gamepad touchpad event structure (event.gtouchpad.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_GamepadTouchpadEvent
{
    SDL_EventType type; /**< SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN or SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION or SDL_EVENT_GAMEPAD_TOUCHPAD_UP */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Sint32 touchpad;    /**< The index of the touchpad */
    Sint32 finger;      /**< The index of the finger on the touchpad */
    float x;            /**< Normalized in the range 0...1 with 0 being on the left */
    float y;            /**< Normalized in the range 0...1 with 0 being at the top */
    float pressure;     /**< Normalized in the range 0...1 */
} SDL_GamepadTouchpadEvent;

/**
 * Gamepad sensor event structure (event.gsensor.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_GamepadSensorEvent
{
    SDL_EventType type; /**< SDL_EVENT_GAMEPAD_SENSOR_UPDATE */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_JoystickID which; /**< The joystick instance id */
    Sint32 sensor;      /**< The type of the sensor, one of the values of SDL_SensorType */
    float data[3];      /**< Up to 3 values from the sensor, as defined in SDL_sensor.h */
    Uint64 sensor_timestamp; /**< The timestamp of the sensor reading in nanoseconds, not necessarily synchronized with the system clock */
} SDL_GamepadSensorEvent;

/**
 * Audio device event structure (event.adevice.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_AudioDeviceEvent
{
    SDL_EventType type; /**< SDL_EVENT_AUDIO_DEVICE_ADDED, or SDL_EVENT_AUDIO_DEVICE_REMOVED, or SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_AudioDeviceID which;       /**< SDL_AudioDeviceID for the device being added or removed or changing */
    Uint8 recording;    /**< zero if a playback device, non-zero if a recording device. */
    Uint8 padding1;
    Uint8 padding2;
    Uint8 padding3;
} SDL_AudioDeviceEvent;

/**
 * Camera device event structure (event.cdevice.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_CameraDeviceEvent
{
    SDL_EventType type; /**< SDL_EVENT_CAMERA_DEVICE_ADDED, SDL_EVENT_CAMERA_DEVICE_REMOVED, SDL_EVENT_CAMERA_DEVICE_APPROVED, SDL_EVENT_CAMERA_DEVICE_DENIED */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_CameraID which;       /**< SDL_CameraID for the device being added or removed or changing */
} SDL_CameraDeviceEvent;

/**
 * Touch finger event structure (event.tfinger.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_TouchFingerEvent
{
    SDL_EventType type; /**< SDL_EVENT_FINGER_MOTION or SDL_EVENT_FINGER_DOWN or SDL_EVENT_FINGER_UP */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_TouchID touchID; /**< The touch device id */
    SDL_FingerID fingerID;
    float x;            /**< Normalized in the range 0...1 */
    float y;            /**< Normalized in the range 0...1 */
    float dx;           /**< Normalized in the range -1...1 */
    float dy;           /**< Normalized in the range -1...1 */
    float pressure;     /**< Normalized in the range 0...1 */
    SDL_WindowID windowID; /**< The window underneath the finger, if any */
} SDL_TouchFingerEvent;


/**
 * Pressure-sensitive pen touched or stopped touching surface (event.ptip.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_PenTipEvent
{
    SDL_EventType type;     /**< SDL_EVENT_PEN_DOWN or SDL_EVENT_PEN_UP */
    Uint32 reserved;
    Uint64 timestamp;       /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID;  /**< The window with pen focus, if any */
    SDL_PenID which;        /**< The pen instance id */
    Uint8 tip;              /**< SDL_PEN_TIP_INK when using a regular pen tip, or SDL_PEN_TIP_ERASER if the pen is being used as an eraser (e.g., flipped to use the eraser tip)  */
    Uint8 state;            /**< SDL_PRESSED on SDL_EVENT_PEN_DOWN and SDL_RELEASED on SDL_EVENT_PEN_UP */
    Uint16 pen_state;       /**< Pen button masks (where SDL_BUTTON(1) is the first button, SDL_BUTTON(2) is the second button etc.), SDL_PEN_DOWN_MASK is set if the pen is touching the surface, and SDL_PEN_ERASER_MASK is set if the pen is (used as) an eraser. */
    float x;                /**< X coordinate, relative to window */
    float y;                /**< Y coordinate, relative to window */
    float axes[SDL_PEN_NUM_AXES];   /**< Pen axes such as pressure and tilt (ordered as per SDL_PenAxis) */
} SDL_PenTipEvent;

/**
 * Pressure-sensitive pen motion / pressure / angle event structure
 * (event.pmotion.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_PenMotionEvent
{
    SDL_EventType type;     /**< SDL_EVENT_PEN_MOTION */
    Uint32 reserved;
    Uint64 timestamp;       /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID;  /**< The window with pen focus, if any */
    SDL_PenID which;        /**< The pen instance id */
    Uint8 padding1;
    Uint8 padding2;
    Uint16 pen_state;       /**< Pen button masks (where SDL_BUTTON(1) is the first button, SDL_BUTTON(2) is the second button etc.), SDL_PEN_DOWN_MASK is set if the pen is touching the surface, and SDL_PEN_ERASER_MASK is set if the pen is (used as) an eraser. */
    float x;                /**< X coordinate, relative to window */
    float y;                /**< Y coordinate, relative to window */
    float axes[SDL_PEN_NUM_AXES];   /**< Pen axes such as pressure and tilt (ordered as per SDL_PenAxis) */
} SDL_PenMotionEvent;

/**
 * Pressure-sensitive pen button event structure (event.pbutton.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_PenButtonEvent
{
    SDL_EventType type;     /**< SDL_EVENT_PEN_BUTTON_DOWN or SDL_EVENT_PEN_BUTTON_UP */
    Uint32 reserved;
    Uint64 timestamp;       /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID;  /**< The window with pen focus, if any */
    SDL_PenID which;        /**< The pen instance id */
    Uint8 button;           /**< The pen button index (1 represents the pen tip for compatibility with mouse events) */
    Uint8 state;            /**< SDL_PRESSED or SDL_RELEASED */
    Uint16 pen_state;       /**< Pen button masks (where SDL_BUTTON(1) is the first button, SDL_BUTTON(2) is the second button etc.), SDL_PEN_DOWN_MASK is set if the pen is touching the surface, and SDL_PEN_ERASER_MASK is set if the pen is (used as) an eraser. */
    float x;                /**< X coordinate, relative to window */
    float y;                /**< Y coordinate, relative to window */
    float axes[SDL_PEN_NUM_AXES]; /**< Pen axes such as pressure and tilt (ordered as per SDL_PenAxis) */
} SDL_PenButtonEvent;

/**
 * An event used to drop text or request a file open by the system
 * (event.drop.*)
 *
 * The source and data strings are temporary memory which will be
 * automatically freed later, and can be claimed with
 * SDL_ClaimTemporaryMemory().
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_DropEvent
{
    SDL_EventType type; /**< SDL_EVENT_DROP_BEGIN or SDL_EVENT_DROP_FILE or SDL_EVENT_DROP_TEXT or SDL_EVENT_DROP_COMPLETE or SDL_EVENT_DROP_POSITION */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID;    /**< The window that was dropped on, if any */
    float x;            /**< X coordinate, relative to window (not on begin) */
    float y;            /**< Y coordinate, relative to window (not on begin) */
    const char *source; /**< The source app that sent this drop event, or NULL if that isn't available */
    const char *data;   /**< The text for SDL_EVENT_DROP_TEXT and the file name for SDL_EVENT_DROP_FILE, NULL for other events */
} SDL_DropEvent;

/**
 * An event triggered when the clipboard contents have changed
 * (event.clipboard.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_ClipboardEvent
{
    SDL_EventType type; /**< SDL_EVENT_CLIPBOARD_UPDATE */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
} SDL_ClipboardEvent;

/**
 * Sensor event structure (event.sensor.*)
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_SensorEvent
{
    SDL_EventType type; /**< SDL_EVENT_SENSOR_UPDATE */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_SensorID which; /**< The instance ID of the sensor */
    float data[6];      /**< Up to 6 values from the sensor - additional values can be queried using SDL_GetSensorData() */
    Uint64 sensor_timestamp; /**< The timestamp of the sensor reading in nanoseconds, not necessarily synchronized with the system clock */
} SDL_SensorEvent;

/**
 * The "quit requested" event
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_QuitEvent
{
    SDL_EventType type; /**< SDL_EVENT_QUIT */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
} SDL_QuitEvent;

/**
 * A user-defined event type (event.user.*)
 *
 * This event is unique; it is never created by SDL, but only by the
 * application. The event can be pushed onto the event queue using
 * SDL_PushEvent(). The contents of the structure members are completely up to
 * the programmer; the only requirement is that '''type''' is a value obtained
 * from SDL_RegisterEvents().
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef struct SDL_UserEvent
{
    Uint32 type;        /**< SDL_EVENT_USER through SDL_EVENT_LAST-1, Uint32 because these are not in the SDL_EventType enumeration */
    Uint32 reserved;
    Uint64 timestamp;   /**< In nanoseconds, populated using SDL_GetTicksNS() */
    SDL_WindowID windowID; /**< The associated window if any */
    Sint32 code;        /**< User defined event code */
    void *data1;        /**< User defined data pointer */
    void *data2;        /**< User defined data pointer */
} SDL_UserEvent;


/**
 * The structure for all events in SDL.
 *
 * \since This struct is available since SDL 3.0.0.
 */
typedef union SDL_Event
{
    Uint32 type;                            /**< Event type, shared with all events, Uint32 to cover user events which are not in the SDL_EventType enumeration */
    SDL_CommonEvent common;                 /**< Common event data */
    SDL_DisplayEvent display;               /**< Display event data */
    SDL_WindowEvent window;                 /**< Window event data */
    SDL_KeyboardDeviceEvent kdevice;        /**< Keyboard device change event data */
    SDL_KeyboardEvent key;                  /**< Keyboard event data */
    SDL_TextEditingEvent edit;              /**< Text editing event data */
    SDL_TextEditingCandidatesEvent edit_candidates; /**< Text editing candidates event data */
    SDL_TextInputEvent text;                /**< Text input event data */
    SDL_MouseDeviceEvent mdevice;           /**< Mouse device change event data */
    SDL_MouseMotionEvent motion;            /**< Mouse motion event data */
    SDL_MouseButtonEvent button;            /**< Mouse button event data */
    SDL_MouseWheelEvent wheel;              /**< Mouse wheel event data */
    SDL_JoyDeviceEvent jdevice;             /**< Joystick device change event data */
    SDL_JoyAxisEvent jaxis;                 /**< Joystick axis event data */
    SDL_JoyBallEvent jball;                 /**< Joystick ball event data */
    SDL_JoyHatEvent jhat;                   /**< Joystick hat event data */
    SDL_JoyButtonEvent jbutton;             /**< Joystick button event data */
    SDL_JoyBatteryEvent jbattery;           /**< Joystick battery event data */
    SDL_GamepadDeviceEvent gdevice;         /**< Gamepad device event data */
    SDL_GamepadAxisEvent gaxis;             /**< Gamepad axis event data */
    SDL_GamepadButtonEvent gbutton;         /**< Gamepad button event data */
    SDL_GamepadTouchpadEvent gtouchpad;     /**< Gamepad touchpad event data */
    SDL_GamepadSensorEvent gsensor;         /**< Gamepad sensor event data */
    SDL_AudioDeviceEvent adevice;           /**< Audio device event data */
    SDL_CameraDeviceEvent cdevice;          /**< Camera device event data */
    SDL_SensorEvent sensor;                 /**< Sensor event data */
    SDL_QuitEvent quit;                     /**< Quit request event data */
    SDL_UserEvent user;                     /**< Custom event data */
    SDL_TouchFingerEvent tfinger;           /**< Touch finger event data */
    SDL_PenTipEvent ptip;                   /**< Pen tip touching or leaving drawing surface */
    SDL_PenMotionEvent pmotion;             /**< Pen change in position, pressure, or angle */
    SDL_PenButtonEvent pbutton;             /**< Pen button press */
    SDL_DropEvent drop;                     /**< Drag and drop event data */
    SDL_ClipboardEvent clipboard;           /**< Clipboard event data */

    /* This is necessary for ABI compatibility between Visual C++ and GCC.
       Visual C++ will respect the push pack pragma and use 52 bytes (size of
       SDL_TextEditingEvent, the largest structure for 32-bit and 64-bit
       architectures) for this union, and GCC will use the alignment of the
       largest datatype within the union, which is 8 bytes on 64-bit
       architectures.

       So... we'll add padding to force the size to be the same for both.

       On architectures where pointers are 16 bytes, this needs rounding up to
       the next multiple of 16, 64, and on architectures where pointers are
       even larger the size of SDL_UserEvent will dominate as being 3 pointers.
    */
    Uint8 padding[128];
} SDL_Event;

/* Make sure we haven't broken binary compatibility */
SDL_COMPILE_TIME_ASSERT(SDL_Event, sizeof(SDL_Event) == sizeof(((SDL_Event *)NULL)->padding));


/* Function prototypes */

/**
 * Pump the event loop, gathering events from the input devices.
 *
 * This function updates the event queue and internal input device state.
 *
 * **WARNING**: This should only be run in the thread that initialized the
 * video subsystem, and for extra safety, you should consider only doing those
 * things on the main thread in any case.
 *
 * SDL_PumpEvents() gathers all the pending input information from devices and
 * places it in the event queue. Without calls to SDL_PumpEvents() no events
 * would ever be placed on the queue. Often the need for calls to
 * SDL_PumpEvents() is hidden from the user since SDL_PollEvent() and
 * SDL_WaitEvent() implicitly call SDL_PumpEvents(). However, if you are not
 * polling or waiting for events (e.g. you are filtering them), then you must
 * call SDL_PumpEvents() to force an event queue update.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_PollEvent
 * \sa SDL_WaitEvent
 */
extern SDL_DECLSPEC void SDLCALL SDL_PumpEvents(void);

/* @{ */
typedef enum SDL_EventAction
{
    SDL_ADDEVENT,
    SDL_PEEKEVENT,
    SDL_GETEVENT
} SDL_EventAction;

/**
 * Check the event queue for messages and optionally return them.
 *
 * `action` may be any of the following:
 *
 * - `SDL_ADDEVENT`: up to `numevents` events will be added to the back of the
 *   event queue.
 * - `SDL_PEEKEVENT`: `numevents` events at the front of the event queue,
 *   within the specified minimum and maximum type, will be returned to the
 *   caller and will _not_ be removed from the queue.
 * - `SDL_GETEVENT`: up to `numevents` events at the front of the event queue,
 *   within the specified minimum and maximum type, will be returned to the
 *   caller and will be removed from the queue.
 *
 * You may have to call SDL_PumpEvents() before calling this function.
 * Otherwise, the events may not be ready to be filtered when you call
 * SDL_PeepEvents().
 *
 * This function is thread-safe.
 *
 * \param events destination buffer for the retrieved events, may be NULL to
 *               leave the events in the queue and return the number of events
 *               that would have been stored.
 * \param numevents if action is SDL_ADDEVENT, the number of events to add
 *                  back to the event queue; if action is SDL_PEEKEVENT or
 *                  SDL_GETEVENT, the maximum number of events to retrieve.
 * \param action action to take; see [[#action|Remarks]] for details.
 * \param minType minimum value of the event type to be considered;
 *                SDL_EVENT_FIRST is a safe choice.
 * \param maxType maximum value of the event type to be considered;
 *                SDL_EVENT_LAST is a safe choice.
 * \returns the number of events actually stored or a negative error code on
 *          failure; call SDL_GetError() for more information.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_PollEvent
 * \sa SDL_PumpEvents
 * \sa SDL_PushEvent
 */
extern SDL_DECLSPEC int SDLCALL SDL_PeepEvents(SDL_Event *events, int numevents, SDL_EventAction action, Uint32 minType, Uint32 maxType);
/* @} */

/**
 * Check for the existence of a certain event type in the event queue.
 *
 * If you need to check for a range of event types, use SDL_HasEvents()
 * instead.
 *
 * \param type the type of event to be queried; see SDL_EventType for details.
 * \returns SDL_TRUE if events matching `type` are present, or SDL_FALSE if
 *          events matching `type` are not present.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_HasEvents
 */
extern SDL_DECLSPEC SDL_bool SDLCALL SDL_HasEvent(Uint32 type);


/**
 * Check for the existence of certain event types in the event queue.
 *
 * If you need to check for a single event type, use SDL_HasEvent() instead.
 *
 * \param minType the low end of event type to be queried, inclusive; see
 *                SDL_EventType for details.
 * \param maxType the high end of event type to be queried, inclusive; see
 *                SDL_EventType for details.
 * \returns SDL_TRUE if events with type >= `minType` and <= `maxType` are
 *          present, or SDL_FALSE if not.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_HasEvents
 */
extern SDL_DECLSPEC SDL_bool SDLCALL SDL_HasEvents(Uint32 minType, Uint32 maxType);

/**
 * Clear events of a specific type from the event queue.
 *
 * This will unconditionally remove any events from the queue that match
 * `type`. If you need to remove a range of event types, use SDL_FlushEvents()
 * instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make
 * sure that all pending OS events are flushed, you can call SDL_PumpEvents()
 * on the main thread immediately before the flush call.
 *
 * If you have user events with custom data that needs to be freed, you should
 * use SDL_PeepEvents() to remove and clean up those events before calling
 * this function.
 *
 * \param type the type of event to be cleared; see SDL_EventType for details.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_FlushEvents
 */
extern SDL_DECLSPEC void SDLCALL SDL_FlushEvent(Uint32 type);

/**
 * Clear events of a range of types from the event queue.
 *
 * This will unconditionally remove any events from the queue that are in the
 * range of `minType` to `maxType`, inclusive. If you need to remove a single
 * event type, use SDL_FlushEvent() instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make
 * sure that all pending OS events are flushed, you can call SDL_PumpEvents()
 * on the main thread immediately before the flush call.
 *
 * \param minType the low end of event type to be cleared, inclusive; see
 *                SDL_EventType for details.
 * \param maxType the high end of event type to be cleared, inclusive; see
 *                SDL_EventType for details.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_FlushEvent
 */
extern SDL_DECLSPEC void SDLCALL SDL_FlushEvents(Uint32 minType, Uint32 maxType);

/**
 * Poll for currently pending events.
 *
 * If `event` is not NULL, the next event is removed from the queue and stored
 * in the SDL_Event structure pointed to by `event`. The 1 returned refers to
 * this event, immediately stored in the SDL Event structure -- not an event
 * to follow.
 *
 * If `event` is NULL, it simply returns 1 if there is an event in the queue,
 * but will not remove it from the queue.
 *
 * As this function may implicitly call SDL_PumpEvents(), you can only call
 * this function in the thread that set the video mode.
 *
 * SDL_PollEvent() is the favored way of receiving system events since it can
 * be done from the main loop and does not suspend the main loop while waiting
 * on an event to be posted.
 *
 * The common practice is to fully process the event queue once every frame,
 * usually as a first step before updating the game's state:
 *
 * ```c
 * while (game_is_still_running) {
 *     SDL_Event event;
 *     while (SDL_PollEvent(&event)) {  // poll until all events are handled!
 *         // decide what to do with this event.
 *     }
 *
 *     // update game state, draw the current frame
 * }
 * ```
 *
 * \param event the SDL_Event structure to be filled with the next event from
 *              the queue, or NULL.
 * \returns SDL_TRUE if this got an event or SDL_FALSE if there are none
 *          available.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_PushEvent
 * \sa SDL_WaitEvent
 * \sa SDL_WaitEventTimeout
 */
extern SDL_DECLSPEC SDL_bool SDLCALL SDL_PollEvent(SDL_Event *event);

/**
 * Wait indefinitely for the next available event.
 *
 * If `event` is not NULL, the next event is removed from the queue and stored
 * in the SDL_Event structure pointed to by `event`.
 *
 * As this function may implicitly call SDL_PumpEvents(), you can only call
 * this function in the thread that initialized the video subsystem.
 *
 * \param event the SDL_Event structure to be filled in with the next event
 *              from the queue, or NULL.
 * \returns SDL_TRUE on success or SDL_FALSE if there was an error while
 *          waiting for events; call SDL_GetError() for more information.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_PollEvent
 * \sa SDL_PushEvent
 * \sa SDL_WaitEventTimeout
 */
extern SDL_DECLSPEC SDL_bool SDLCALL SDL_WaitEvent(SDL_Event *event);

/**
 * Wait until the specified timeout (in milliseconds) for the next available
 * event.
 *
 * If `event` is not NULL, the next event is removed from the queue and stored
 * in the SDL_Event structure pointed to by `event`.
 *
 * As this function may implicitly call SDL_PumpEvents(), you can only call
 * this function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * \param event the SDL_Event structure to be filled in with the next event
 *              from the queue, or NULL.
 * \param timeoutMS the maximum number of milliseconds to wait for the next
 *                  available event.
 * \returns SDL_TRUE if this got an event or SDL_FALSE if the timeout elapsed
 *          without any events available.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_PollEvent
 * \sa SDL_PushEvent
 * \sa SDL_WaitEvent
 */
extern SDL_DECLSPEC SDL_bool SDLCALL SDL_WaitEventTimeout(SDL_Event *event, Sint32 timeoutMS);

/**
 * Add an event to the event queue.
 *
 * The event queue can actually be used as a two way communication channel.
 * Not only can events be read from the queue, but the user can also push
 * their own events onto it. `event` is a pointer to the event structure you
 * wish to push onto the queue. The event is copied into the queue, and the
 * caller may dispose of the memory pointed to after SDL_PushEvent() returns.
 *
 * Note: Pushing device input events onto the queue doesn't modify the state
 * of the device within SDL.
 *
 * This function is thread-safe, and can be called from other threads safely.
 *
 * Note: Events pushed onto the queue with SDL_PushEvent() get passed through
 * the event filter but events added with SDL_PeepEvents() do not.
 *
 * For pushing application-specific events, please use SDL_RegisterEvents() to
 * get an event type that does not conflict with other code that also wants
 * its own custom event types.
 *
 * \param event the SDL_Event to be added to the queue.
 * \returns 1 on success, 0 if the event was filtered, or a negative error
 *          code on failure; call SDL_GetError() for more information. A
 *          common reason for error is the event queue being full.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_PeepEvents
 * \sa SDL_PollEvent
 * \sa SDL_RegisterEvents
 */
extern SDL_DECLSPEC int SDLCALL SDL_PushEvent(SDL_Event *event);

/**
 * A function pointer used for callbacks that watch the event queue.
 *
 * \param userdata what was passed as `userdata` to SDL_SetEventFilter() or
 *                 SDL_AddEventWatch, etc.
 * \param event the event that triggered the callback.
 * \returns 1 to permit event to be added to the queue, and 0 to disallow it.
 *          When used with SDL_AddEventWatch, the return value is ignored.
 *
 * \threadsafety SDL may call this callback at any time from any thread; the
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * \since This datatype is available since SDL 3.0.0.
 *
 * \sa SDL_SetEventFilter
 * \sa SDL_AddEventWatch
 */
typedef int (SDLCALL *SDL_EventFilter)(void *userdata, SDL_Event *event);

/**
 * Set up a filter to process all events before they change internal state and
 * are posted to the internal event queue.
 *
 * If the filter function returns 1 when called, then the event will be added
 * to the internal queue. If it returns 0, then the event will be dropped from
 * the queue, but the internal state will still be updated. This allows
 * selective filtering of dynamically arriving events.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * On platforms that support it, if the quit event is generated by an
 * interrupt signal (e.g. pressing Ctrl-C), it will be delivered to the
 * application at the next event poll.
 *
 * There is one caveat when dealing with the SDL_QuitEvent event type. The
 * event filter is only called when the window manager desires to close the
 * application window. If the event filter returns 1, then the window will be
 * closed, otherwise the window will remain open if possible.
 *
 * Note: Disabled events never make it to the event filter function; see
 * SDL_SetEventEnabled().
 *
 * Note: If you just want to inspect events without filtering, you should use
 * SDL_AddEventWatch() instead.
 *
 * Note: Events pushed onto the queue with SDL_PushEvent() get passed through
 * the event filter, but events pushed onto the queue with SDL_PeepEvents() do
 * not.
 *
 * \param filter an SDL_EventFilter function to call when an event happens.
 * \param userdata a pointer that is passed to `filter`.
 *
 * \threadsafety SDL may call the filter callback at any time from any thread;
 *               the application is responsible for locking resources the
 *               callback touches that need to be protected.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_AddEventWatch
 * \sa SDL_SetEventEnabled
 * \sa SDL_GetEventFilter
 * \sa SDL_PeepEvents
 * \sa SDL_PushEvent
 */
extern SDL_DECLSPEC void SDLCALL SDL_SetEventFilter(SDL_EventFilter filter, void *userdata);

/**
 * Query the current event filter.
 *
 * This function can be used to "chain" filters, by saving the existing filter
 * before replacing it with a function that will call that saved filter.
 *
 * \param filter the current callback function will be stored here.
 * \param userdata the pointer that is passed to the current event filter will
 *                 be stored here.
 * \returns SDL_TRUE on success or SDL_FALSE if there is no event filter set.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_SetEventFilter
 */
extern SDL_DECLSPEC SDL_bool SDLCALL SDL_GetEventFilter(SDL_EventFilter *filter, void **userdata);

/**
 * Add a callback to be triggered when an event is added to the event queue.
 *
 * `filter` will be called when an event happens, and its return value is
 * ignored.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * If the quit event is generated by a signal (e.g. SIGINT), it will bypass
 * the internal queue and be delivered to the watch callback immediately, and
 * arrive at the next event poll.
 *
 * Note: the callback is called for events posted by the user through
 * SDL_PushEvent(), but not for disabled events, nor for events by a filter
 * callback set with SDL_SetEventFilter(), nor for events posted by the user
 * through SDL_PeepEvents().
 *
 * \param filter an SDL_EventFilter function to call when an event happens.
 * \param userdata a pointer that is passed to `filter`.
 * \returns 0 on success or a negative error code on failure; call
 *          SDL_GetError() for more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_DelEventWatch
 * \sa SDL_SetEventFilter
 */
extern SDL_DECLSPEC int SDLCALL SDL_AddEventWatch(SDL_EventFilter filter, void *userdata);

/**
 * Remove an event watch callback added with SDL_AddEventWatch().
 *
 * This function takes the same input as SDL_AddEventWatch() to identify and
 * delete the corresponding callback.
 *
 * \param filter the function originally passed to SDL_AddEventWatch().
 * \param userdata the pointer originally passed to SDL_AddEventWatch().
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_AddEventWatch
 */
extern SDL_DECLSPEC void SDLCALL SDL_DelEventWatch(SDL_EventFilter filter, void *userdata);

/**
 * Run a specific filter function on the current event queue, removing any
 * events for which the filter returns 0.
 *
 * See SDL_SetEventFilter() for more information. Unlike SDL_SetEventFilter(),
 * this function does not change the filter permanently, it only uses the
 * supplied filter until this function returns.
 *
 * \param filter the SDL_EventFilter function to call when an event happens.
 * \param userdata a pointer that is passed to `filter`.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_GetEventFilter
 * \sa SDL_SetEventFilter
 */
extern SDL_DECLSPEC void SDLCALL SDL_FilterEvents(SDL_EventFilter filter, void *userdata);

/**
 * Set the state of processing events by type.
 *
 * \param type the type of event; see SDL_EventType for details.
 * \param enabled whether to process the event or not.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_EventEnabled
 */
extern SDL_DECLSPEC void SDLCALL SDL_SetEventEnabled(Uint32 type, SDL_bool enabled);

/**
 * Query the state of processing events by type.
 *
 * \param type the type of event; see SDL_EventType for details.
 * \returns SDL_TRUE if the event is being processed, SDL_FALSE otherwise.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_SetEventEnabled
 */
extern SDL_DECLSPEC SDL_bool SDLCALL SDL_EventEnabled(Uint32 type);

/**
 * Allocate a set of user-defined events, and return the beginning event
 * number for that set of events.
 *
 * \param numevents the number of events to be allocated.
 * \returns the beginning event number, or 0 if numevents is invalid or if
 *          there are not enough user-defined events left.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_PushEvent
 */
extern SDL_DECLSPEC Uint32 SDLCALL SDL_RegisterEvents(int numevents);

/**
 * Allocate temporary memory.
 *
 * You can use this to allocate memory that will be automatically freed later,
 * after event processing is complete.
 *
 * \param size the amount of memory to allocate.
 * \returns a pointer to the memory allocated or NULL on failure; call
 *          SDL_GetError() for more information.
 *
 * \threadsafety It is safe to call this function from any thread.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_ClaimTemporaryMemory
 */
extern SDL_DECLSPEC void * SDLCALL SDL_AllocateTemporaryMemory(size_t size);

/**
 * Claim ownership of temporary memory.
 *
 * Some functions return temporary memory which SDL will automatically clean
 * up. If you want to hold onto it for a long time or beyond the current
 * function scope, you can call this function to get a pointer that you own,
 * and can free using SDL_free() when you're done.
 *
 * If the memory isn't temporary, this will return NULL, and you don't have
 * ownership of the memory.
 *
 * \param mem a pointer allocated with SDL_AllocateTemporaryMemory().
 * \returns a pointer to the memory now owned by the application, which must
 *          be freed using SDL_free(), or NULL if the memory is not temporary
 *          or is not owned by this thread.
 *
 * \threadsafety It is safe to call this function from any thread.
 *
 * \since This function is available since SDL 3.0.0.
 *
 * \sa SDL_AllocateTemporaryMemory
 * \sa SDL_free
 */
extern SDL_DECLSPEC void * SDLCALL SDL_ClaimTemporaryMemory(const void *mem);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include <SDL3/SDL_close_code.h>

#endif /* SDL_events_h_ */
