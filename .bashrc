#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

source ~/bash-scripts/git-prompt.sh

PROMPT_DIRTRIM=3
PROMPT_COMMAND='PS1_CMD1=$(__git_ps1 " (%s)")'

export PS1='\u@\h \w\[\e[38;5;154m\]${PS1_CMD1}\[\e[0m\]\$ '

alias ls='ls --color=auto -F --group-directories-first'
alias l='ls -Falh --color=auto --group-directories-first'
alias grep='grep --color=auto'
alias ..='cd ..'
alias idea='nohup idea &'

# Don't put duplicate lines in the history and do not add lines that start with a space
export HISTCONTROL=erasedups:ignoredups:ignorespace

# Go
export PATH="/home/cybi/go/bin:$PATH"

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

