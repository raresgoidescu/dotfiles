#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

GREEN="\[\e[1;38;5;2m\]"
RED="\[\e[1;38;5;9m\]"
BLUE="\[\e[1;38;5;4m\]"
RESET="\[\033[0m\]"

source ~/.env_vars
source ~/.git-prompt.sh
export PROMPT_DIRTRIM=2

ps1_prompt() {
    local ps1_exit=$?
 
    if [ $ps1_exit -eq 0 ]; then
        ps1_status=""
    else
        ps1_status=`echo -e "[$?] "`
    fi
 
    ps1_git=''
    if [ "$(__git_ps1 %s)" != '' ]; then
        ps1_git=" $RED""git::""$(__git_ps1 "(%s)")"
    fi
 
    PS1="$GREEN\u$RESET $BLUE\w$RESET${ps1_git}$RESET\$ ${ps1_status}"
}
 
# preserve earlier PROMPT_COMMAND entries...
PROMPT_COMMAND="ps1_prompt;$PROMPT_COMMAND"

# very hacker
setxkbmap -option caps:escape_shifted_capslock
setxkbmap ro

# export TERM=xterm-256color

alias v='NVIM_APPNAME=lazyvim nvim'
alias vi='nvim'
alias hx='helix'

alias ls='ls --color=auto -F --group-directories-first'
alias l='ls -Falh --color=auto --group-directories-first'
alias grep='grep --color=auto'
alias ..='cd ..'
alias fman='compgen -c | fzf | xargs man'
alias idea='nohup idea < /dev/null > /dev/null 2>&1 &'
alias discrd='nohup discord --enable-features=UseOzonePlatform --ozone-platform=wayland < /dev/null > /dev/null 2>&1 &'

# Rust Cargo
# export PATH="$HOME/.cargo/bin:$PATH"
. "$HOME/.cargo/env"

# Don't put duplicate lines in the history and do not add lines that start with a space
# export HISTCONTROL=erasedups:ignoredups:ignorespace

export EDITOR=nvim
export VISUAL=nvim
