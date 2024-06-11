set autoscale
set grid
set style data lines

set multiplot layout 2,1

#------------- Graf 1 ----------------
set title "Oscilador 1 | 259 Hz"
set xlabel   "muestras"
set ylabel   "x(n)"
plot  "os1.dat"

unset title
unset xlabel
unset ylabel

#------------- Graf 2 ----------------
set title "Oscilador 2 | 594 Hz"
set xlabel "muestras"
set ylabel "y(n)"
plot "os2.dat"

unset title
unset xlabel
unset ylabel
