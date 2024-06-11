set autoscale
set grid
set style data lines

set multiplot layout 2,1

#------------- Graf 1 ----------------
set title "audio filtrado con filtro fir"
set xlabel   "muestras"
set ylabel   "x(n)"
plot  "y_fir.dat"

unset title
unset xlabel
unset ylabel

#------------- Graf 2 ----------------
set title "audio filtrado con filtro iir"
set xlabel "muestras"
set ylabel "y(n)"
plot "y_iir.dat"
