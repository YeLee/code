#!/bin/bash
FIREFOX_URL="https://addons.mozilla.org/firefox/downloads"
EXT_DIR="$HOME/.mozilla/firefox/.config/extensions"

mkdir -p $EXT_DIR
wget "$FIREFOX_URL/latest/62/addon-62-latest.xpi" -O "$EXT_DIR/{c4dc572a-3295-40eb-b30f-b54aa4cdc4b7}.xpi"
wget "$FIREFOX_URL/latest/953/addon-953-latest.xpi" -O "$EXT_DIR/{455D905A-D37C-4643-A9E2-F6FEFAA0424A}.xpi"
wget "$FIREFOX_URL/latest/2324/addon-2324-latest.xpi" -O "$EXT_DIR/{1280606b-2510-4fe0-97ef-9b5a22eafe30}.xpi"
wget "$FIREFOX_URL/latest/4891/addon-4891-latest.xpi" -O "$EXT_DIR/vimperator@mozdev.org.xpi"
wget "$FIREFOX_URL/file/123595/firebug-latest-fx.xpi" -O "$EXT_DIR/firebug@software.joehewitt.com.xpi"
#wget "$FIREFOX_URL/latest/235854/addon-235854-latest.xpi" -O "$EXT_DIR/pentadactyl@dactyl.googlecode.com.xpi"
