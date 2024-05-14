# -------------------------------------------------------------
# -- Fichier permettant de faire une copie d ecran en jpeg
#     
# --------------------------------------------------------------
#
#Pour un fichier xwd de 739935
# avec quality = 1   jpg = 19183 
# avec quality = 5   jpg = 26908 
# avec quality =10   jpg = 42015 
# avec quality =50   jpg = 103118 
# avec quality =99   jpg = 368068 
# avec quality =500  jpg = 385366 


FIC_EVT=/produits/migrazur/appliSD/fichiers/stra/nomade/out/fmc
FIC_EVT2=/produits/migrazur/appliSD/fichiers/stra/nomade/out/fmc2
FIC_TFM=/produits/migrazur/appliSD/fichiers/stra/nomade/out/tfm
FIC_TFM2=/produits/migrazur/appliSD/fichiers/stra/nomade/out/tfm2
FIC_REPONSE=/produits/migrazur/appliSD/fichiers/stra/nomade/out/reponse_synthese
FIC_HEURE=/produits/migrazur/appliSD/fichiers/stra/nomade/out/heure_reception
FIC_HEURE2=/produits/migrazur/appliSD/fichiers/stra/nomade/out/heure_reception
FIC_TDP=/produits/migrazur/appliSD/fichiers/stra/nomade/out/tdp
FIC_TDP2=/produits/migrazur/appliSD/fichiers/stra/nomade/out/tdp2
FIC_TFM_SYNTHESE=/produits/migrazur/appliSD/fichiers/stra/nomade/out/tfm_synthese
FIC_TFM_SYNTHESE2=/produits/migrazur/appliSD/fichiers/stra/nomade/out/tfm_synthese2
FIC_AST=/produits/migrazur/appliSD/fichiers/stra/nomade/out/astreintes
FIC_AST2=/produits/migrazur/appliSD/fichiers/stra/nomade/out/astreintes2



mv $FIC_EVT $FIC_EVT2
mv $FIC_AST $FIC_AST2
mv $FIC_TFM $FIC_TFM2
mv $FIC_TDP $FIC_TDP2
mv $FIC_HEURE $FIC_HEURE2
mv $FIC_TFM_SYNTHESE $FIC_TFM_SYNTHESE2


gzip -f $FIC_HEURE
mv $FIC_HEURE.gz $FIC_HEURE

exit

rm $FIC_EVT2
gzip -f $FIC_EVT
mv $FIC_EVT.gz $FIC_EVT2

gzip -f $FIC_TFM
mv $FIC_TFM.gz $FIC_TFM

gzip -f $FIC_REPONSE
mv $FIC_REPONSE.gz $FIC_REPONSE

gzip -f $FIC_HEURE
mv $FIC_HEURE.gz $FIC_HEURE

gzip -f $FIC_TDP
mv $FIC_TDP.gz $FIC_TDP

gzip -f $FIC_TFM_SYNTHESE
mv $FIC_TFM_SYNTHESE.gz $FIC_TFM_SYNTHESE
exit 0  	

