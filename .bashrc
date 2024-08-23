#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

source ~/bash-scripts/git-prompt.sh

PROMPT_DIRTRIM=3
PROMPT_COMMAND='PS1_CMD1=$(__git_ps1 " (%s)")'

export PS1='\u@\h:\[\e[33m\]\w\[\e[38;5;154m\]${PS1_CMD1}\[\e[0m\]\$ '

alias ls='ls --color=auto'
alias grep='grep --color=auto'
alias l='ls -alh --color=auto'
alias ..='cd ..'

export PATH="/home/cybi/.cargo/bin:$PATH"
export PATH="/home/cybi/go/bin:$PATH"

export JAVA_HOME=/usr/lib/jvm/java-21-openjdk/
# export JAVA_HOME=/usr/lib/jvm/java-17-openjdk/

. "$HOME/.cargo/env"
