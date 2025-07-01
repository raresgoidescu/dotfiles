#!/bin/bash

US="us"
IT="ro_us"
current=$(setxkbmap -query | grep layout | cut -f6 -d ' ')
if [ "$current" = $US ]; then
    setxkbmap $IT
	notify-send "$IT"
else
    setxkbmap $US
	notify-send "$US"
fi
