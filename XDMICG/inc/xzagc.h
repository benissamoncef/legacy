/*E Fichier : $Id: xzagc.h,v 1.3 1995/09/12 19:27:17 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/09/12 19:27:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME _SS_SYSTEME_
------------------------------------------------------
* MODULE XZAE * FICHIER xzagc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
* C.T.  15/11/94	: Creation V 1.1
* C.T.  12/09/95	: Ajout constantes XZAGC_NB_CARACT_A_LIRE V 1.2
------------------------------------------------------ */


/* fichiers inclus */

/* definitions de constantes exportees */
#define	 XZAGC_NB_CARACT_A_LIRE	80

/* type de trace pour la suppression des enregistrements */

#define XZAGC_IMP_NBR	0	/* afficher le nombre d'enregistrements concernes par table */
#define XZAGC_IMP_ENR	1	/* afficher toutes les infos sur les enregistrements concernes */

/* code d'erreur de la sauvegarde de base */
#define XZAGC_VAR_ENV_INC	4
#define XZAGC_DISP_NON_POS	5
#define XZAGC_SVG_FORK		6
#define XZAGC_SVG_KO		7

/* code retour de la commande system quand c'est ko */
/* : probleme de fork                              */
#define XZAGC_CMD_SYS_FORK_KO	-1

/* mode de sauvegarde de base */
#define XZAGC_SVG_GLOB	"globale"
#define XZAGC_SVG_INCR	"incrementale"

/* longueur du nom du volume de sauvegarde */
#define XZAGC_VOLUME	7

/* titre de la fenetre de compte rendu de la sauvegarde de base */
#define XZAGC_TITRE_SVG		"Sauvegarde de base"
#define XZAGC_TITRE_REST	"Restauration de base"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

