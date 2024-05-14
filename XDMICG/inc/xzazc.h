/*E*/
/*  Fichier : 	$Id: xzazc.h,v 1.2 1995/05/18 15:11:41 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/18 15:11:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZAZ * FICHIER xzazc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* reception de travaux de satir
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	17/01/95	: Creation
* B.G.  18/05/95	: Ajout de XZAZC_FMC_SATIR_MODIF (1.2)
------------------------------------------------------ */

/* definitions de constantes exportees */

/* code d'erreur de xzaz01 */
#define XZAZC_DEBUT_INC	6	/* date prevu < date actuelle + 24H */
#define XZAZC_LOC_INC	7	/* cpnfiguration des voies incorrectes */

/* texte de origine creation pour la fiche travaux */
#define XZAZC_FMC_SATIR_MODIF	"SATIR MODIFIE"
#define XZAZC_FMC_SATIR	"SATIR"
