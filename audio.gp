set autoscale
set grid
set style data lines

set multiplot layout 2,1

#------------- Graf 1 ----------------
set title "Audio original"
set xlabel   "muestras"
set ylabel   "x(n)"
plot  "audio.dat"

unset title
unset xlabel
unset ylabel

#------------- Graf 2 ----------------
set title "Audio con tonos | 259Hz y 594Hz"
set xlabel "muestras"
set ylabel "x(n) + os1 + os2"
plot "x_tonos.dat"

unset title
unset xlabel
unset ylabel
