#!/bin/sh
# Preview image

image_file=${1}
scene_file=${2}
param_file=${3}

sleep 2s && feh --force-aliasing -F --keep-zoom-vp -R2 ${image_file} &
if ! type "mcrt" &> /dev/null; then
    ./mcrt ${image_file} ${scene_file} ${param_file}
else
    mcrt   ${image_file} ${scene_file} ${param_file}
fi
