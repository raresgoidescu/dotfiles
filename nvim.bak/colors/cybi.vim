source $VIMRUNTIME/colors/vim.lua " Nvim: revert to Vim default color scheme

set background=dark
set cursorline
" set cursorcolumn

highlight clear SignColumn
highlight ColorColumn	ctermbg=233
highlight LineNr        ctermbg=0       ctermfg=12                  cterm=NONE
highlight CursorLineNr  ctermbg=233     ctermfg=12                  cterm=NONE
highlight CursorLine    ctermbg=233                                 cterm=NONE
highlight CursorColumn  ctermbg=233                                 cterm=NONE
highlight IncSearch     ctermbg=3       ctermfg=1
highlight Search        ctermbg=1       ctermfg=3
highlight Visual        ctermbg=250     ctermfg=233
highlight Pmenu         ctermbg=240     ctermfg=12
highlight PmenuSel      ctermbg=3       ctermfg=1
highlight SpellBad      ctermbg=0       ctermfg=1
highlight SpellCap      ctermbg=4       ctermfg=15
highlight VertSplit     ctermbg=6       ctermfg=0
highlight MatchParen    ctermbg=241
highlight SpellLocal    ctermbg=55
highlight Search        ctermbg=16      ctermfg=15

highlight Comment                       ctermfg=248                 cterm=bold
highlight Whitespace                    ctermfg=240
highlight Identifier                    ctermfg=15
highlight @function.call.c              ctermfg=15
highlight @function.c                   ctermfg=15
highlight Constant                      ctermfg=13

highlight clear MatchParen
highlight DiffAdd       ctermbg=4       ctermfg=15
highlight DiffDelete    ctermbg=5       ctermfg=14
highlight DiffChange    ctermbg=238

" Code folding
highlight Folded        ctermbg=0       ctermfg=1
highlight FoldColumn    ctermbg=0       ctermfg=1

" Tab line
highlight TabLineFill   ctermbg=234     ctermfg=7       term=NONE   cterm=NONE
highlight TabLine       ctermbg=236     ctermfg=230     term=NONE   cterm=NONE
highlight TabLineSel    ctermbg=240     ctermfg=7       term=NONE   cterm=bold

" VSplit
highlight WinSeparator  ctermbg=0

" Debugger
highlight debugPC       ctermbg=24

" FZF
highlight Pmenu         ctermfg=252     ctermbg=233
highlight fzf1          ctermfg=161     ctermbg=0
highlight fzf2          ctermfg=151     ctermbg=0
highlight fzf3          ctermfg=252     ctermbg=0

