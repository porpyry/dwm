/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 1;
static const unsigned int gappx    = 8; /* gap >= border */
static const unsigned int snap     = 32; /* snap pixel */
static const int rmaster           = 1;  /* 1 means master-area is initially on the right */
static const int showbar           = 0;  /* 0 means no bar */
static const int topbar            = 1;  /* 0 means bottom bar */
static const char *fonts[]         = { "Noto Sans CJK KR:size=12" };
static const char dmenufont[]      = "Noto Sans CJK KR:size=12";
static const char col_nf[]         = "#a0a0a0";
static const char col_nb[]         = "#000000";
static const char col_sf[]         = "#ffffff";
static const char col_sb[]         = "#000000";
static const char col_nB[]         = "#000000";
static const char col_sB[]         = "#ffffff";
static const char *colors[][3]     = {
	/*               fg      bg      border   */
    [SchemeNorm] = { col_nf, col_nb, col_nB },
	[SchemeSel]  = { col_sf, col_sb, col_sB },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   isfakefullscreen    monitor */
	//{ "Gimp",     NULL,       NULL,       0,            1,          0,     -1 },
	//{ "Firefox",  NULL,       NULL,       1 << 8,       0,          1,     -1 },
	{ "Google-chrome", NULL, NULL, 0, 0, 1, -1 }
};

/* layout(s) */
static const float mfact        = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster        = 1;   /* number of clients in master area */
static const int resizehints    = 1;   /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 0;   /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	//{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY, view,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY, toggleview, {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY, tag,        {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY, toggletag,  {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]     = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nb, "-nf", col_nf, "-sb", col_sb, "-sf", col_sf, NULL };
static const char *termcmd[]      = { "alacritty", NULL };
static const char *browsercmd[]   = { "google-chrome-stable", NULL };
static const char *incognitocmd[] = { "google-chrome-stable", "--incognito", NULL };
static const char *editorcmd[]    = { "emacsclient", "-c", "-a", "", NULL };
static const char *bgimagecmd[]   = { "nitrogen", "--random", "--set-zoom-fill", NULL };

static const char audiomutesh[]    = "pulsemixer --toggle-mute; if pulsemixer --get-mute | grep -Fq 1; then volnoti-show -m; else volnoti-show $(pulsemixer --get-volume | cut -d' ' -f1); fi";
static const char audiovolupsh[]   = "pulsemixer --change-volume +5; pulsemixer --max-volume 100; volnoti-show $(pulsemixer --get-volume | cut -d' ' -f1)";
static const char audiovoldownsh[] = "pulsemixer --change-volume -5; volnoti-show $(pulsemixer --get-volume | cut -d' ' -f1)";

static Key keys[] = {
	// System
	{ MODKEY|ShiftMask, XK_r,      quit,           {0} }, // Refresh
	{ MODKEY,           XK_w,      killclient,     {0} },
	{ MODKEY|ShiftMask|ControlMask, XK_q, spawn,   SHCMD("sudo poweroff") },
	{ MODKEY|ShiftMask|ControlMask, XK_r, spawn,   SHCMD("sudo reboot") },

	// Programs
	{ MODKEY,           XK_q,      spawn,          {.v = dmenucmd } },
	{ MODKEY,           XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,           XK_n,      spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask, XK_n,      spawn,          {.v = incognitocmd } },
	{ MODKEY,           XK_e,      spawn,          {.v = editorcmd } },
	{ MODKEY,           XK_b,      spawn,          {.v = bgimagecmd } },

	// Focus
	{ MODKEY,           XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,           XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,           XK_Tab,    focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask, XK_Tab,    focusstack,     {.i = -1 } },
	{ MODKEY,           XK_m,      focusmaster,    {0} },
	{ MODKEY,           XK_a,      swapfocus,      {0} },

	// Zoom, swap
	{ MODKEY,           XK_space,  zoom,           {0} },
	{ MODKEY|ShiftMask, XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask, XK_k,      movestack,      {.i = -1 } },

	// Rotate
	{ MODKEY,           XK_grave,  rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask, XK_grave,  rotatestack,    {.i = -1 } },

	// Master stack number
	{ MODKEY,           XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,           XK_minus,  incnmaster,     {.i = -1 } },

	// Resize
	{ MODKEY,           XK_h,      setmfact,       {.f = -0.03} },
	{ MODKEY,           XK_l,      setmfact,       {.f = +0.03} },
	{ MODKEY,           XK_o,      setmfact,       {.f = 1.0 + mfact } },
	{ MODKEY|ShiftMask, XK_h,      setcfact,       {.f = +0.15} },
	{ MODKEY|ShiftMask, XK_l,      setcfact,       {.f = -0.15} },
	{ MODKEY|ShiftMask, XK_o,      setcfact,       {.f =  0.00} },

	// Layout
	//{ MODKEY,           XK_space,  setlayout,      {0} }, // toggle layout
	{ MODKEY,           XK_d,      togglebar,      {0} },
	{ MODKEY,           XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,           XK_backslash, togglefloating, {0} },
	{ MODKEY|ShiftMask, XK_backslash, movecenter,  {0} },
	{ MODKEY,           XK_r,      togglermaster,  {0} },

	// Monitors
	{ MODKEY,           XK_comma,  focusmon,       {.i = +1 } },
	{ MODKEY,           XK_period, focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask, XK_comma,  tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask, XK_period, tagmon,         {.i = -1 } },

	// Tags
	{ MODKEY,           XK_s,      view,           {0} },
	TAGKEYS(            XK_1,                      0)
	TAGKEYS(            XK_2,                      1)
	TAGKEYS(            XK_3,                      2)
	TAGKEYS(            XK_4,                      3)
	TAGKEYS(            XK_5,                      4)
	TAGKEYS(            XK_6,                      5)
	TAGKEYS(            XK_7,                      6)
	TAGKEYS(            XK_8,                      7)
	TAGKEYS(            XK_9,                      8)
	{ MODKEY,           XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask, XK_0,      tag,            {.ui = ~0 } },

	// Float control
	{ MODKEY,                       XK_Down,  moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,    moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right, moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,  moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,  moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,    moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right, moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,  moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,    moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,  moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,  moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right, moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,    moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,  moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,  moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right, moveresizeedge, {.v = "R"} },

	// Audio
	{ 0, XF86XK_AudioMute,        spawn, SHCMD(audiomutesh) },
	{ 0, XF86XK_AudioRaiseVolume, spawn, SHCMD(audiovolupsh) },
	{ 0, XF86XK_AudioLowerVolume, spawn, SHCMD(audiovoldownsh) },
	{ MODKEY, XK_F5, spawn, SHCMD(audiomutesh) },
	{ MODKEY, XK_F6, spawn, SHCMD(audiovoldownsh) },
	{ MODKEY, XK_F7, spawn, SHCMD(audiovolupsh) },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click         event mask button   function        argument */
	{ ClkLtSymbol,   0,         Button1, setlayout,      {0} },
	{ ClkLtSymbol,   0,         Button3, setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,   0,         Button2, zoom,           {0} },
	{ ClkStatusText, 0,         Button2, spawn,          {.v = termcmd } },
	{ ClkClientWin,  MODKEY,    Button1, movemouse,      {0} },
	{ ClkClientWin,  MODKEY,    Button2, togglefloating, {0} },
	{ ClkClientWin,  MODKEY,    Button3, resizemouse,    {0} },
	{ ClkTagBar,     0,         Button1, view,           {0} },
	{ ClkTagBar,     0,         Button3, toggleview,     {0} },
	{ ClkTagBar,     MODKEY,    Button1, tag,            {0} },
	{ ClkTagBar,     MODKEY,    Button3, toggletag,      {0} },
};

