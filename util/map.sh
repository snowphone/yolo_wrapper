data=$1
cfg=$2
weight_folder=$3

for weight in $(ls $weight_folder)
do
    epoch=$(echo $weight | grep -Po '\d+')
    accuracy=$(./darknet detector map $data $cfg $weight_folder$weight 2>&1 | grep --line-buffered -Po 'ap = \K(\d+\.\d+ %)')
    printf '%s, %s' $epoch $accuracy 
    echo ""
done
