vim.cmd(':so $HOME/.config/nvim/nolua.vim')

require('lazy_init')

require'lspconfig'.clangd.setup{
    on_attach = function(client, bufnr)
        client.server_capabilities.semanticTokensProvider = nil -- Disable semantic highlighting
    end
}
