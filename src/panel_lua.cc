
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "screen.h"


/**
 * Hide the panel.
 */
int
l_CPanel_hide (lua_State * l)
{
    CScreen *screen = CScreen::instance ();
    screen->hide_status_panel ();
    return 0;
}

/**
 * Show the panel.
 */
int
l_CPanel_show (lua_State * l)
{
    CScreen *screen = CScreen::instance ();
    screen->show_status_panel ();
    return 0;
}


/**
 * Get/Set the (array) of text which is displayed.
 */
int
l_CPanel_text (lua_State * l)
{
    CScreen *screen = CScreen::instance ();

    if (lua_istable (l, 2))
    {
	std::vector < std::string > vals;

	lua_pushnil (l);
	while (lua_next (l, -2))
	{
	    const char *entry = lua_tostring (l, -1);
	    vals.push_back (entry);
	    lua_pop (l, 1);
	}
	screen->status_panel_text (vals);

	return 0;
    }

  /**
   * Return the text.
   */
    std::vector < std::string > cur = screen->status_panel_text ();

    lua_newtable (l);
    int i = 1;

    for (auto it = cur.begin (); it != cur.end (); ++it)
    {
	std::string line = (*it);

	lua_pushinteger (l, i);
	lua_pushstring (l, line.c_str ());

	lua_settable (l, -3);
	i += 1;
    }

    return 1;

}

/**
 * Get/Set the title text which is displayed.
 */
int
l_CPanel_title (lua_State * l)
{
    CScreen *screen = CScreen::instance ();

    const char *str = lua_tostring (l, 2);
    if (str != NULL)
    {
	screen->status_panel_title (str);
	return 0;
    }
    else
    {
	std::string existing = screen->status_panel_title ();
	lua_pushstring (l, existing.c_str ());
	return 1;
    }
}

/**
 * Toggle the visibility of the panel.
 */
int
l_CPanel_toggle (lua_State * l)
{
    CScreen *screen = CScreen::instance ();
    screen->toggle_status_panel ();
    return 0;
}

/**
 * Is the panel visible?
 */
int
l_CPanel_visible (lua_State * l)
{
    CScreen *screen = CScreen::instance ();
    if (screen->status_panel_visible ())
	lua_pushboolean (l, 1);
    else
	lua_pushboolean (l, 0);
    return 1;
}

void
InitPanel (lua_State * l)
{
    luaL_Reg sFooRegs[] = {
	{"hide", l_CPanel_hide},
	{"show", l_CPanel_show},
	{"text", l_CPanel_text},
	{"title", l_CPanel_title},
	{"toggle", l_CPanel_toggle},
	{"visible", l_CPanel_visible},
	{NULL, NULL}
    };
    luaL_newmetatable (l, "luaL_CPanel");

#if LUA_VERSION_NUM == 501
    luaL_register (l, NULL, sFooRegs);
#elif LUA_VERSION_NUM == 502
    luaL_setfuncs (l, sFooRegs, 0);
#else
#error unsupported Lua version
#endif

    lua_pushvalue (l, -1);
    lua_setfield (l, -1, "__index");
    lua_setglobal (l, "Panel");
}