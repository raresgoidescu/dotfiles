syntax on

set tabstop=4

set shiftwidth=4

set expandtab

set ai

set number relativenumber

set hlsearch

set ruler

colorscheme zaibatsu

highlight Comment ctermfg=green


" Plugins

call plug#begin()

Plug 'prabirshrestha/vim-lsp'

call plug#end()

if executable('rust-analyzer')
  au User lsp_setup call lsp#register_server({
        \   'name': 'Rust Language Server',
        \   'cmd': {server_info->['rust-analyzer']},
        \   'whitelist': ['rust'],
        \ })
endif

