" my not so modern config <3
" --------------------------

if (has("nvim"))
	let $NVIM_TUI_ENABLE_TRUE_COLOR=1
endif

if (has("termguicolors"))
	set notermguicolors
endif

colorscheme cybi

let g:mapleader=" "

" sane defaults
" -------------
set ruler
set laststatus=0
set showcmd
set wildmenu
set colorcolumn=80
set scrolloff=8

" personal pref
" -------------
set number relativenumber   " <3
set path+=**                " can use find -r in n-mode and get autocomplete
syntax enable
filetype on

" tab settings
" ------------
set tabstop=4
set shiftwidth=4
set expandtab
set smarttab

" autoindent
" ----------
set autoindent
filetype indent on

" netrw settings
" --------------
let g:netrw_banner=0    " disable the netrw giant banner
let g:netrw_altv=1      " open splits to the right
let g:netrw_liststyle=3 " tree view

" highlight search results
" ------------------------
set hlsearch    " highlight searches
set incsearch   " browser-like searching
set magic       " turn on REGEX

" some vs-code'ness
" -----------------
"
" Move lines|blocks up|down with Alt+k|j
nnoremap <silent> <M-Up>   :<C-u>move-2<CR>==
nnoremap <silent> <M-Down> :<C-u>move+<CR>==
xnoremap <silent> <M-Up>   :move-2<CR>gv=gv
xnoremap <silent> <M-Down> :move'>+<CR>gv=gv
"
" Fuzzy Find
"nnoremap <silent> <C-p> :Files<CR>
