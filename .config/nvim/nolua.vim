" my not so modern config <3
" --------------------------

set list listchars=tab:\ \ ,trail:·,extends:»,precedes:«
"
if (has("nvim"))
	let $NVIM_TUI_ENABLE_TRUE_COLOR=1
endif

if (has("termguicolors"))
	set notermguicolors
endif

"colorscheme helix
"colorscheme cybi
"colorscheme vividchalk
"colorscheme nightfly
colorscheme modus

highlight ColorColumn1  ctermbg=232
highlight ColorColumn2  ctermbg=233
highlight ColorColumn3  ctermbg=234

" Highlight at column 80
execute 'syntax match ColorColumn1 /\%81v./'

" Highlight at column 100
execute 'syntax match ColorColumn2 /\%101v./'

" sane defaults
" -------------
set ruler
set laststatus=2
set showcmd
set wildmenu
set colorcolumn=80,100,120
set scrolloff=8

" personal pref
" -------------
set number relativenumber   " <3
set pumheight=20
set nowrap
set signcolumn=yes
set path+=**                " can use find -r in n-mode and get autocomplete
syntax enable
filetype on

" TeX flavor
let g:tex_flavor = 'latex'

" tab settings
" ------------
set tabstop=4
set shiftwidth=4
"set expandtab
set smarttab

" autoindent
" ----------
set autoindent
filetype indent on

" netrw settings
" --------------
let g:netrw_banner=0    " disable the netrw giant banner
let g:netrw_altv=1      " open splits to the right
let g:netrw_liststyle=0

" search results
" --------------
set hlsearch
set incsearch   " browser-like searching
set magic       " turn on REGEX

" thank God for git
" -----------------
set noswapfile
set nobackup
set undodir=/home/cybi/.vim/undodir
set undofile

" some vs-code'ness
" -----------------
"
" Move lines|blocks up|down with Alt+k|j
nnoremap <silent> <M-Up>   :<C-u>move-2<CR>==
nnoremap <silent> <M-Down> :<C-u>move+<CR>==
xnoremap <silent> <M-Up>   :move-2<CR>gv=gv
xnoremap <silent> <M-Down> :move'>+<CR>gv=gv

" snips
" -----
nnoremap ,sort :-1read $HOME/.vim/.qsort.cpp<CR>1j2wi

