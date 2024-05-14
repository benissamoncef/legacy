/*E*/
/*  Fichier : @(#)xzae920sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae920sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture des libelles de plans de feux associes a un controleur
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL 	16/04/18        : Creation CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE920_Lect_Lib_CFE
*
* Arguments en entree
* smallint   va_Notyp_in
*
* Arguments en sortie
* smallint 	va_NumeroCFE_out
* int		va_Code_out
* char(100)	va_Libelle_out
* bit		va_Valide_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des libelles associes aux plans de feux d'un controleur en entree (base CFG)
-------------------------------------------------------*/

 
#define XZAE920_LECT_LIB_CFE	"XZAE920" 
