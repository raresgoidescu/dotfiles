-- Yes, I have everything in init.lua

-- Bootstrap Lazy Vim

local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not vim.loop.fs_stat(lazypath) then
  vim.fn.system({
    "git",
    "clone",
    "--filter=blob:none",
    "https://github.com/folke/lazy.nvim.git",
    "--branch=stable", -- latest stable release
    lazypath,
  })
end
vim.opt.rtp:prepend(lazypath)

-- Lazy plugins
require("lazy").setup({
	{'nvim-treesitter/nvim-treesitter'},
	{'VonHeikemen/lsp-zero.nvim', branch = 'v3.x'},
	{'williamboman/mason.nvim'},
	{'williamboman/mason-lspconfig.nvim'},
	{'neovim/nvim-lspconfig'},
	{'hrsh7th/nvim-cmp'},
	{'hrsh7th/cmp-nvim-lsp'},
	{'hrsh7th/cmp-buffer'},
	{'L3MON4D3/LuaSnip'},
	{
		'windwp/nvim-autopairs',
		event = "InsertEnter",
		config = true
		-- use opts = {} for passing setup options
		-- this is equalent to setup({}) function
	},
	{
		'nvim-telescope/telescope.nvim', tag = '0.1.6',
		dependencies = { 'nvim-lua/plenary.nvim' }
	}
})

-------------------------------------------------------------------------------

-- Editor settings
vim.wo.number = true
vim.cmd('autocmd! FileType * setlocal signcolumn=no')

-- Define the highlight group for floating windows
vim.cmd [[highlight FloatBorder guifg=#ffffff guibg=#4c566a]] -- Border color
vim.cmd [[highlight NormalFloat guifg=#d8dee9 guibg=#2e3440]] -- Content area color

-- Set the background color of the entire floating window
vim.cmd [[autocmd ColorScheme * highlight NormalFloat guibg=#2e3440]]

-- Additional adjustments if needed
vim.cmd [[autocmd ColorScheme * highlight FloatBorder guibg=#4c566a]]

-- Set LSPZero's autocomplete prompt background color
vim.cmd [[autocmd ColorScheme * hi LspFloatWinBorder guifg=#ffffff guibg=#4c566a]] -- Border color
vim.cmd [[autocmd ColorScheme * hi LspFloatWinNormal guifg=#d8dee9 guibg=#2e3440]] -- Content area color

-------------------------------------------------------------------------------

-- Plugins config
local lsp_zero = require('lsp-zero')

lsp_zero.on_attach(function(client, bufnr)
	lsp_zero.default_keymaps({buffer = bufnr})
end)

require('mason').setup({})
require('mason-lspconfig').setup({
	handlers = {
		lsp_zero.default_setup,
	},
})

local cmp = require('cmp')
local cmp_action = require('lsp-zero').cmp_action()

cmp.setup({
	sources = {
		{name = 'nvim_lsp'},
		{name = 'nvim_lua'},
	},
	mapping = cmp.mapping.preset.insert({
		-- Super-Tab to go from snip to snip
		['<Tab>'] = cmp_action.luasnip_supertab(),
    		['<S-Tab>'] = cmp_action.luasnip_shift_supertab(),
		-- `Enter` key to confirm completion
		['<CR>'] = cmp.mapping.confirm({select = true}),

		-- Ctrl+Space to trigger completion menu
		-- ['<C-Space>'] = cmp.mapping.complete(),

		-- Navigate between snippet placeholder
		['<C-f>'] = cmp_action.luasnip_jump_forward(),
		['<C-b>'] = cmp_action.luasnip_jump_backward(),

		-- Scroll up and down in the completion documentation
		['<C-u>'] = cmp.mapping.scroll_docs(-4),
		['<C-d>'] = cmp.mapping.scroll_docs(4),
	}),
	window = {
		-- Makes the autocompletion menu prettier
		completion = cmp.config.window.bordered(),
		documentation = cmp.config.window.bordered(),
	},
	formatting = {
		fields = {'abbr', 'menu', 'kind'},
		format = function(entry, item)
			local short_name = {
				nvim_lsp = 'LSP',
				nvim_lua = 'nvim'
			}

			local menu_name = short_name[entry.source.name] or entry.source.name

			item.menu = string.format('[%s]', menu_name)
			return item
		end,
	},
})
