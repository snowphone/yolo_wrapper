


if [ -z "$1" ]
then
    printf 'Usage: %s <img foler>\n' $0
    exit 1
fi

folder=$1

for text in $(ls $folder/*.txt)
do
    sed -ir 's/^1/0/' $text
done
