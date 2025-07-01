local wezterm = require("wezterm")
local config = {}

if wezterm.config_builder then
	config = wezterm.config_builder()
end

require("theme").set(config)

config.audible_bell = "Disabled" -- No beeps

-- config.default_cursor_style = "BlinkingBlock"
-- config.cursor_blink_rate = 800

-- UI Settings
config.enable_tab_bar = false
config.window_padding = {
	left = 0,
	right = 0,
	top = 0,
	bottom = 0,
}
config.window_background_opacity = 1.0

config.font = wezterm.font({
	-- family = "IosevkaTerm NFM",
	-- family = "DejaVuSansM Nerd Font Mono",
	-- family = "MesloLGS Nerd Font Mono",
	family = "UbuntuMono Nerd Font Mono",
	-- family = "Hack",
	-- weight = "Light",
	italic = false,
	harfbuzz_features = { "calt=0", "clig=0", "liga=0" },
})

config.font_size = 13.5
-- config.font_size = 11.5
config.cell_width = 1.0
config.line_height = 1.0
config.freetype_load_target = "Light" -- Sharper rendering, easier on eyes - k
config.freetype_render_target = "HorizontalLcd" -- Optimize for LCD screens

-- Performance
config.max_fps = 120
config.animation_fps = 30
config.front_end = "OpenGL"

config.enable_scroll_bar = false

-- Wayland
config.enable_wayland = true
config.webgpu_power_preference = "HighPerformance"
config.prefer_egl = true -- Prefer EGL for better Wayland performance

config.keys = {
	-- Fullscreen toggle
	{ key = "F11", action = wezterm.action.ToggleFullScreen },
	-- Copy/Paste (standard for Linux)
	{ key = "c", mods = "CTRL|SHIFT", action = wezterm.action.CopyTo("Clipboard") },
	{ key = "v", mods = "CTRL|SHIFT", action = wezterm.action.PasteFrom("Clipboard") },
	-- Font size adjustment
	{ key = "+", mods = "CTRL", action = wezterm.action.IncreaseFontSize },
	{ key = "-", mods = "CTRL", action = wezterm.action.DecreaseFontSize },
	{ key = "0", mods = "CTRL", action = wezterm.action.ResetFontSize },
	-- Split panes for multitasking
	{ key = "|", mods = "CTRL|SHIFT", action = wezterm.action.SplitHorizontal({ domain = "CurrentPaneDomain" }) },
	-- Navigate panes
	{ key = "h", mods = "CTRL|SHIFT", action = wezterm.action.ActivatePaneDirection("Left") },
	{ key = "l", mods = "CTRL|SHIFT", action = wezterm.action.ActivatePaneDirection("Right") },
	{ key = "k", mods = "CTRL|SHIFT", action = wezterm.action.ActivatePaneDirection("Up") },
	{ key = "j", mods = "CTRL|SHIFT", action = wezterm.action.ActivatePaneDirection("Down") },
	-- Close pane
	{ key = "w", mods = "CTRL|SHIFT", action = wezterm.action.CloseCurrentPane({ confirm = true }) },
}

config.use_dead_keys = false
config.scrollback_lines = 10000

config.initial_cols = 120
config.initial_rows = 36

return config
