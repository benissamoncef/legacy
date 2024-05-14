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
 
FIC_XWD=/tmp/shot_ecran.xwd
FIC_GIF=/produits/migrazur/appliSD/fichiers/stra/nomade/out/vue
FIC_TMP=/produits/migrazur/appliSD/fichiers/stra/nomade/out/vue2

xwd -silent -nobdrs -out $FIC_XWD -name "SYNOPTIQUES D'EXPLOITATION"

convert -quality 500 $FIC_XWD gif:$FIC_TMP
#/opt/ImageMagick/bin/convert $FIC_XWD bmp:$FIC_TMP

gzip -f $FIC_TMP

mv $FIC_TMP.gz $FIC_GIF

exit 0  	

