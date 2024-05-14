# 
# Script pour Cyclope permettant de lister tous les evenements sur l'A50
# du PR 68750 au PR 72549 pour la veille
#
export SYBASE=/produits/sybase15
. /produits/sybase15/SYBASE.sh
export LANG=

DATE=`date +%y%m%d%H%M`

echo "PRC..myXZAE01 3,68750,72549,0,null,3,-1,-1
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DS -w2000 -Jiso_1 > /produits/migrazur/appliSD/fichiers/stra/cyclope/CyclopeListeEvt$DATE.tmp
