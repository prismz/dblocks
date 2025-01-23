#!/bin/sh


playerctl -p spotify metadata > /dev/null
if [ $? -ne 0 ]; then
        exit 1
fi


convert_secs() {
        printf "%d:%02d" "$((10#$1 / 60))" "$((10#$1 % 60))";
}

status=$(playerctl -p spotify status)
artist=$(playerctl -p spotify metadata artist)
title=$(playerctl -p spotify metadata title)

position_raw=$(playerctl -p spotify position | sed 's/\./\n/g' | head -n 1)

# "no player could handle this command" ?
# echo "$(playerctl -p spotify metadata length)"

# UGLY
length_raw=$(($(playerctl -p spotify metadata | grep length | sed 's/ /\n/g' | tail -n 1) / 1000000))

position=$(convert_secs "$position_raw")
length=$(convert_secs "$length_raw")

echo "$status $artist - $title $position/$length"
