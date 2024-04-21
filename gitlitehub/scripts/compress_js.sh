#! /bin/bash


JS_PATH=/home/long/GitliteServer/gitlitehub/static/js
JS_DIST_PATH=${JS_PATH}/dist
JS_SRC_PATH=${JS_PATH}/src

find $JS_SRC_PATH -type f -name '*.js' | sort | xargs cat > ${JS_DIST_PATH}/gitlitehub.js
