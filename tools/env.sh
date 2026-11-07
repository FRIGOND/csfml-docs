# source tools/env.sh — outils locaux sans sudo (xvfb, pkgconf) et préfixes CSFML.
export PATH="$HOME/local/root/usr/bin:$PATH"
export P3="$HOME/csfml-3.0" P2="$HOME/csfml-2.6"
export XKB_DIR="$HOME/local/root/usr/share/X11/xkb"
start_xvfb() { # démarre Xvfb :99 s'il n'tourne pas
  pgrep -x Xvfb >/dev/null || (Xvfb :99 -screen 0 1024x768x24 -nolisten tcp -xkbdir "$XKB_DIR" >/dev/null 2>&1 &)
  sleep 1; export DISPLAY=:99 LIBGL_ALWAYS_SOFTWARE=1
}
