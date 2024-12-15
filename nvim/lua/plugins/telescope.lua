return {
    "nvim-telescope/telescope.nvim",

    tag = "0.1.5",

    dependencies = {
        "nvim-lua/plenary.nvim",
		"nvim-telescope/telescope-file-browser.nvim",
    },

    config = function()
        require('telescope').setup({})

        local builtin = require('telescope.builtin')

        -- ' fw' find by word under cursor
        vim.keymap.set('n', '<leader>fw', function()
            local word = vim.fn.expand("<cword>")
            builtin.grep_string({ search = word })
        end)

        -- ' fW' find by complete word under cursor
        vim.keymap.set('n', '<leader>fW', function()
            local word = vim.fn.expand("<cWORD>")
            builtin.grep_string({ search = word })
        end)

		require('telescope').setup({
			extentions = {
				file_browser = {
					hijack_netrw = true,
				},
			},
		})
		require('telescope').load_extension("file_browser")
    end,
	keys = {
		{ "<C-p>", "<cmd>Telescope git_files<CR>", desc = "Git Files" },
		{ "<leader>ff", "<cmd>Telescope find_files<CR>", desc = "Files" },
		{ "<leader>ft", "<cmd>Telescope help_tags<CR>", desc = "Tags" },
		{ "<leader>fg", "<cmd>Telescope live_grep<CR>", desc = "Live Grep" },
		{ "<leader>fb", "<cmd>Telescope file_browser<CR>", desc = "File Browser" },
		{ "<leader>fh", "<cmd>Telescope help_tags<CR>", desc = "Help Tags" },
	},
}

