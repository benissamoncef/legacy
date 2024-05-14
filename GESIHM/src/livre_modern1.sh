# Script de livraison de la config eqt vers tous les modern1
#
# PN1 1.1 		: creation
# PNI 1.2	02/07/08	: suppression de MOD1CAA,MOD1PRA,MOD1VDA
# JPL 1.3	20/03/17	: Suppression de deux machines; renommage pour regionalisation
# PN1 1.4	28/05/21	: ajout MOD1TL et correctif pour compatibilité linux
#-------------------------------------------------------------------------------

for i in MOD1CI MOD1TST  MOD1VC MOD1DP MOD1TL
do
echo $i
if ping -c 1 $i 1>/dev/null 2>/dev/null; then
rsh $i "/produits/modern1/outils/install_config_modern1.sh"
else
echo $i inaccessible
fi
done
echo "<Taper une touche pour finir>"
read c
exit 0
