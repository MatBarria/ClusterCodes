#!/bin/bash
for i in 1 2 3 4 5 6 7 8 9
do
  ./send_GST_HSim.sh C $i
  ./send_GST_HSim.sh Fe $i
  ./send_GST_HSim.sh Pb $i
  ./send_GST_HSim.sh D $i
done
