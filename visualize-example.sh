#!/usr/bin/env bash
rm *.png
rm *.gif
dot -Tpng 1.txt -Gdpi=300  -o 1.png
dot -Tpng 2.txt -Gdpi=300  -o 2.png
dot -Tpng 3.txt -Gdpi=300  -o 3.png

convert -loop 0 -delay 300 *.png viz.gif
xdg-open viz.gif