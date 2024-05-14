/*E*/
/*  Fichier : @(#)xzae910sp.prc  1.1      Release : 1.1        Date : 20/06/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae910sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture de la liste des CFE
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL  16/04/18        : Creation CTRL_FEUX 1.1 DEM1284
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE910_Lect_Liste_CFE
*
* Arguments en entree
*
* Arguments en sortie
* smallint 	va_Numero_out
* char(10)	va_Nom_out
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des elements de la table EQT_GEN de type 66
-------------------------------------------------------*/

 
#define XZAE910_LECT_LISTE_CFE	"XZAE910" 
