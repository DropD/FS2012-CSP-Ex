app="ising"

cmake -DCMAKE_BUILD_TYPE=Release --mario-random=on .
make

tb=0
te=8
ts=100
ns=200

for l in 10 15 20 25
do
    params="$l $tb $te $ts $ns" 
    file="l$l.par"
    echo "$params" > "$file"
    ./$app $file &
done
