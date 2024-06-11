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
set title "Audio con tonos"
set xlabel "muestras"
set ylabel "y(n)"
plot "audio_filtrado.dat"

unset title
unset xlabel
unset ylabel
