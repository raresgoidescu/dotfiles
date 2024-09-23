#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

source ~/bash-scripts/git-prompt.sh

ORANGE="\[\e[38;5;214m\]"
PURPLE="\[\e[38;5;56m\]"
SBLUE="\[\e[38;5;39;1m\]"
# SWHITE="\[\e[97;1m\]"
RESET="\[\033[0m\]"

PROMPT_DIRTRIM=3

ps1_prompt() {
    local ps1_exit=$?
 
    if [ $ps1_exit -eq 0 ]; then
        ps1_status=""
    else
        ps1_status=`echo -e " !$? "`
 
    fi
 
    ps1_git=''
    if [ "$(__git_ps1 %s)" != '' ]; then
        ps1_git=" "$(__git_ps1 "(%s)")""
    fi
 
    PS1="$SWHITE\u@\h \w$RESET$SBLUE${ps1_git}$RESET$SWHITE${ps1_status} \$$RESET "
}
 
# preserve earlier PROMPT_COMMAND entries...
PROMPT_COMMAND="ps1_prompt;$PROMPT_COMMAND"

# very hacker
setxkbmap -option caps:escape_shifted_capslock
# cycle through completions
# bind 'TAB:menu-complete'

export TERM=xterm-256color

alias v='NVIM_APPNAME=lazyvim nvim'
alias ls='ls --color=auto -F --group-directories-first'
alias l='ls -Falh --color=auto --group-directories-first'
alias grep='grep --color=auto'
alias ..='cd ..'
alias fman='compgen -c | fzf | xargs man'
alias idea='nohup idea < /dev/null > /dev/null 2>&1 &'
alias discrd='nohup discord --enable-features=UseOzonePlatform --ozone-platform=wayland < /dev/null > /dev/null 2>&1 &'

export PATH="$HOME/personal-projects/rust/password-manager/target/release/:$PATH"
export PATH="$HOME/.local/bin/:$PATH"

# Don't put duplicate lines in the history and do not add lines that start with a space
export HISTCONTROL=erasedups:ignoredups:ignorespace

# Go
export PATH="$HOME/go/bin:$PATH"

# Java
export JAVA_HOME=/usr/lib/jvm/java-21-openjdk/
# export JAVA_HOME=/usr/lib/jvm/java-17-openjdk/

# InteliJIDEA
export PATH="$HOME/intelij/idea-IU-242.21829.142/bin:$PATH"

# Rust Cargo
export PATH="$HOME/.cargo/bin:$PATH"
. "$HOME/.cargo/env"

# Ruby Gems
export GEM_HOME="$HOME/gems"
export PATH="$HOME/gems/bin:$PATH"
export PATH="$HOME/.local/share/gem/ruby/3.2.0/bin:$PATH"

export EDITOR=nvim
export VISUAL=nvim

