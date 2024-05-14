/*E Fichier : $Id: eimi_mdon.h,v 1.2 2021/01/06 09:00:06 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2021/01/06 09:00:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER eimi_mdon.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Inclusion des fichiers generalement necessaires par inclusion de elexi_common.h
* Declaration en Extern des variables globales modules eimi_xxx.c.
* Fichier inclus ds. tous les mods. eimi_xxx.c sauf eimi_dir.c
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   01/03/18 :      Creation CTRL_FEUX 1.1 DEM1284
* JMG  05/01/21 : bloc inter 1.2 DEM SAE-215
------------------------------------------------------ */

#ifndef eimi_mdon
#define eimi_mdon

/* fichiers inclus */

#include "ex_mcom.h"

/* definitions de constantes exportees */

/* definitions de types exportes */
typedef struct {
	XDY_AdresseIP	adresse;
	XDY_PortIP	port;
	char		tampon[1000];
} T_Tampon;
#define C_MAX_TAMPON	50

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK	*vg_SockList ;			/* liste globale des sockets */
extern XZEXT_IP_Config	vg_Config;			/* config tache TELxi */
extern XZEXT_ST_Sockets	vg_DesSocks;			/* struct. descs. sockets typiques tache TELxi */
extern XZEXT_Liste_Cmdes_LCR_IP	*pg_ListeCmdesLCR;	/* liste cmdes. LCR empilees */
extern char		vg_idpassw[18];
extern int		vg_EtatST;

/* declaration de fonctions externes */

#endif
