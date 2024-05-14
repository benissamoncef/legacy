#!/bin/ksh
# niepceron 7/11/1997 ajout tscmd.x pour ioscan (passage en 10.20) v1.2
export NOMBRE_ECRANS="`tscmd.x /etc/ioscan -f | grep -E graphics | wc -l`"
echo $NOMBRE_ECRANS
exit 0
